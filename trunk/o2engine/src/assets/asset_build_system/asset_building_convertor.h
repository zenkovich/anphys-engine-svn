#ifndef ASSET_BUILDING_CONVERTOR_H
#define ASSET_BUILDING_CONVERTOR_H

#include "public.h"
#include "util/file_system/file_info.h"
#include "util/type_indexation.h"

OPEN_O2_NAMESPACE

class Assets;

struct asAssetConfig
{
	cFileLocation mLocation;
	bool          mIncludeBuild;
};

class asAssetBuildingConvertor
{
	Assets* mAssets;

public:
	asAssetBuildingConvertor();
	asAssetBuildingConvertor(Assets* assets);
	virtual ~asAssetBuildingConvertor();

	virtual void convert(asAssetConfig* assetCfg) = 0;
	virtual UniqueType getConvertingType() const = 0;
};

CLOSE_O2_NAMESPACE

#endif // ASSET_BUILDING_CONVERTOR_H
