#include "asset_atlas_building_convertor.h"

#include "asset_build_system.h"
#include "app/application.h"
#include "util/file_system/file_system.h"

OPEN_O2_NAMESPACE

asAssetAtlasBuildingConvertor::asAssetAtlasBuildingConvertor():
	asAssetBuildingConvertor()
{
}

asAssetAtlasBuildingConvertor::asAssetAtlasBuildingConvertor(AssetBuildSystem* buildSystem):
	asAssetBuildingConvertor(buildSystem)
{
}

asAssetAtlasBuildingConvertor::~asAssetAtlasBuildingConvertor()
{
}

void asAssetAtlasBuildingConvertor::convert(abAssetInfo* asset)
{
}

void asAssetAtlasBuildingConvertor::remove(abAssetInfo* asset)
{
	hlog("Removing atlas: %s", asset->mLocation.mPath.c_str());
	string buildedPath = mBuildSystem->getBuildedAssetsFolderPath() + "/" + asset->mLocation.mPath;

	fileSystem()->deleteFile(buildedPath);
	fileSystem()->deleteFile(extractExtension(buildedPath) + ".png");
}

UniqueType asAssetAtlasBuildingConvertor::getConvertingType() const
{
	return abAtlasAssetInfo::getStaticType();
}

CLOSE_O2_NAMESPACE