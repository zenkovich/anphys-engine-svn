#ifndef ASSET_BUILDING_CONVERTOR_H
#define ASSET_BUILDING_CONVERTOR_H

#include "public.h"
#include "util/file_system/file_info.h"
#include "util/type_indexation.h"
#include "asset_file_info.h"

OPEN_O2_NAMESPACE

class AssetBuildSystem;

class asAssetBuildingConvertor
{
protected:
	AssetBuildSystem* mBuildSystem;

public:
	asAssetBuildingConvertor();
	asAssetBuildingConvertor(AssetBuildSystem* buildSystem);
	virtual ~asAssetBuildingConvertor();

	virtual void convert(abAssetInfo* asset) = 0;
	virtual void remove(abAssetInfo* asset);
	virtual UniqueType getConvertingType() const = 0;
};

CLOSE_O2_NAMESPACE

#endif // ASSET_BUILDING_CONVERTOR_H
