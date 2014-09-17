#ifndef NONBUILD_FILES_BUILDER_H
#define NONBUILD_FILES_BUILDER_H

#include "build_system.h"

OPEN_O2_NAMESPACE

class cNonBuildFilesBuildStage: public cBuildSystem::IBuildStage
{
	string mAssetsPath;
	string mDataPath;

public:
	cNonBuildFilesBuildStage(cBuildSystem* buildSystem):cBuildSystem::IBuildStage(buildSystem) {}

	void process();

protected:
	void removeFile(cBuildFileInfo* fileInfo);
	void changeFile(cBuildFileInfo* oldFileInfo, cBuildFileInfo* newFileInfo);
	void copyFile(cBuildFileInfo* fileInfo);
};

CLOSE_O2_NAMESPACE

#endif //NONBUILD_FILES_BUILDER_H
