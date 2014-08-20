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

	return true;
}

SERIALIZE_INHERITED_METHOD_IMPL(cBuildSystem::ImageFileMeta)
{
	SERIALIZE_ID(mAtlasName, "atlas");

	return true;
}

cBuildSystem::cBuildSystem(const string& projectPath):
	mActiveBuildConfig(NULL)
{
	gSerializeTypesContainer::outputRegisteredSamples();

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

	if (!mActiveBuildConfig)
		mActiveBuildConfig = mBuildConfigs[0];
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
	string buildInfoFile = buildAssetsPath + "/buildInfo";
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
	string assetsPath = mProjectPath + "/Assets/";
	string dataPath = getBuildAssetsPath();
	FOREACH(cBuildInfo::FilesMetaVec, mActiveBuildConfig->mFilesMeta, fileMetaIt)
	{
		FileMeta* fileMeta = *fileMetaIt;

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
			continue;

		string src = assetsPath + (*fileMetaIt)->mPath;
		string dst = dataPath + (*fileMetaIt)->mPath;

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
}

void cBuildSystem::saveBuildInfo()
{
	cSerializer outSer(cSerializer::ST_SERIALIZE);
	mBuildInfo->serialize(&outSer);
	outSer.save(getBuildAssetsPath() + "/buildInfo");
}

CLOSE_O2_NAMESPACE