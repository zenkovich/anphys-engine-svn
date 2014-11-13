#ifndef ASSET_BUILD_SYSTEM_H
#define ASSET_BUILD_SYSTEM_H

#include "public.h"
#include "util/file_system/file_info.h"

OPEN_O2_NAMESPACE

class Assets;
class asAssetBuildingConvertor;
class asAssetConfig;

class AssetBuildSystem
{
	friend class Assets;

	typedef array<asAssetConfig*> AssetsConfigsArr;
	typedef array<asAssetBuildingConvertor*> AsConvertersArr;

protected:
	Assets*          mAssets;
	AssetsConfigsArr mAssetsConfigs;
	AsConvertersArr  mAssetConverters;
	cPathInfo        mAssetsPathInfo;

protected:
	AssetBuildSystem(Assets* assets);
	~AssetBuildSystem();

	void rebuildAssets(bool forcible = false);

protected:
	void loadAssetsConfigs();
	void getAssetsFolderInfo();
	void checkRemovedFiles();
	void checkNewFiles();
	void convertFiles();
};

CLOSE_O2_NAMESPACE

#endif // ASSET_BUILD_SYSTEM_H
