#include "asset_folder_building_convertor.h"

#include "asset_build_system.h"
#include "app/application.h"
#include "util/file_system/file_system.h"

OPEN_O2_NAMESPACE

asAssetFolderBuildingConvertor::asAssetFolderBuildingConvertor():
	asAssetBuildingConvertor()
{
}

asAssetFolderBuildingConvertor::asAssetFolderBuildingConvertor(AssetBuildSystem* buildSystem):
	asAssetBuildingConvertor(buildSystem)
{
}

asAssetFolderBuildingConvertor::~asAssetFolderBuildingConvertor()
{
}

void asAssetFolderBuildingConvertor::convert(abAssetInfo* asset)
{
	fileSystem()->createDirectory(mBuildSystem->getBuildedAssetsFolderPath() + "/" + asset->mLocation.mPath);
}

UniqueType asAssetFolderBuildingConvertor::getConvertingType() const
{
	return abFolderInfo::getStaticType();
}

void asAssetFolderBuildingConvertor::remove(abAssetInfo* asset)
{
	fileSystem()->removeDirectory(mBuildSystem->getBuildedAssetsFolderPath() + "/" + asset->mLocation.mPath);
}

CLOSE_O2_NAMESPACE