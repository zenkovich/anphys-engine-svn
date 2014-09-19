#ifndef ATLASES_BUILDER_H
#define ATLASES_BUILDER_H

#include "build_system.h"

OPEN_O2_NAMESPACE

class cImageAtlasInfo;

class cAtlasesBuildingStage: cBuildSystem::IBuildStage
{
	friend class cBuildSystem;

	typedef cBuildInfo::AtlasesVec AtlasesVec;

	AtlasesVec mRebuildingAtlases;

public:
	cAtlasesBuildingStage(cBuildSystem* buildSystem):cBuildSystem::IBuildStage(buildSystem) {}

	void process();

protected:
	void updateAtlases();
	void rebuildAtlases();
	void updateAtlas(cImageAtlasInfo* confAtlas, cImageAtlasInfo* infoAtlas);
	void rebuildAtlas(cImageAtlasInfo* atlas);
};

CLOSE_O2_NAMESPACE

#endif // ATLASES_BUILDER_H
