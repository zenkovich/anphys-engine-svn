#include "atlas_packer.h"

#include "build_system.h"
#include "build_info.h"
#include "atlas_info.h"

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
		mImages.add(imageContainer(mRectsPacker.addRect((*imgIt)->mSize), *imgIt));

	mRectsPacker.pack();
}

CLOSE_O2_NAMESPACE