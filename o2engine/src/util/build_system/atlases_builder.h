#ifndef ATLASES_BUILDER_H
#define ATLASES_BUILDER_H

#include "build_system.h"

OPEN_O2_NAMESPACE

class cImageAtlasInfo;
class cAtlasPacker;

class cAtlasesBuildingStage: cBuildSystem::IBuildStage
{
	friend class cBuildSystem;

	typedef cBuildInfo::AtlasesArr AtlasesArr;

	AtlasesArr    mRebuildingAtlases;
	cAtlasPacker* mAtlasPacker;

public:
	cAtlasesBuildingStage(cBuildSystem* buildSystem);
	~cAtlasesBuildingStage();

	void process();

protected:
	void updateAtlases();
	void rebuildAtlases();
	void updateAtlas(cImageAtlasInfo* confAtlas, cImageAtlasInfo* infoAtlas);
	void rebuildAtlas(cImageAtlasInfo* atlas);
};

CLOSE_O2_NAMESPACE

#endif // ATLASES_BUILDER_H
