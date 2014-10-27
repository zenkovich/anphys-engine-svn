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
	
	mBuildStages.add(mnew cNonBuildFilesBuildStage(this));
	mBuildStages.add(mnew cAtlasesBuildingStage(this));
}


cBuildSystem::~cBuildSystem()
{
	saveConfig();
	release_array(BuildConfigsArr, mBuildConfigs);
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
	foreach(BuildConfigsArr, mBuildConfigs, conf)
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

	release_array(BuildConfigsArr, mBuildConfigs);
	mActiveBuildConfig = NULL;
	mReady = false;
}

void cBuildSystem::createEmptyBuildConfig(const string& buildName)
{
	saveConfig();

	mActiveBuildConfig = mnew cBuildConfig();
	mActiveBuildConfig->mName = buildName;
	mBuildConfigs.add(mActiveBuildConfig);

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
		buildInfoSerialz.serialize(mBuildInfo, "info"); 
}

void cBuildSystem::updateBuildConfig()
{	
	BuildFileInfoArr assetsFiles;
	gatherAssetsFileInfos(assetsFiles);

	//filter assets metas
	int cutMetaPathIdx = (mProjectPath + "/assets/").length();
	foreach(BuildFileInfoArr, assetsFiles, meta)
		(*meta)->mLocation.mPath = (*meta)->mLocation.mPath.substr(cutMetaPathIdx);

	//search removed files
	string assetsPath = getAssetsPath();

	hlog("Searching removed files:");

	//search removed files
	foreach_rem(BuildFileInfoArr, mActiveBuildConfig->mFileInfos, metaIt)
	{
		hlog("Processing asset: %s ...", (*metaIt)->mLocation.mPath.c_str());

		bool exist = false;
		foreach(BuildFileInfoArr, assetsFiles, asMetaIt)
		{
			if ((*asMetaIt)->mLocation == (*metaIt)->mLocation)
			{
				exist = true;
				break;
			}
		}

		if (!exist)
		{ 
			hlog("Removed!");

			safe_release(*metaIt);
			mActiveBuildConfig->mFileInfos.remove(metaIt);
		}
		else metaIt++;
	}

	hlog("Searching new files:");

	//search new files
	foreach(BuildFileInfoArr, assetsFiles, asMetaIt)
	{
		hlog("Processing asset: %s ...", (*asMetaIt)->mLocation.mPath.c_str());

		bool exist = false;
		foreach(BuildFileInfoArr, mActiveBuildConfig->mFileInfos, metaIt)
		{
			if ((*asMetaIt)->mLocation == (*metaIt)->mLocation)
			{
				hlog("Already exist");

				exist = true;

				if (**asMetaIt != **metaIt)
				{
					hlog("Changed!");

					(*metaIt)->mLocation = (*asMetaIt)->mLocation;
					(*metaIt)->mSize = (*asMetaIt)->mSize;
					(*metaIt)->mWritedTime = (*asMetaIt)->mWritedTime;
				}

				break;
			}
		}

		if (!exist) 
		{
			hlog("New asset. Adding to build info");

			mActiveBuildConfig->addFile((*asMetaIt)->clone());
		}
	}

	//update paths infos inside files arrays
	foreach(BuildFileInfoArr, mActiveBuildConfig->mFileInfos, fileIt) 
	{
		if ((*fileIt)->mType == cBuildFileInfo::MT_FOLDER)
		{
			cBuildPathInfo* path = static_cast<cBuildPathInfo*>(*fileIt);
			path->updateInsideFiles(mActiveBuildConfig->mFileInfos);
		}
	}

	release_array(BuildFileInfoArr, assetsFiles);
	mActiveBuildConfig->updateRootPathFiles();

	mActiveBuildConfig->refreshAtlases();
}

void cBuildSystem::gatherAssetsFileInfos(BuildFileInfoArr& filesMeta)
{
	cPathInfo assetsPathInfo = getFileSystem().getPathInfo(mProjectPath + "/Assets");
	gatherAssetsFileInfosFromFolder(assetsPathInfo, filesMeta);
}

void cBuildSystem::gatherAssetsFileInfosFromFolder(cPathInfo& pathInfo, BuildFileInfoArr& filesMeta)
{
	foreach(cPathInfo::FilesArr, pathInfo.mFiles, fileInfo) {
		if (fileInfo->mPath.rfind(".meta.xml") == fileInfo->mPath.length() - 9)
			continue;

		filesMeta.add(createBuildFileInfo(*fileInfo));
	}

	foreach(cPathInfo::PathsArr, pathInfo.mPaths, pathInfoIt)
	{
		filesMeta.add(createBuildFileMeta(*pathInfoIt));
		gatherAssetsFileInfosFromFolder(*pathInfoIt, filesMeta);
	}
}

cBuildFileInfo* cBuildSystem::createBuildFileMeta(const cPathInfo& pathinfo)
{
	cBuildFileInfo* res = mnew cBuildPathInfo();
	res->mType = cBuildFileInfo::MT_FOLDER;
	
	res->mLocation.mPath = pathinfo.mPath;
	res->mBuildIncluded = true;
	res->mSize = 0;

	loadFileMeta(res);

	return res;
}

cBuildFileInfo* cBuildSystem::createBuildFileInfo(const cFileInfo& fileInfo)
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
	outSer.serialize(mBuildInfo, "info");
	outSer.save(getBuildAssetsPath() + "/buildInfo");
}

void cBuildSystem::loadFileMeta(cBuildFileInfo* info, const string& pathPrefix /*= ""*/)
{
	cSerializer metaSerz;
	if (metaSerz.load(pathPrefix + info->mLocation.mPath + ".meta"))
	{
		int metaId;
		metaSerz.serialize(metaId, "id");
		info->mLocation.mId = metaId;
	}
	else createFileMeta(info, pathPrefix);
}

void cBuildSystem::createFileMeta(cBuildFileInfo* info, const string& pathPrefix /*= ""*/)
{
	info->mLocation.mId = genNewMetaId();

	cSerializer metaSerz(cSerializer::ST_SERIALIZE);
	metaSerz.serialize(info->mLocation.mId, "id");
	metaSerz.save(pathPrefix + info->mLocation.mPath + ".meta");
}

void cBuildSystem::processBuildStages()
{
	foreach(BuildStagesArr, mBuildStages, stage)
		(*stage)->process();
}

uint32 cBuildSystem::genNewMetaId() const
{
	uint32 res = rand()%(UINT_MAX - 1) + 1;
	hlog("new id: %i", res);
	return res;
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