#include "build_system.h"

#include "util/xml_tools.h"
#include "build_config.h"
#include "build_info.h"
#include "util/file_system/file_system.h"

OPEN_O2_NAMESPACE

DECLARE_SINGLETON(cBuildSystem);

SERIALIZE_METHOD_IMPL(cBuildSystem::FileMeta)
{
	SERIALIZE_ID(mPath, "path");
	SERIALIZE_ID(mBuildIncluded, "buildIncluded");
	SERIALIZE_ID(mSize, "size");
	SERIALIZE_ID(mWritedTime, "writedTime");

	return true;
}

SERIALIZE_INHERITED_METHOD_IMPL(cBuildSystem::ImageFileMeta)
{
	SERIALIZE_ID(mAtlasName, "atlas");

	return true;
}

cBuildSystem::cBuildSystem(const string& projectPath)
{
	mProjectPath = projectPath;

	string configFile = mProjectPath + "/projConfigs";
	cSerializer serializer;
	if (!serializer.load(configFile))
	{
		logError("Failed to load build config file: %s", configFile.c_str());
		mActiveBuildConfig = mnew cBuildConfig();
		mActiveBuildConfig->mName = "win32";
		mProjectName = "template";
		mBuildConfigs.push_back(mActiveBuildConfig);
		saveConfig();
		return;
	}

	serializer.serialize(mProjectName, "projectName");
	serializer.serialize(mBuildConfigs, "buildConfigs");

	string activeBuildConfig;
	serializer.serialize(activeBuildConfig, "activeBuildConfig");

	FOREACH(BuildConfigsVec, mBuildConfigs, conf)
		if ((*conf)->mName == activeBuildConfig)
			mActiveBuildConfig = *conf;
}

cBuildSystem::~cBuildSystem()
{
	saveConfig();
	safe_release(mBuildInfo);
}

void cBuildSystem::saveConfig()
{
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

void cBuildSystem::rebuildAssets( bool forcible /*= false*/ )
{
	hlog("Rebuilding assets %s", forcible ? "forcible":"");

	if (forcible)
		cleanUpBuildedAssest();
	
	string buildAssetsPath = getBuildAssetsPath();

	mBuildInfo = mnew cBuildInfo();

	cSerializer buildInfoSerialz;
	string buildInfoFile = buildAssetsPath + "buildInfo.xml";
	if (!buildInfoSerialz.load(buildInfoFile))
	{
		if (!forcible)
			logWarning("Failed to load build info file: %s\nRebuilding all assets.", buildInfoFile.c_str());
	}
	else
	{
		mBuildInfo->serialize(&buildInfoSerialz);
	}

	updateBuildConfig();
	copyNonBuildingFiles();
}

void cBuildSystem::cleanUpBuildedAssest()
{
	hlog("Cleanup assets");

	getFileSystem().removeDirectory(getBuildAssetsPath());
	getFileSystem().createDirectory(getBuildAssetsPath());
}

void cBuildSystem::updateBuildConfig()
{
	cPathInfo assetsPathInfo = getFileSystem().getPathInfo(mProjectPath + "/Assets/");
	updateBuildConfigPath(assetsPathInfo);
}

void cBuildSystem::updateBuildConfigPath(cPathInfo pathInfo)
{
	FOREACH(cPathInfo::FilesVec, pathInfo.mFiles, fileInfo)
	{
		bool exist = false;
		FOREACH(cBuildInfo::FilesMetaVec, mBuildInfo->mFilesMeta, fileMeta)
		{
			if ((*fileMeta)->mPath == fileInfo->mPath)
			{
				exist = true;
				break;
			}
		}

		if (!exist)
			mActiveBuildConfig->addFile(*fileInfo);
	}

	FOREACH(cPathInfo::PathsVec, pathInfo.mPaths, pathInf)
		updateBuildConfigPath(*pathInf);
}

void cBuildSystem::copyNonBuildingFiles()
{

}

CLOSE_O2_NAMESPACE