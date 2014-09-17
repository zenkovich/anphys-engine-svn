#include "nonbuild_files_builder.h"

#include "build_config.h"

OPEN_O2_NAMESPACE

void cNonBuildFilesBuildStage::process()
{
	mAssetsPath = mBuildSystem->getAssetsPath();
	mDataPath = mBuildSystem->getBuildAssetsPath();

	cBuildInfo* buildInfo = mBuildSystem->mBuildInfo;
	cBuildConfig* buildConfig = mBuildSystem->mActiveBuildConfig;

	//search removed and changed files
	FOREACH(BuildFileInfoVec, buildInfo->mFileInfos, fileInfIt) 
	{
		bool removed = true;
		FOREACH(BuildFileInfoVec, buildConfig->mFileInfos, fileConfIt)
		{
			if ((*fileInfIt)->mLocation == (*fileConfIt)->mLocation)
			{
				removed = false;

				if (**fileInfIt != **fileConfIt)
					changeFile(*fileInfIt, *fileConfIt);
			}
		}

		if (removed)
		{
			removeFile(*fileInfIt);
			safe_release(*fileInfIt);
			*fileInfIt = NULL;
		}
	}

	for (BuildFileInfoVec::iterator it = buildInfo->mFileInfos.begin(); it != buildInfo->mFileInfos.end();)
	{
		if (*it == NULL)
			it = buildInfo->mFileInfos.erase(it);
		else
			++it;
	}

	//search new files
	FOREACH(BuildFileInfoVec, buildConfig->mFileInfos, fileConfIt)
	{
		bool newFile = true;
		FOREACH(BuildFileInfoVec, buildInfo->mFileInfos, fileInfIt)
		{
			if ((*fileInfIt)->mLocation == (*fileConfIt)->mLocation)
				newFile = false;
		}

		if (newFile)
		{
			copyFile(*fileConfIt);
			buildInfo->addFile((*fileConfIt)->clone());
		}
	}
}

void cNonBuildFilesBuildStage::removeFile(cBuildFileInfo* fileInfo)
{
	getFileSystem().deleteFile(mDataPath + fileInfo->mLocation.mPath);
	hlog("Deleted file: %s", fileInfo->mLocation.mPath.c_str());
}

void cNonBuildFilesBuildStage::changeFile(cBuildFileInfo* oldFileInfo, cBuildFileInfo* newFileInfo)
{
	getFileSystem().deleteFile(mDataPath + oldFileInfo->mLocation.mPath);
	getFileSystem().copyFile(mAssetsPath + newFileInfo->mLocation.mPath, mDataPath + newFileInfo->mLocation.mPath);
	hlog("Changed file: %s to %s", oldFileInfo->mLocation.mPath.c_str(), newFileInfo->mLocation.mPath.c_str());
}

void cNonBuildFilesBuildStage::copyFile(cBuildFileInfo* fileInfo)
{
	getFileSystem().copyFile(mAssetsPath + fileInfo->mLocation.mPath, mDataPath + fileInfo->mLocation.mPath);
	hlog("Copied file: %s", fileInfo->mLocation.mPath.c_str());
}

CLOSE_O2_NAMESPACE