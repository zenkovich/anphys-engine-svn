#ifndef ASSET_FILE_BUILDING_CONVERTOR_H
#define ASSET_FILE_BUILDING_CONVERTOR_H

#include "asset_building_convertor.h"

OPEN_O2_NAMESPACE
	
class asAssetFileBuildingConvertor: public asAssetBuildingConvertor
{
public:
	asAssetFileBuildingConvertor();
	asAssetFileBuildingConvertor(AssetBuildSystem* buildSystem);
	virtual ~asAssetFileBuildingConvertor();

	virtual void convert(abAssetInfo* asset);
	virtual UniqueType getConvertingType() const;
};

CLOSE_O2_NAMESPACE

#endif // ASSET_FILE_BUILDING_CONVERTOR_H
