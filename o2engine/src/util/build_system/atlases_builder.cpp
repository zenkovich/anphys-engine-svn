#include "atlases_builder.h"
#include "build_info.h"
#include "build_system.h"
#include "atlas_info.h"
#include "build_config.h"
#include "atlas_packer.h"

OPEN_O2_NAMESPACE
	
cAtlasesBuildingStage::cAtlasesBuildingStage(cBuildSystem* buildSystem):
	cBuildSystem::IBuildStage(buildSystem)
{
	mAtlasPacker = mnew cAtlasPacker(buildSystem);
}

cAtlasesBuildingStage::~cAtlasesBuildingStage()
{
	safe_release(mAtlasPacker);
}

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
	foreach_rem(AtlasesArr, buildInfo->mAtlases, infAtlIt)
	{
		bool removed = true;
		FOREACH(AtlasesArr, buildConfig->mAtlases, confAtlIt)
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
			infAtlIt = buildInfo->mAtlases.remove(infAtlIt);
		}
		else ++infAtlIt;
	}

	//search new atlases
	FOREACH(AtlasesArr, buildConfig->mAtlases, confAtlIt)
	{
		bool newAtlas = true;
		FOREACH(AtlasesArr, buildInfo->mAtlases, infAtlIt)
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
	FOREACH(AtlasesArr, mRebuildingAtlases, atlIt)
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
		FOREACH(BuildImageInfoArr, confAtlas->mImages, confImgIt)
		{
			cBuildImageInfo* image = static_cast<cBuildImageInfo*>(buildInfo->getFile((*confImgIt)->mLocation));
			infoAtlas->addImage( image );
		}
	}

	mRebuildingAtlases.add(infoAtlas);
}

void cAtlasesBuildingStage::rebuildAtlas(cImageAtlasInfo* atlas)
{
	mAtlasPacker->packAtlas(atlas);
}


CLOSE_O2_NAMESPACE
