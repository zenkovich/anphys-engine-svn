#ifndef NONBUILD_FILES_BUILDER_H
#define NONBUILD_FILES_BUILDER_H

#include "build_system.h"

OPEN_O2_NAMESPACE

struct cNonBuildFilesBuildStage: cBuildSystem::IBuildStage
{
	cNonBuildFilesBuildStage(cBuildSystem* buildSystem):cBuildSystem::IBuildStage(buildSystem) {}

	void process();
};

CLOSE_O2_NAMESPACE

#endif //NONBUILD_FILES_BUILDER_H
