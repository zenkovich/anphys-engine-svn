#ifndef ASSET_ATLAS_BUILDING_CONVERTOR_H
#define ASSET_ATLAS_BUILDING_CONVERTOR_H

#include "asset_building_convertor.h"
#include "util/math/rects_packer.h"

OPEN_O2_NAMESPACE

class abImageAssetInfo;
class Bitmap;

class asAssetAtlasBuildingConvertor: public asAssetBuildingConvertor
{
public:
	struct imageDef: public Serializable
	{
		abImageAssetInfo*  mImageAssetInfo;
		Bitmap*            mImage;
		RectsPacker::rect* mPackRect;

		imageDef(abImageAssetInfo* imageAssetInfo = NULL, Bitmap* image = NULL, RectsPacker::rect* packRect = NULL);

		bool operator==(const imageDef& other);

		SERIALIZBLE_METHODS(imageDef);
	};

private:
	typedef array<abImageAssetInfo*> ImageAssetsInfosArr;
	typedef array<imageDef>          ImageDefsArr;

	ImageDefsArr mImageDefs;
	RectsPacker  mRectsPacker;

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
