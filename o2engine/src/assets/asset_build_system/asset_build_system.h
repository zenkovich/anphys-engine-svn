#ifndef ASSET_BUILD_SYSTEM_H
#define ASSET_BUILD_SYSTEM_H

#include "public.h"

OPEN_O2_NAMESPACE

class Assets;

class AssetBuildSystem
{
	Assets* mAssets;

public:
	bool rebuildAssets(bool forcible = false);
};

CLOSE_O2_NAMESPACE

#endif // ASSET_BUILD_SYSTEM_H
