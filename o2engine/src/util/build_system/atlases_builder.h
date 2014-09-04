#ifndef ATLASES_BUILDER_H
#define ATLASES_BUILDER_H

#include "build_system.h"

OPEN_O2_NAMESPACE

class cImageAtlasInfo;

struct cAtlasesBuildingStage: cBuildSystem::IBuildStage
{
	cAtlasesBuildingStage(cBuildSystem* buildSystem):cBuildSystem::IBuildStage(buildSystem) {}

	void process();

	void updateAtlases();
	void getChangedAtlases();
	void rebuildChangedAtlases();

	void updateAtlas(cImageAtlasInfo* atlas);
};

CLOSE_O2_NAMESPACE

#endif // ATLASES_BUILDER_H
