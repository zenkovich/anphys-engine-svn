#include "asset_atlas_building_convertor.h"

#include "asset_build_system.h"
#include "app/application.h"
#include "util/file_system/file_system.h"
#include "util/image/image.h"

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
	abAtlasAssetInfo* atlas = static_cast<abAtlasAssetInfo*>(asset);
	string buildedPath = mBuildSystem->getBuildedAssetsFolderPath() + extractExtension(asset->mLocation.mPath);

	mImageDefs.clear();
	mRectsPacker.clear();

	foreach(abImageAssetsInfosArr, atlas->mImages, imgIt)
	{
		cImage* image = mnew cImage(mBuildSystem->getAssetsFolderPath() + extractExtension((*imgIt)->mLocation.mPath));
		cRectsPacker::rect* packRect = mRectsPacker.addRect(image->getSize());

		mImageDefs.add(imageDef(*imgIt, image, packRect));
	}

	mRectsPacker.setMaxSize(atlas->mMaxSize);
	mRectsPacker.pack();

	cImage resAtlasImage(cImage::FMT_R8G8B8A8, atlas->mMaxSize);

	foreach(ImageDefsArr, mImageDefs, imgDefIt)
	{
		resAtlasImage.copyImage(imgDefIt->mImage, imgDefIt->mPackRect->mRect.getltCorner());
		safe_release(imgDefIt->mImage);
	}

	resAtlasImage.save(buildedPath + ".png", cImage::IT_PNG);

	cSerializer serializer;
	serializer.serialize(mImageDefs, "images");
	serializer.save(buildedPath);
}

void asAssetAtlasBuildingConvertor::remove(abAssetInfo* asset)
{
	hlog("Removing atlas: %s", asset->mLocation.mPath.c_str());
	string buildedPath = mBuildSystem->getBuildedAssetsFolderPath() + "/" + asset->mLocation.mPath;

	fileSystem()->deleteFile(buildedPath + ".xml");
	fileSystem()->deleteFile(extractExtension(buildedPath) + ".png");
}

UniqueType asAssetAtlasBuildingConvertor::getConvertingType() const
{
	return abAtlasAssetInfo::getStaticType();
}


asAssetAtlasBuildingConvertor::imageDef::imageDef( abImageAssetInfo* imageAssetInfo /*= NULL*/, cImage* image /*= NULL*/, 
	                                               cRectsPacker::rect* packRect /*= NULL*/ ):
	mImageAssetInfo(imageAssetInfo), mImage(image), mPackRect(packRect)
{
}

bool asAssetAtlasBuildingConvertor::imageDef::operator==( const imageDef& other )
{
	return mImage == other.mImage && mPackRect == other.mPackRect && mImageAssetInfo == other.mImageAssetInfo;
}

SERIALIZE_METHOD_IMPL(asAssetAtlasBuildingConvertor::imageDef)
{
	SERIALIZE_ID(&mImageAssetInfo->mLocation, "location");
	SERIALIZE_ID(mPackRect->mRect, "rect");

	return true;
}

CLOSE_O2_NAMESPACE