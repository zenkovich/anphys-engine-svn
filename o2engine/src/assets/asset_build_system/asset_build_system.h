#ifndef ASSET_BUILD_SYSTEM_H
#define ASSET_BUILD_SYSTEM_H

#include "public.h"
#include "util/singleton.h"

OPEN_O2_NAMESPACE

class ProjectBuildConfig;

class AssetBuildSystem: public cSingleton<AssetBuildSystem>
{
public:
	bool rebuildAssets(ProjectBuildConfig* buildConfig, bool forcible = false);
};

CLOSE_O2_NAMESPACE

#endif // ASSET_BUILD_SYSTEM_H
