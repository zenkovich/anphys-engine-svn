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
	
	hlog("Converting image: %s", asset->mLocation.mPath.c_str());

	abImageAssetInfo* imgAsset = static_cast<abImageAssetInfo*>(asset);

	cSerializer serializer;
	serializer.serialize(imgAsset->mAtlasName, "atlas");

	string fullPath = mBuildSystem->getBuildedAssetsFolderPath() + "/" + extractExtension(asset->mLocation.mPath) + ".atl_img";
	serializer.save(fullPath, false);
}

UniqueType asAssetImageBuildingConvertor::getConvertingType() const
{
	return abImageAssetInfo::getStaticType();
}

CLOSE_O2_NAMESPACE