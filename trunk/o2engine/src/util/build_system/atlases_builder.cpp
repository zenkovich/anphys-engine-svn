#include "atlases_builder.h"
#include "build_info.h"
#include "build_system.h"
#include "atlas_info.h"
#include "build_config.h"

OPEN_O2_NAMESPACE

void cAtlasesBuildingStage::process()
{
	updateAtlases();
	rebuildAtlases();
}

void cAtlasesBuildingStage::updateAtlases()
{
	cBuildConfig* buildConfig = mBuildSystem->mActiveBuildConfig;
	cBuildInfo* buildInfo = mBuildSystem->mBuildInfo;

	//search removed or changed atlases atlases
	for(AtlasesVec::iterator infAtlIt = buildInfo->mAtlases.begin(); infAtlIt != buildInfo->mAtlases.end();)
	{
		bool removed = true;
		FOREACH(AtlasesVec, buildConfig->mAtlases, confAtlIt)
		{
			if ((*infAtlIt)->mName == (*confAtlIt)->mName)
			{
				removed = false;
				if (**infAtlIt != **confAtlIt)
					updateAtlas(*confAtlIt, *infAtlIt);
			}
		}

		if (removed)
		{
			safe_release(*infAtlIt);
			infAtlIt = buildInfo->mAtlases.erase(infAtlIt);
		}
		else infAtlIt++;
	}

	//search new atlases
	FOREACH(AtlasesVec, buildConfig->mAtlases, confAtlIt)
	{
		bool newAtlas = true;
		FOREACH(AtlasesVec, buildInfo->mAtlases, infAtlIt)
		{
			if ((*infAtlIt)->mName == (*confAtlIt)->mName)
			{
				newAtlas = false;
				break;
			}
		}

		if (newAtlas)
		{
			cImageAtlasInfo* newAtlas = buildInfo->addAtlas((*confAtlIt)->mName, (*confAtlIt)->mMaxSize);
			updateAtlas(*confAtlIt, newAtlas);
		}
	}

	//check basic atlas
	if (*(buildConfig->mBasicAtlas) != *(buildInfo->mBasicAtlas))
		updateAtlas(buildConfig->mBasicAtlas, buildInfo->mBasicAtlas);
}

void cAtlasesBuildingStage::rebuildAtlases()
{
	FOREACH(AtlasesVec, mRebuildingAtlases, atlIt)
		rebuildAtlas(*atlIt);
}

void cAtlasesBuildingStage::updateAtlas(cImageAtlasInfo* confAtlas, cImageAtlasInfo* infoAtlas)
{
	cBuildConfig* buildConfig = mBuildSystem->mActiveBuildConfig;
	cBuildInfo* buildInfo = mBuildSystem->mBuildInfo;

	if (confAtlas->mAttachedPath)
	{
		cBuildPathInfo* path = static_cast<cBuildPathInfo*>(buildInfo->getFile(confAtlas->mAttachedPathLoc));
		infoAtlas->unattachPath();
		infoAtlas->attachPath( path );
	}
	else
	{
		infoAtlas->clear();
		FOREACH(BuildImageInfoVec, confAtlas->mImages, confImgIt)
		{
			cBuildImageInfo* image = static_cast<cBuildImageInfo*>(buildInfo->getFile((*confImgIt)->mLocation));
			infoAtlas->addImage( image );
		}
	}

	mRebuildingAtlases.push_back(infoAtlas);
}

void cAtlasesBuildingStage::rebuildAtlas(cImageAtlasInfo* atlas)
{

}


CLOSE_O2_NAMESPACE
