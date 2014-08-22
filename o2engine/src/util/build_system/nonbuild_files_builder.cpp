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
	cBuildSystem::AssetChangesInfo* changesInf = &mBuildSystem->mAssetsChangesInfo;

	string buildDataPath = mBuildSystem->getBuildAssetsPath();
	FOREACH(cBuildSystem::FilesMetaVec, changesInf->mRemovedFiles, metaIt)
	{
		getFileSystem().deleteFile(buildDataPath + (*metaIt)->mPath);
		mBuildSystem->mActiveBuildConfig->removeFile(*metaIt);
		mBuildSystem->mBuildInfo->removeFile(*metaIt);
	}
}

void cNonBuildFilesBuildStage::copyNewFiles()
{
	cBuildSystem::AssetChangesInfo* changesInf = &mBuildSystem->mAssetsChangesInfo;

	string buildDataPath = mBuildSystem->getBuildAssetsPath();
	string assetsPath = mBuildSystem->getAssetsPath();
	FOREACH(cBuildSystem::FilesMetaVec, changesInf->mNewFiles, metaIt)
	{
		getFileSystem().copyFile(assetsPath + (*metaIt)->mPath, buildDataPath + (*metaIt)->mPath);
		mBuildSystem->mActiveBuildConfig->addFile(*metaIt);
		mBuildSystem->mBuildInfo->addFile(*metaIt);
	}
}

void cNonBuildFilesBuildStage::moveFiles()
{
	cBuildSystem::AssetChangesInfo* changesInf = &mBuildSystem->mAssetsChangesInfo;

	string buildDataPath = mBuildSystem->getBuildAssetsPath();
	string assetsPath = mBuildSystem->getAssetsPath();
	FOREACH(cBuildSystem::FilesMetaVec, changesInf->mMovedFiles, metaIt)
	{
		cBuildSystem::FileMeta* oldFile = mBuildSystem->mBuildInfo->findFile((*metaIt)->mMetaId);
		getFileSystem().deleteFile(buildDataPath + oldFile->mPath);
		mBuildSystem->mBuildInfo->removeFile(oldFile);
		mBuildSystem->mActiveBuildConfig->removeFile(mBuildSystem->mActiveBuildConfig->findFile((*metaIt)->mMetaId));

		getFileSystem().copyFile(assetsPath + (*metaIt)->mPath, buildDataPath + (*metaIt)->mPath);
		mBuildSystem->mActiveBuildConfig->addFile(*metaIt);
		mBuildSystem->mBuildInfo->addFile(*metaIt);
	}
}

void cNonBuildFilesBuildStage::copyChangedFiles()
{
	cBuildSystem::AssetChangesInfo* changesInf = &mBuildSystem->mAssetsChangesInfo;

	string buildDataPath = mBuildSystem->getBuildAssetsPath();
	string assetsPath = mBuildSystem->getAssetsPath();
	FOREACH(cBuildSystem::FilesMetaVec, changesInf->mChangedFiles, metaIt)
	{
		getFileSystem().deleteFile(buildDataPath + (*metaIt)->mPath);
		getFileSystem().copyFile(assetsPath + (*metaIt)->mPath, buildDataPath + (*metaIt)->mPath);
		
		cBuildSystem::FileMeta* confMeta = mBuildSystem->mActiveBuildConfig->findFile((*metaIt)->mMetaId);
		cBuildSystem::FileMeta* infoMeta = mBuildSystem->mBuildInfo->findFile((*metaIt)->mMetaId);
		
		*confMeta = **metaIt;
		*infoMeta = **metaIt;
	}
}

CLOSE_O2_NAMESPACE