#include "nonbuild_files_builder.h"

#include "build_config.h"

OPEN_O2_NAMESPACE

void cNonBuildFilesBuildStage::process()
{
	mAssetsPath = mBuildSystem->getAssetsPath();
	mDataPath = mBuildSystem->getBuildAssetsPath();

	cBuildInfo* buildInfo = mBuildSystem->mBuildInfo;
	cBuildConfig* buildConfig = mBuildSystem->mActiveBuildConfig;

	StringsVec removingPaths;

	//search removed and changed files
	foreach(BuildFileInfoArr, buildInfo->mFileInfos, fileInfIt) 
	{
		bool removed = true;
		foreach(BuildFileInfoArr, buildConfig->mFileInfos, fileConfIt)
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
			if ((*fileInfIt)->mType == cBuildFileInfo::MT_FOLDER)
				removingPaths.push_back(mDataPath + (*fileInfIt)->mLocation.mPath);

			removeFile(*fileInfIt);
			safe_release(*fileInfIt);
			*fileInfIt = NULL;
		}
	}

	FOREACH(StringsVec, removingPaths, pathIt)
		getFileSystem().removeDirectory((*pathIt));

	foreach_rem(BuildFileInfoArr, buildInfo->mFileInfos, it)
		if (*it == NULL)
			buildInfo->mFileInfos.remove(it);
		else ++it;

	//search new files
	foreach(BuildFileInfoArr, buildConfig->mFileInfos, fileConfIt)
	{
		bool newFile = true;
		foreach(BuildFileInfoArr, buildInfo->mFileInfos, fileInfIt)
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