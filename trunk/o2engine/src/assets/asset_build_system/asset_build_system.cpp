#include "asset_build_system.h"

#include "assets/assets.h"
#include "asset_building_convertor.h"
#include "app/application.h"
#include "util/file_system/file_system.h"

OPEN_O2_NAMESPACE

AssetBuildSystem::AssetBuildSystem(Assets* assets):
	mAssets(assets)
{

}

AssetBuildSystem::~AssetBuildSystem()
{
	release_array(AssetsConfigsArr, mAssetsConfigs);
	release_array(AsConvertersArr, mAssetConverters);
}

void AssetBuildSystem::rebuildAssets(bool forcible /*= false*/)
{
	string assetsPath = "../../../assets";

	loadAssetsConfigs();
	mAssetsPathInfo = fileSystem()->getPathInfo(assetsPath);

	checkRemovedFiles();
	checkNewFiles();
	convertFiles();
}

void AssetBuildSystem::loadAssetsConfigs()
{
	string assetsConfigsFilePath = "../config.xml";

	release_array(AssetsConfigsArr, mAssetsConfigs);

	cSerializer serializer;
	if (serializer.load(assetsConfigsFilePath))
	{
		serializer.serialize(mAssetsConfigs, "assetsConfigs");
	}
}

void AssetBuildSystem::checkRemovedFiles()
{
	foreach(AssetsConfigsArr, mAssetsConfigs, confIt)
	{
		if (mAssetsPathInfo.isFileExist((*confIt)->mLocation.mPath))
	}
}

void AssetBuildSystem::checkNewFiles()
{

}

void AssetBuildSystem::convertFiles()
{

}

CLOSE_O2_NAMESPACE