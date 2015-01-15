#include "image_asset.h"

#include "util/image/bitmap.h"
#include "app/application.h"
#include "assets.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(asImage);

asImage::asImage():
	asAsset(), mImage(NULL)
{
}

asImage::asImage(const string& location):
	asAsset(location), mImage(NULL)
{
	loadData();
}

asImage::asImage(const FileLocation& location):
	asAsset(location), mImage(NULL)
{
	loadData();
}

asImage::asImage(Bitmap* image):
	asAsset(), mImage(image)
{
}

asImage::~asImage()
{
	safe_release(mImage);
}

Bitmap* asImage::getImage()
{
	if (mImage ==  NULL)
		loadImage();

	return mImage;
}

void asImage::setImage(Bitmap* image)
{
	safe_release(mImage);
	mImage = image;
}

asImage& asImage::operator=(const asImage& asset)
{
	setImage(asset.mImage);
	return *this;
}

void asImage::loadData()
{
	Serializer serializer;
	if (!serializer.load(assets()->getAssetFullPath(mLocation.mPath) + ".atl_img", false))
	{
		assets()->mLog->error("Failed to load image asset: %s", mLocation.mPath.c_str());
		return;
	}

	serializer.serialize(mAtlas, "atlas");
}

void asImage::loadImage()
{
	safe_release(mImage);
	mImage = mnew Bitmap(assets()->getAssetSourceFullPath(mLocation.mPath));
}

void asImage::saveData()
{
	if (mImage)
		mImage->save(assets()->getAssetSourceFullPath(mLocation.mPath), Bitmap::IT_PNG);

	asAssetConfig* config = assets()->mAssetsConfigs.getAssetConfig(mLocation);
	if (!config)
}

void asImage::setAtlasName( const string& atlasName )
{
	mAtlas = atlasName;
}

SERIALIZE_METHOD_IMPL(asImage)
{
	SERIALIZE_ID(mAtlas, "atlas")

	return true;
}

CLOSE_O2_NAMESPACE