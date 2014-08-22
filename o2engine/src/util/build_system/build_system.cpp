#include "build_system.h"

#include "util/xml_tools.h"
#include "build_config.h"
#include "build_info.h"
#include "util/file_system/file_system.h"
#include "util/serialize_util.h"

OPEN_O2_NAMESPACE

DECLARE_SINGLETON(cBuildSystem);

std::map<string, cSerializable*> gSerializeTypesContainer::mSamples;

SERIALIZE_METHOD_IMPL(cBuildSystem::FileMeta)
{
	SERIALIZE_ID(mPath, "path");
	SERIALIZE_ID(mBuildIncluded, "buildIncluded");
	SERIALIZE_ID(mSize, "size");
	SERIALIZE_ID(mWritedTime, "writedTime");
	SERIALIZE_ID(mMetaId, "metaId");

	int itype = (int)mType;
	SERIALIZE_ID(itype, "type");
	mType = (Type)itype;

	return true;
}

cBuildSystem::cBuildSystem(const string& projectPath):
	mActiveBuildConfig(NULL), mReady(false), mLastMetaId(0)
{
	if (!loadProject(projectPath))
	{
		createEmptyProject("o2proj", projectPath);
		createEmptyBuildConfig("win32");
	}
}


cBuildSystem::~cBuildSystem()
{
	saveConfig();

	FOREACH(BuildConfigsVec, mBuildConfigs, conf)
		safe_release(*conf);
	mBuildConfigs.clear();

	safe_release(mBuildInfo);
}

bool cBuildSystem::loadProject(const string& projectPath)
{
	saveConfig();

	mReady = false;
	mProjectPath = projectPath;
	
	string activeBuildConfig;
	string configFile = mProjectPath + "/projConfigs";

	cSerializer serializer;
	if (!serializer.load(configFile))
	{
		logError("Failed to load build config file: %s", configFile.c_str());
		return false;
	}

	serializer.serialize(mProjectName, "projectName");
	serializer.serialize(mBuildConfigs, "buildConfigs");
	serializer.serialize(mLastMetaId, "lastMetaId");
	serializer.serialize(activeBuildConfig, "activeBuildConfig");

	setActiveBuildConfig(activeBuildConfig);

	mReady = true;

	return true;
}

void cBuildSystem::setActiveBuildConfig(const string& name)
{	
	FOREACH(BuildConfigsVec, mBuildConfigs, conf)
	{
		if ((*conf)->mName == name) 
		{
			mActiveBuildConfig = *conf;
			return;
		}
	}

	logWarning("Failed to setup active build config: %s\nSetting first %s", name.c_str(), mBuildConfigs[0]->mName.c_str());

	if (!mActiveBuildConfig)
		mActiveBuildConfig = mBuildConfigs[0];
}

void cBuildSystem::createEmptyProject(const string& projName, const string& projPath)
{
	saveConfig();

	mProjectName = projName;
	mProjectPath = projPath;

	FOREACH(BuildConfigsVec, mBuildConfigs, conf)
		safe_release(*conf);
	mBuildConfigs.clear();

	mActiveBuildConfig = NULL;

	mReady = false;
}

void cBuildSystem::createEmptyBuildConfig(const string& buildName)
{
	saveConfig();

	mActiveBuildConfig = mnew cBuildConfig();
	mActiveBuildConfig->mName = buildName;
	mBuildConfigs.push_back(mActiveBuildConfig);

	mReady = true;
}

void cBuildSystem::saveConfig()
{
	if (!mReady)
		return;

	cSerializer serializer(cSerializer::ST_SERIALIZE);
	string configFile = mProjectPath + "/projConfigs";
	
	serializer.serialize(mProjectName, "projectName");
	serializer.serialize(mBuildConfigs, "buildConfigs");
	serializer.serialize(mActiveBuildConfig->mName, "activeBuildConfig");
	serializer.serialize(mLastMetaId, "lastMetaId");

	serializer.save(configFile);
}

string cBuildSystem::getBuildAssetsPath() const
{
	return mProjectPath + "/builds/" + mActiveBuildConfig->mName + "/data/";
}

string cBuildSystem::getAssetsPath() const
{
	return mProjectPath + "/assets/";
}

void cBuildSystem::rebuildAssets( bool forcible /*= false*/ )
{
	hlog("Rebuilding assets %s", forcible ? "forcible":"");

	if (forcible)
		cleanUpBuildedAssest();

	AssetChangesInfo assetChanges;
	
	loadBuildInfo(!forcible);
	gatherAssetsChanges(assetChanges);

	saveBuildInfo();
	saveConfig();
}

void cBuildSystem::cleanUpBuildedAssest()
{
	hlog("Cleanup assets");

	if (!getFileSystem().removeDirectory(getBuildAssetsPath()))
		hlog("failed to remove dir");

	getFileSystem().createDirectory(getBuildAssetsPath());
}

void cBuildSystem::loadBuildInfo(bool errors /*= false*/)
{
	mBuildInfo = mnew cBuildInfo();

	string buildInfoFile = getBuildAssetsPath() + "/buildInfo";
	cSerializer buildInfoSerialz;
	if (!buildInfoSerialz.load(buildInfoFile))
	{
		if (errors)
			logWarning("Failed to load build info file: %s", buildInfoFile.c_str());
	}
	else
		mBuildInfo->serialize(&buildInfoSerialz);
}

void cBuildSystem::gatherAssetsChanges(AssetChangesInfo& assetChangesInfo)
{	
	FilesMetaVec assetsFiles;
	gatherAssetsFilesMeta(assetsFiles);

	//filter assets metas
	int cutMetaPathIdx = (mProjectPath + "/assets/").length();
	FOREACH(FilesMetaVec, assetsFiles, meta)
		(*meta)->mPath = (*meta)->mPath.substr(cutMetaPathIdx);

	//search removed files
	string assetsPath = getAssetsPath();
	FOREACH(FilesMetaVec, mActiveBuildConfig->mFilesMeta, metaIt)
	{
		FileMeta* meta = *metaIt;

		bool exist = false;
		bool changed = false;
		FOREACH(FilesMetaVec, assetsFiles, asMetaIt)
		{
			if (meta->mPath == (*asMetaIt)->mPath && (*asMetaIt)->mMetaId >= 0)
			{
				exist = true;
				if ((*metaIt)->mSize != (*asMetaIt)->mSize || (*metaIt)->mWritedTime != (*asMetaIt)->mWritedTime)
					changed = true;
				break;
			}
		}

		if (exist)
		{
			if (changed)
				assetChangesInfo.mChangedFiles.push_back(meta);

			continue;
		}

		assetChangesInfo.mRemovedFiles.push_back(*metaIt);
	}

	//search new files
	FOREACH(FilesMetaVec, assetsFiles, asMetaIt)
	{
		bool isNew = true;

		FOREACH(FilesMetaVec, mActiveBuildConfig->mFilesMeta, metaIt)
		{
			string fileRelAssets = assetsPath + (*metaIt)->mPath;
			if (fileRelAssets == (*asMetaIt)->mPath)
			{
				isNew = false;
				break;
			}
		}

		if (!isNew)
			continue;

		if ((*asMetaIt)->mMetaId < 0)
			createFileMeta(*asMetaIt, mProjectPath + "/assets/");

		assetChangesInfo.mNewFiles.push_back(*asMetaIt);
	}

	//check moved files
	for(FilesMetaVec::iterator newMetaIt = assetChangesInfo.mNewFiles.begin(); newMetaIt != assetChangesInfo.mNewFiles.end(); )
	{
		bool moved = false;
		FOREACH(FilesMetaVec, assetChangesInfo.mRemovedFiles, remMetaIt)
		{
			if ((*newMetaIt)->mMetaId == (*remMetaIt)->mMetaId)
			{
				moved = true;
				assetChangesInfo.mRemovedFiles.erase(remMetaIt);
				break;
			}
		}

		if (moved)
		{
			assetChangesInfo.mMovedFiles.push_back(*newMetaIt);
			newMetaIt = assetChangesInfo.mNewFiles.erase(newMetaIt);
		}
		else ++newMetaIt;
	}
}

void cBuildSystem::gatherAssetsFilesMeta(FilesMetaVec& filesMeta)
{
	cPathInfo assetsPathInfo = getFileSystem().getPathInfo(mProjectPath + "/Assets");
	gatherAssetsFilesMetaFromFolder(assetsPathInfo, filesMeta);
}

void cBuildSystem::gatherAssetsFilesMetaFromFolder(cPathInfo& pathInfo, FilesMetaVec& filesMeta)
{
	FOREACH(cPathInfo::FilesVec, pathInfo.mFiles, fileInfo)
		filesMeta.push_back(createFileMetaFromFileInfo(*fileInfo));

	FOREACH(cPathInfo::PathsVec, pathInfo.mPaths, pathInfoIt)
	{
		filesMeta.push_back(createFileMetaFromPathInfo(*pathInfoIt));
		gatherAssetsFilesMetaFromFolder(*pathInfoIt, filesMeta);
	}
}

cBuildSystem::FileMeta* cBuildSystem::createFileMetaFromPathInfo(const cPathInfo& pathinfo)
{
	FileMeta* res = mnew FileMeta();
	res->mType = FileMeta::MT_FOLDER;
	
	res->mPath = pathinfo.mPath;
	res->mBuildIncluded = true;
	res->mSize = 0;

	loadFileMeta(res);

	return res;
}

cBuildSystem::FileMeta* cBuildSystem::createFileMetaFromFileInfo(const cFileInfo& fileInfo)
{
	FileMeta* res = mnew FileMeta();
	if (fileInfo.mFileType == cFileType::FT_IMAGE)
		res->mType = FileMeta::MT_IMAGE;
	else
		res->mType = FileMeta::MT_FILE;
	
	res->mPath = fileInfo.mPath;
	res->mBuildIncluded = true;
	res->mSize = fileInfo.mSize;
	res->mWritedTime = fileInfo.mEditDate;

	loadFileMeta(res);

	return res;
}

void cBuildSystem::updateBuildConfig()
{
	cPathInfo assetsPathInfo = getFileSystem().getPathInfo(mProjectPath + "/Assets");
	updateBuildConfigPath(assetsPathInfo);
}

void cBuildSystem::updateBuildConfigPath(cPathInfo pathInfo)
{
	FOREACH(cPathInfo::FilesVec, pathInfo.mFiles, fileInfo)
	{
		fileInfo->mPath = fileInfo->mPath.substr((mProjectPath + "/Assets/").length());

		bool exist = false;
		FOREACH(cBuildInfo::FilesMetaVec, mActiveBuildConfig->mFilesMeta, fileMeta)
		{
			if ((*fileMeta)->mPath == fileInfo->mPath)
			{
				(*fileMeta)->mSize = fileInfo->mSize;
				(*fileMeta)->mWritedTime = fileInfo->mEditDate;
				exist = true;
				break;
			}
		}
// 
// 		if (!exist)
// 			mActiveBuildConfig->addFile(*fileInfo, mLastMetaId++);
	}

	FOREACH(cPathInfo::PathsVec, pathInfo.mPaths, pathInf)
		updateBuildConfigPath(*pathInf);
}

void cBuildSystem::copyNonBuildingFiles()
{
	string assetsPath = mProjectPath + "/Assets/";
	string dataPath = getBuildAssetsPath();
	FOREACH(cBuildInfo::FilesMetaVec, mActiveBuildConfig->mFilesMeta, fileMetaIt)
		processAssetCopying(*fileMetaIt, assetsPath, dataPath);
}

void cBuildSystem::processAssetCopying(FileMeta* fileMeta, const string& assetsPath, const string& dataPath)
{
	bool needCopy = true;
	FileMeta* copyDestFileMeta = NULL;
	FOREACH(cBuildInfo::FilesMetaVec, mBuildInfo->mFilesMeta, infFileMetaIt)
	{
		FileMeta* infFileMeta = *infFileMetaIt;

		if (fileMeta->mPath != infFileMeta->mPath)
			continue;

		if (fileMeta->mSize != infFileMeta->mSize || fileMeta->mWritedTime != infFileMeta->mWritedTime)
			copyDestFileMeta = infFileMeta;
		else
			needCopy = false;

		break;
	}

	if (!needCopy)
		return;

	string src = assetsPath + fileMeta->mPath;
	string dst = dataPath + fileMeta->mPath;

	if (copyDestFileMeta)
		getFileSystem().deleteFile(dst);

	getFileSystem().createDirectory(extractPath(dst));

	if (getFileSystem().copyFile(src, dst))
	{
		hlog("Copied asset %s to %s", src.c_str(), dst.c_str());
	}
	else
	{
		hlog("Failed to copy asset %s to %s", src.c_str(), dst.c_str());
	}

	if (copyDestFileMeta)
	{
		copyDestFileMeta->mSize = fileMeta->mSize;
		copyDestFileMeta->mWritedTime = fileMeta->mWritedTime;
	}
	else mBuildInfo->mFilesMeta.push_back(fileMeta);
}

void cBuildSystem::saveBuildInfo()
{
	cSerializer outSer(cSerializer::ST_SERIALIZE);
	mBuildInfo->serialize(&outSer);
	outSer.save(getBuildAssetsPath() + "/buildInfo");
}

void cBuildSystem::loadFileMeta(FileMeta* meta, const string& pathPrefix /*= ""*/)
{
	cSerializer metaSerz;
	if (metaSerz.load(pathPrefix + meta->mPath + ".meta"))
	{
		int metaId;
		metaSerz.serialize(metaId, "id");
		meta->mMetaId = metaId;
	}
	else meta->mMetaId = -1;
}

void cBuildSystem::createFileMeta(FileMeta* meta, const string& pathPrefix /*= ""*/)
{
	meta->mMetaId = mLastMetaId++;

	cSerializer metaSerz(cSerializer::ST_SERIALIZE);
	metaSerz.serialize(meta->mMetaId, "id");
	metaSerz.save(pathPrefix + meta->mPath + ".meta");
}

CLOSE_O2_NAMESPACE