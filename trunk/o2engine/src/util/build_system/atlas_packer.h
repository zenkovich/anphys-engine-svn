#ifndef ATLAS_PACHER_H
#define ATLAS_PACHER_H

#include "public.h"
#include "util/math/rects_packer.h"

OPEN_O2_NAMESPACE

class cBuildSystem;
struct cBuildImageInfo;
class cImageAtlasInfo;

class cAtlasPacker
{
protected:
	struct imageContainer
	{
		cRectsPacker::rect* mRect;
		cBuildImageInfo*    mImageInfo;

		imageContainer():mRect(NULL), mImageInfo(NULL) {}
		imageContainer(cRectsPacker::rect* rect, cBuildImageInfo* imageInfo):mRect(rect), mImageInfo(imageInfo) {}

		bool operator==(const imageContainer& cc) { return cc.mRect == mRect && cc.mImageInfo == mImageInfo; }
	};

	cBuildSystem*         mBuildSystem;
	cRectsPacker          mRectsPacker;
	array<imageContainer> mImages;

public:
	cAtlasPacker(cBuildSystem* buildSystem);
	~cAtlasPacker();

	void packAtlas(cImageAtlasInfo* atlas);
};

CLOSE_O2_NAMESPACE


#endif //ATLAS_PACHER_H