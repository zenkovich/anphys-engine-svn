#include "asset_building_convertor.h"

#include "assets/assets.h"

OPEN_O2_NAMESPACE

asAssetBuildingConvertor::asAssetBuildingConvertor():
	mAssets(NULL)
{
}

asAssetBuildingConvertor::asAssetBuildingConvertor(Assets* assets):
	mAssets(assets)
{
}

asAssetBuildingConvertor::~asAssetBuildingConvertor()
{
}

CLOSE_O2_NAMESPACE