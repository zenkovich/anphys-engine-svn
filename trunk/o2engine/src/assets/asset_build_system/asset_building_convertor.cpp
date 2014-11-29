#include "asset_building_convertor.h"

#include "asset_build_system.h"
#include "assets/assets.h"
#include "app/application.h"
#include "util/file_system/file_system.h"

OPEN_O2_NAMESPACE

asAssetBuildingConvertor::asAssetBuildingConvertor():
	mBuildSystem(NULL)
{
}

asAssetBuildingConvertor::asAssetBuildingConvertor(AssetBuildSystem* buildSystem):
	mBuildSystem(buildSystem)
{
}

asAssetBuildingConvertor::~asAssetBuildingConvertor()
{
}

void asAssetBuildingConvertor::remove(abAssetInfo* asset)
{	
	hlog("Removing asset: %s", asset->mLocation.mPath.c_str());
	fileSystem()->deleteFile(mBuildSystem->getBuildedAssetsFolderPath() + "/" + asset->mLocation.mPath);
}

CLOSE_O2_NAMESPACE