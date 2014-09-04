#include "atlases_builder.h"
#include "build_info.h"
#include "build_system.h"
#include "atlas_info.h"
#include "build_config.h"

OPEN_O2_NAMESPACE

void cAtlasesBuildingStage::process()
{
	updateAtlases();
	getChangedAtlases();
	rebuildChangedAtlases();
}

bool atlasesSortFunc(cImageAtlasInfo* atlas1, cImageAtlasInfo* atlas2) 
{
	int length1 = atlas1->mAttachedPath.length();
	int length2 = atlas2->mAttachedPath.length();

	if (length1 == 0)
		length1 = INT_MAX;

	if (length2 == 0)
		length2 = INT_MAX;

	return length1 < length2;
}

void cAtlasesBuildingStage::updateAtlases()
{	
	cBuildInfo::AtlasesVec sortedAtlases;
	FOREACH(cBuildInfo::AtlasesVec, mBuildSystem->mActiveBuildConfig->mAtlases, atlas)
		sortedAtlases.push_back(*atlas);

	std::sort(sortedAtlases.begin(), sortedAtlases.end(), atlasesSortFunc);

	sortedAtlases.push_back(mBuildSystem->mActiveBuildConfig->mBasicAtlas);

	FOREACH(cBuildInfo::AtlasesVec, sortedAtlases, atlas) 
		updateAtlas(*atlas);
}

void cAtlasesBuildingStage::getChangedAtlases()
{

}

void cAtlasesBuildingStage::rebuildChangedAtlases()
{

}

void cAtlasesBuildingStage::updateAtlas(cImageAtlasInfo* atlas)
{
	bool worksByPath = atlas->mAttachedPath.length() > 0;
	cBuildSystem::AssetChangesInfo* changesInf = &mBuildSystem->mAssetsChangesInfo;

	//process new files
	for(cBuildSystem::FilesMetaVec::iterator metaIt = changesInf->mNewFiles.begin(); metaIt != changesInf->mNewFiles.end();)
	{
		if ((*metaIt)->mType != cBuildSystem::FileMeta::MT_IMAGE)
		{
			metaIt++;
			continue;
		}

		cBuildSystem::ImageFileMeta* imageMeta = static_cast<cBuildSystem::ImageFileMeta*>(*metaIt);

		if (worksByPath)
		{
			if ((imageMeta->mAtlas != "" && imageMeta->mAtlas != atlas->mName) || 
				!isPathInsideOtherPath(atlas->mAttachedPath, imageMeta->mLocation.mPath))
			{
				metaIt++;
				continue;
			}
		}
		else
		{
			if (imageMeta->mAtlas != atlas->mName)
			{
				metaIt++;
				continue;
			}
		}
			
		atlas->addImage( static_cast<cBuildSystem::ImageFileMeta*>(imageMeta->clone()) );
		changesInf->mProcessedFiles.push_back(*metaIt);
		metaIt = changesInf->mNewFiles.erase(metaIt);
	}

	//process removed files
	for(cBuildSystem::FilesMetaVec::iterator metaIt = changesInf->mRemovedFiles.begin(); metaIt != changesInf->mRemovedFiles.end();)
	{
		if ((*metaIt)->mType != cBuildSystem::FileMeta::MT_IMAGE)
		{
			metaIt++;
			continue;
		}

		cBuildSystem::ImageFileMeta* imageMeta = static_cast<cBuildSystem::ImageFileMeta*>(*metaIt);

		if (atlas->getImage(imageMeta->mLocation) == NULL)
		{
			metaIt++;
			continue;
		}

		atlas->removeImage(imageMeta);
		changesInf->mProcessedFiles.push_back(*metaIt);
		metaIt = changesInf->mRemovedFiles.erase(metaIt);
	}

	//process changed files	
	for(cBuildSystem::FilesMetaVec::iterator metaIt = changesInf->mChangedFiles.begin(); metaIt != changesInf->mChangedFiles.end();)
	{
		if ((*metaIt)->mType != cBuildSystem::FileMeta::MT_IMAGE)
		{
			metaIt++;
			continue;
		}

		cBuildSystem::ImageFileMeta* imageMeta = static_cast<cBuildSystem::ImageFileMeta*>(*metaIt);
		cBuildSystem::ImageFileMeta* imageMetaInAtlas = atlas->getImage(imageMeta->mLocation);
		if (imageMetaInAtlas == NULL)
		{
			metaIt++;
			continue;
		}

		*imageMetaInAtlas = *imageMeta;
		changesInf->mProcessedFiles.push_back(*metaIt);
		metaIt = changesInf->mChangedFiles.erase(metaIt);
	}

	//process moved files
	for(cBuildSystem::FilesMetaVec::iterator metaIt = changesInf->mMovedFiles.begin(); metaIt != changesInf->mMovedFiles.end();)
	{
		if ((*metaIt)->mType != cBuildSystem::FileMeta::MT_IMAGE)
		{
			metaIt++;
			continue;
		}

		cBuildSystem::ImageFileMeta* imageMeta = static_cast<cBuildSystem::ImageFileMeta*>(*metaIt);

		if (worksByPath)
		{
			if ((imageMeta->mAtlas != "" && imageMeta->mAtlas != atlas->mName) || 
				!isPathInsideOtherPath(atlas->mAttachedPath, imageMeta->mLocation.mPath))
			{
				metaIt++;
				continue;
			}
		}
		else
		{
			if (imageMeta->mAtlas != atlas->mName)
			{
				metaIt++;
				continue;
			}
		}
			
		atlas->addImage( static_cast<cBuildSystem::ImageFileMeta*>(imageMeta->clone()) );
		changesInf->mProcessedFiles.push_back(*metaIt);
		metaIt = changesInf->mMovedFiles.erase(metaIt);
	}
}

CLOSE_O2_NAMESPACE
