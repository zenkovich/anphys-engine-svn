#ifndef ASSET_IMAGE_BUILDING_CONVERTOR_H
#define ASSET_IMAGE_BUILDING_CONVERTOR_H

#include "asset_building_convertor.h"

OPEN_O2_NAMESPACE
	
class asAssetImageBuildingConvertor: public asAssetBuildingConvertor
{
public:
	asAssetImageBuildingConvertor();
	asAssetImageBuildingConvertor(AssetBuildSystem* buildSystem);
	virtual ~asAssetImageBuildingConvertor();

	virtual void convert(abAssetInfo* asset);
	virtual UniqueType getConvertingType() const;
};

CLOSE_O2_NAMESPACE

#endif // ASSET_IMAGE_BUILDING_CONVERTOR_H
