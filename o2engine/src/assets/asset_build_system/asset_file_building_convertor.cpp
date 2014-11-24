#include "asset_file_building_convertor.h"

#include "asset_build_system.h"
#include "app/application.h"
#include "util/file_system/file_system.h"

OPEN_O2_NAMESPACE

asAssetFileBuildingConvertor::asAssetFileBuildingConvertor():
	asAssetBuildingConvertor()
{
}

asAssetFileBuildingConvertor::asAssetFileBuildingConvertor(AssetBuildSystem* buildSystem):
	asAssetBuildingConvertor(buildSystem)
{
}

asAssetFileBuildingConvertor::~asAssetFileBuildingConvertor()
{
}

void asAssetFileBuildingConvertor::convert(abAssetInfo* asset)
{
	fileSystem()->copyFile(mBuildSystem->getAssetsFolderPath() + asset->mLocation.mPath,
		                   mBuildSystem->getBuildedAssetsFolderPath() + asset->mLocation.mPath);
}

UniqueType asAssetFileBuildingConvertor::getConvertingType() const
{
	return abAssetInfo::getStaticType();
}

CLOSE_O2_NAMESPACE