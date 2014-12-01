#ifndef ASSET_ATLAS_BUILDING_CONVERTOR_H
#define ASSET_ATLAS_BUILDING_CONVERTOR_H

#include "asset_building_convertor.h"

OPEN_O2_NAMESPACE

class asAssetAtlasBuildingConvertor: public asAssetBuildingConvertor
{
public:
	asAssetAtlasBuildingConvertor();
	asAssetAtlasBuildingConvertor(AssetBuildSystem* buildSystem);
	virtual ~asAssetAtlasBuildingConvertor();

	virtual void convert(abAssetInfo* asset);
	virtual void remove(abAssetInfo* asset);
	virtual UniqueType getConvertingType() const;
};

CLOSE_O2_NAMESPACE

#endif // ASSET_ATLAS_BUILDING_CONVERTOR_H
