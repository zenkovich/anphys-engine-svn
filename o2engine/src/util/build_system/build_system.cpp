#include "build_system.h"

#include "util/xml_tools.h"
#include "build_config.h"
#include "build_info.h"
#include "atlas_info.h"
#include "util/file_system/file_system.h"
#include "util/serialize_util.h"
#include "nonbuild_files_builder.h"
#include "atlases_builder.h"

OPEN_O2_NAMESPACE

DECLARE_SINGLETON(cBuildSystem);


cBuildSystem::cBuildSystem(const string& projectPath):
	mActiveBuildConfig(NULL), mReady(false)
{
	if (!loadProject(projectPath))
	{
		createEmptyProject("o2proj", projectPath);
		createEmptyBuildConfig("win32");
	}
	
	mBuildStages.push_back(mnew cAtlasesBuildingStage(this));
	mBuildStages.push_back(mnew cNonBuildFilesBuildStage(this));
}


cBuildSystem::~cBuildSystem()
{
	saveConfig();
	RELEASE_VECTOR(BuildConfigsVec, mBuildConfigs);
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

	RELEASE_VECTOR(BuildConfigsVec, mBuildConfigs);
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
	
	loadBuildInfo(!forcible);
	updateBuildConfig();
	processBuildStages();
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

void cBuildSystem::updateBuildConfig()
{	
	BuildFileInfoVec assetsFiles;
	gatherAssetsFilesMeta(assetsFiles);

	//filter assets metas
	int cutMetaPathIdx = (mProjectPath + "/assets/").length();
	FOREACH(BuildFileInfoVec, assetsFiles, meta)
		(*meta)->mLocation.mPath = (*meta)->mLocation.mPath.substr(cutMetaPathIdx);

	//search removed files
	string assetsPath = getAssetsPath();

	//search new files
	FOREACH(BuildFileInfoVec, assetsFiles, asMetaIt)
	{
		bool exist = false;
		FOREACH(BuildFileInfoVec, mActiveBuildConfig->mFileInfos, metaIt)
		{
			if ((*asMetaIt)->mLocation == (*metaIt)->mLocation)
			{
				exist = true;

				if (**asMetaIt != **metaIt)
				{
					**metaIt = **asMetaIt;
				}

				break;
			}
		}

		if (!exist) 
		{
			mActiveBuildConfig->addFile((*asMetaIt)->clone());
		}
	}

	//search removed files
	for(BuildFileInfoVec::iterator metaIt = mActiveBuildConfig->mFileInfos.begin(); metaIt != mActiveBuildConfig->mFileInfos.end();)
	{
		bool exist = false;
		FOREACH(BuildFileInfoVec, assetsFiles, asMetaIt)
		{
			if ((*asMetaIt)->mLocation == (*metaIt)->mLocation)
			{
				exist = true;
				break;
			}
		}

		if (!exist)
		{ 
			safe_release(*metaIt);
			metaIt = mActiveBuildConfig->mFileInfos.erase(metaIt);
		}
		else metaIt++;
	}

	RELEASE_VECTOR(BuildFileInfoVec, assetsFiles);
}

void cBuildSystem::gatherAssetsFilesMeta(BuildFileInfoVec& filesMeta)
{
	cPathInfo assetsPathInfo = getFileSystem().getPathInfo(mProjectPath + "/Assets");
	gatherAssetsFilesMetaFromFolder(assetsPathInfo, filesMeta);
}

void cBuildSystem::gatherAssetsFilesMetaFromFolder(cPathInfo& pathInfo, BuildFileInfoVec& filesMeta)
{
	FOREACH(cPathInfo::FilesVec, pathInfo.mFiles, fileInfo) {
		if (fileInfo->mPath.rfind(".meta.xml") == fileInfo->mPath.length() - 9)
			continue;

		filesMeta.push_back(createFileMetaFromFileInfo(*fileInfo));
	}

	FOREACH(cPathInfo::PathsVec, pathInfo.mPaths, pathInfoIt)
	{
		filesMeta.push_back(createFileMetaFromPathInfo(*pathInfoIt));
		gatherAssetsFilesMetaFromFolder(*pathInfoIt, filesMeta);
	}
}

cBuildFileInfo* cBuildSystem::createFileMetaFromPathInfo(const cPathInfo& pathinfo)
{
	cBuildFileInfo* res = mnew cBuildPathInfo();
	res->mType = cBuildFileInfo::MT_FOLDER;
	
	res->mLocation.mPath = pathinfo.mPath;
	res->mBuildIncluded = true;
	res->mSize = 0;

	loadFileMeta(res);

	return res;
}

cBuildFileInfo* cBuildSystem::createFileMetaFromFileInfo(const cFileInfo& fileInfo)
{
	cBuildFileInfo* res;
	if (fileInfo.mFileType == cFileType::FT_IMAGE) 
	{
		res = mnew cBuildImageInfo();
		res->mType = cBuildFileInfo::MT_IMAGE;
	}
	else
	{
		res = mnew cBuildFileInfo();
		res->mType = cBuildFileInfo::MT_FILE;
	}
	
	res->mLocation.mPath = fileInfo.mPath;
	res->mBuildIncluded = true;
	res->mSize = fileInfo.mSize;
	res->mWritedTime = fileInfo.mEditDate;

	loadFileMeta(res);

	return res;
}

void cBuildSystem::saveBuildInfo()
{
	cSerializer outSer(cSerializer::ST_SERIALIZE);
	mBuildInfo->serialize(&outSer);
	outSer.save(getBuildAssetsPath() + "/buildInfo");
}

void cBuildSystem::loadFileMeta(cBuildFileInfo* meta, const string& pathPrefix /*= ""*/)
{
	cSerializer metaSerz;
	if (metaSerz.load(pathPrefix + meta->mLocation.mPath + ".meta"))
	{
		int metaId;
		metaSerz.serialize(metaId, "id");
		meta->mLocation.mId = metaId;
	}
	else createFileMeta(meta, pathPrefix);
}

void cBuildSystem::createFileMeta(cBuildFileInfo* meta, const string& pathPrefix /*= ""*/)
{
	meta->mLocation.mId = genNewMetaId();

	cSerializer metaSerz(cSerializer::ST_SERIALIZE);
	metaSerz.serialize(meta->mLocation.mId, "id");
	metaSerz.save(pathPrefix + meta->mLocation.mPath + ".meta");
}

void cBuildSystem::processBuildStages()
{
	FOREACH(BuildStagesVec, mBuildStages, stage)
		(*stage)->process();
}

uint32 cBuildSystem::genNewMetaId() const
{
	return rand()%(UINT_MAX - 1) + 1;
}

cImageAtlasInfo* cBuildSystem::createImageAtlas( const string& name, const vec2f& maxSize, 
	                                             cBuildPathInfo* attachingPath /*= NULL*/ )
{
	return mActiveBuildConfig->addAtlas(name, maxSize, attachingPath);
}

void cBuildSystem::removeAtlas( const string& name )
{
	mActiveBuildConfig->removeAtlas(name);
}

cImageAtlasInfo* cBuildSystem::getAtlas( const string& name )
{
	return mActiveBuildConfig->getAtlas(name);
}

CLOSE_O2_NAMESPACE