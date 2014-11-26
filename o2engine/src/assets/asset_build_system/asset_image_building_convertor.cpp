#include "asset_image_building_convertor.h"

#include "asset_build_system.h"
#include "app/application.h"
#include "util/file_system/file_system.h"

OPEN_O2_NAMESPACE

asAssetImageBuildingConvertor::asAssetImageBuildingConvertor():
	asAssetBuildingConvertor()
{
}

asAssetImageBuildingConvertor::asAssetImageBuildingConvertor(AssetBuildSystem* buildSystem):
	asAssetBuildingConvertor(buildSystem)
{
}

asAssetImageBuildingConvertor::~asAssetImageBuildingConvertor()
{
}

void asAssetImageBuildingConvertor::convert(abAssetInfo* asset)
{
	fileSystem()->copyFile(mBuildSystem->getAssetsFolderPath() + "/" + asset->mLocation.mPath,
		                   mBuildSystem->getBuildedAssetsFolderPath() + "/" + asset->mLocation.mPath);
}

UniqueType asAssetImageBuildingConvertor::getConvertingType() const
{
	return abImageAssetInfo::getStaticType();
}

CLOSE_O2_NAMESPACE