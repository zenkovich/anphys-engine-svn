#ifndef ASSET_ATLAS_BUILDING_CONVERTOR_H
#define ASSET_ATLAS_BUILDING_CONVERTOR_H

#include "asset_building_convertor.h"
#include "util/math/rects_packer.h"

OPEN_O2_NAMESPACE

class abImageAssetInfo;
class cImage;

class asAssetAtlasBuildingConvertor: public asAssetBuildingConvertor
{
public:
	struct imageDef: public cSerializable
	{
		abImageAssetInfo*   mImageAssetInfo;
		cImage*             mImage;
		cRectsPacker::rect* mPackRect;

		imageDef(abImageAssetInfo* imageAssetInfo = NULL, cImage* image = NULL, cRectsPacker::rect* packRect = NULL);

		bool operator==(const imageDef& other);

		SERIALIZBLE_METHODS(imageDef);
	};

private:
	typedef array<abImageAssetInfo*> ImageAssetsInfosArr;
	typedef array<imageDef>          ImageDefsArr;

	ImageDefsArr mImageDefs;
	cRectsPacker mRectsPacker;

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
