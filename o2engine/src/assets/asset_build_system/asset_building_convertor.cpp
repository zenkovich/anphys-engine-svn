#include "asset_building_convertor.h"

#include "assets/assets.h"

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

CLOSE_O2_NAMESPACE