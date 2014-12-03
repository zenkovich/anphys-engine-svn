#include "image_asset.h"

#include "util/image/image.h"
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

asImage::asImage(const cFileLocation& location):
	asAsset(location), mImage(NULL)
{
	loadData();
}

asImage::asImage(cImage* image):
	asAsset(), mImage(image)
{
}

asImage::~asImage()
{
	safe_release(mImage);
}

cImage* asImage::getImage()
{
	if (mImage ==  NULL)
		loadImage();

	return mImage;
}

void asImage::setImage(cImage* image)
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
	cSerializer serializer;
	if (!serializer.load(assets()->getAssetFullPath(mLocation.mPath) + ".atl_img", false))
	{
		assets()->mLog->error("Failed to load image asset: %s", mLocation.mPath.c_str());
		return;
	}

	serializer.serialize(mAtlas, "atlas");
}

void asImage::loadImage()
{
	mImage = mnew cImage(assets()->getAssetSourceFullPath(mLocation.mPath));
}

void asImage::saveData()
{
	mImage->save(assets()->getAssetSourceFullPath(mLocation.mPath), cImage::IT_PNG);
}

SERIALIZE_METHOD_IMPL(asImage)
{
	SERIALIZE_ID(mAtlas, "atlas")

	return true;
}

CLOSE_O2_NAMESPACE