#ifndef ASSET_BUILD_SYSTEM_H
#define ASSET_BUILD_SYSTEM_H

#include "public.h"
#include "util/file_system/file_info.h"
#include "asset_file_info.h"
#include "config/asset_config.h"

OPEN_O2_NAMESPACE

class Assets;
class asAssetBuildingConvertor;
class asAssetConfig;
class abAssetInfo;

class AssetBuildSystem
{
	friend class Assets;
	friend class asAssetFileBuildingConvertor;
	friend class asAssetFolderBuildingConvertor;
	friend class asAssetBuildingConvertor;
	friend class asAssetImageBuildingConvertor;

	typedef array<asAssetBuildingConvertor*> AsConvertersArr;

protected:
	Assets*         mAssets;
	AsConvertersArr mAssetConverters;
	abFolderInfo    mAssetsFolderInfo;
	abFolderInfo    mBuildedAssetsFolderInfo;
	string          mAssetsFolderPath;
	string          mBuildedAssetsFolderPath;
	string          mAssetsFolderConfigFilePath;
	string          mBuildedAssetsInfoFilePath;

protected:
	AssetBuildSystem(Assets* assets);
	~AssetBuildSystem();

	void rebuildAssets(bool forcible = false);

	string getAssetsFolderPath() const;
	string getBuildedAssetsFolderPath() const;

protected:
	void loadAssetFolderInfo();
	void loadBuildedAssetsFolderInfo();

	void checkRemovedFiles(abFolderInfo& assetFolderInfo, abFolderInfo& buildedAssetFolderInfo);
	void checkNewFiles(abFolderInfo& assetFolderInfo, abFolderInfo& buildedAssetFolderInfo);
	void convertFiles(abFolderInfo& assetFolderInfo, abFolderInfo& buildedAssetFolderInfo);
	void copyAndConvertAsset(abAssetInfo* assetInfo, abAssetInfo* buildAssetInfo);
	void removeConvertedAsset(abAssetInfo* buildAssetInfo);
	void saveBuildInfo();

	void processLoadingAssetsFolderInfo(cPathInfo& pathInfo, asPathConfig& pathConfig, abFolderInfo& asPathInfo);
	abAssetInfo* createAssetInfroFromFileInfo(const cFileInfo& fileInfo);

	uint32 tryGetAssetsInfoMetaId(cPathInfo &pathInfo, const string& path);

};

CLOSE_O2_NAMESPACE

#endif // ASSET_BUILD_SYSTEM_H
