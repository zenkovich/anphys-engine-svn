#include "atlases_builder.h"

OPEN_O2_NAMESPACE

void cAtlasesBuildingStage::process()
{
	updateAtlases();
	getChangedAtlases();
	rebuildChangedAtlases();
}

void cAtlasesBuildingStage::updateAtlases()
{	
	cBuildSystem::AssetChangesInfo* changesInf = &mBuildSystem->mAssetsChangesInfo;
}

void cAtlasesBuildingStage::getChangedAtlases()
{

}

void cAtlasesBuildingStage::rebuildChangedAtlases()
{

}

CLOSE_O2_NAMESPACE
