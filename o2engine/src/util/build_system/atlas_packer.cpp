#include "atlas_packer.h"

#include "build_system.h"
#include "build_info.h"
#include "atlas_info.h"
#include "util/image/image.h"

OPEN_O2_NAMESPACE

cAtlasPacker::cAtlasPacker(cBuildSystem* buildSystem):
	mBuildSystem(buildSystem)
{
}

cAtlasPacker::~cAtlasPacker()
{
}

void cAtlasPacker::packAtlas(cImageAtlasInfo* atlas)
{
	mRectsPacker.clear();
	mRectsPacker.setMaxSize(atlas->mMaxSize);

	FOREACH(BuildImageInfoVec, atlas->mImages, imgIt)
	{
		cImage* image = mnew cImage( mBuildSystem->getAssetsPath() + extractExtension((*imgIt)->mLocation.mPath) );
		(*imgIt)->mSize = image->getSize();

		mImages.add(imageContainer(mRectsPacker.addRect(image->getSize()), *imgIt, image));
	}

	mRectsPacker.pack();

	foreach(ImagesArr, mImages, img)
		safe_release(img->mImage);

	mImages.clear();
}


cAtlasPacker::imageContainer::imageContainer():
	mRect(NULL), mImageInfo(NULL), mImage(NULL)
{
}

cAtlasPacker::imageContainer::imageContainer(cRectsPacker::rect* rect, cBuildImageInfo* imageInfo, cImage* image):
	mRect(rect), mImageInfo(imageInfo), mImage(image)
{
}

bool cAtlasPacker::imageContainer::operator==(const imageContainer& cc)
{
	return cc.mRect == mRect && cc.mImageInfo == mImageInfo;
}

CLOSE_O2_NAMESPACE