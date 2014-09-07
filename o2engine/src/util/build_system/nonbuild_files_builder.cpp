#include "nonbuild_files_builder.h"

#include "build_config.h"

OPEN_O2_NAMESPACE

void cNonBuildFilesBuildStage::process()
{
	removeFiles();
	copyNewFiles();
	moveFiles();
	copyChangedFiles();
}

void cNonBuildFilesBuildStage::removeFiles()
{
/*	cBuildSystem::AssetChangesInfo* changesInf = &mBuildSystem->mAssetsChangesInfo;

	string buildDataPath = mBuildSystem->getBuildAssetsPath();
	FOREACH(cBuildSystem::FilesMetaVec, changesInf->mRemovedFiles, metaIt)
	{
		hlog("Delete file: %s", (*metaIt)->mLocation.mPath.c_str());

		getFileSystem().deleteFile(buildDataPath + (*metaIt)->mLocation.mPath);
		mBuildSystem->mActiveBuildConfig->removeFile(*metaIt);
		mBuildSystem->mBuildInfo->removeFile(*metaIt);

		changesInf->mProcessedFiles.push_back(*metaIt);
	}

	changesInf->mRemovedFiles.clear();*/
}

void cNonBuildFilesBuildStage::copyNewFiles()
{
	/*cBuildSystem::AssetChangesInfo* changesInf = &mBuildSystem->mAssetsChangesInfo;

	string buildDataPath = mBuildSystem->getBuildAssetsPath();
	string assetsPath = mBuildSystem->getAssetsPath();
	FOREACH(cBuildSystem::FilesMetaVec, changesInf->mNewFiles, metaIt)
	{
		hlog("New file: %s", (*metaIt)->mLocation.mPath.c_str());
		getFileSystem().copyFile(assetsPath + (*metaIt)->mLocation.mPath, buildDataPath + (*metaIt)->mLocation.mPath);
		mBuildSystem->mActiveBuildConfig->addFile((*metaIt)->clone());
		mBuildSystem->mBuildInfo->addFile((*metaIt)->clone());

		changesInf->mProcessedFiles.push_back(*metaIt);
	}

	changesInf->mNewFiles.clear();*/
}

void cNonBuildFilesBuildStage::moveFiles()
{
	/*cBuildSystem::AssetChangesInfo* changesInf = &mBuildSystem->mAssetsChangesInfo;

	string buildDataPath = mBuildSystem->getBuildAssetsPath();
	string assetsPath = mBuildSystem->getAssetsPath();
	FOREACH(cBuildSystem::FilesMetaVec, changesInf->mMovedFiles, metaIt)
	{
		cBuildSystem::cBuildFileInfo* oldFile = mBuildSystem->mBuildInfo->getFile((*metaIt)->mLocation.mId);
		
		hlog("Move file: %s -> %s", oldFile->mPath.c_str(), (*metaIt)->mLocation.mPath.c_str());

		getFileSystem().deleteFile(buildDataPath + oldFile->mLocation.mPath);
		mBuildSystem->mBuildInfo->removeFile(oldFile);
		mBuildSystem->mActiveBuildConfig->removeFile(mBuildSystem->mActiveBuildConfig->getFile((*metaIt)->mLocation.mId));

		getFileSystem().copyFile(assetsPath + (*metaIt)->mLocation.mPath, buildDataPath + (*metaIt)->mLocation.mPath);
		mBuildSystem->mActiveBuildConfig->addFile((*metaIt)->clone());
		mBuildSystem->mBuildInfo->addFile((*metaIt)->clone());

		changesInf->mProcessedFiles.push_back(*metaIt);
	}

	changesInf->mMovedFiles.clear();*/
}

void cNonBuildFilesBuildStage::copyChangedFiles()
{
	/*cBuildSystem::AssetChangesInfo* changesInf = &mBuildSystem->mAssetsChangesInfo;

	string buildDataPath = mBuildSystem->getBuildAssetsPath();
	string assetsPath = mBuildSystem->getAssetsPath();
	FOREACH(cBuildSystem::FilesMetaVec, changesInf->mChangedFiles, metaIt)
	{		
		hlog("Changed file: %s", (*metaIt)->mLocation.mPath.c_str());

		getFileSystem().deleteFile(buildDataPath + (*metaIt)->mLocation.mPath);
		getFileSystem().copyFile(assetsPath + (*metaIt)->mLocation.mPath, buildDataPath + (*metaIt)->mLocation.mPath);
		
		cBuildSystem::cBuildFileInfo* confMeta = mBuildSystem->mActiveBuildConfig->getFile((*metaIt)->mLocation.mId);
		cBuildSystem::cBuildFileInfo* infoMeta = mBuildSystem->mBuildInfo->getFile((*metaIt)->mLocation.mId);
		
		*confMeta = **metaIt;
		*infoMeta = **metaIt;

		changesInf->mProcessedFiles.push_back(*metaIt);
	}

	changesInf->mChangedFiles.clear();*/
}

CLOSE_O2_NAMESPACE