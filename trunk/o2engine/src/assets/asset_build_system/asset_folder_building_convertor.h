#ifndef ASSET_FOLDER_BUILDING_CONVERTOR_H
#define ASSET_FOLDER_BUILDING_CONVERTOR_H

#include "asset_building_convertor.h"

OPEN_O2_NAMESPACE
	
class asAssetFolderBuildingConvertor: public asAssetBuildingConvertor
{
public:
	asAssetFolderBuildingConvertor();
	asAssetFolderBuildingConvertor(AssetBuildSystem* buildSystem);
	virtual ~asAssetFolderBuildingConvertor();

	virtual void convert(abAssetInfo* asset);	
	virtual void remove(abAssetInfo* asset);
	virtual UniqueType getConvertingType() const;
};

CLOSE_O2_NAMESPACE

#endif // ASSET_FOLDER_BUILDING_CONVERTOR_H
