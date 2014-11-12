#include "image_asset.h"

#include "util/image/image.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(asImage);

asImage::asImage():
	asAsset(), mImage(NULL)
{
}

asImage::asImage(const string& location):
	asAsset(location), mImage(NULL)
{
	mImage = mnew cImage(mLocation.mPath);
}

asImage::asImage(const cFileLocation& location):
	asAsset(location), mImage(NULL)
{
	mImage = mnew cImage(mLocation.mPath);
}

asImage::asImage(cImage* image):
	asAsset(), mImage(image)
{
}

asImage::~asImage()
{
	safe_release(mImage);
}

cImage* asImage::getImage() const
{
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

void asImage::saveData()
{
	mImage->save(mLocation.mPath, cImage::IT_PNG);
}

CLOSE_O2_NAMESPACE