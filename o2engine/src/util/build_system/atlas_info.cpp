#include "atlas_info.h"

OPEN_O2_NAMESPACE

SERIALIZE_METHOD_IMPL(cImageAtlasInfo)
{
	SERIALIZE_ID(mName, "name");
	SERIALIZE_ID(mMaxSize, "maxSize");
	SERIALIZE_ID(mImages, "images");

	return true;
}

cImageAtlasInfo::cImageAtlasInfo()
{
}

cImageAtlasInfo::~cImageAtlasInfo()
{
	RELEASE_VECTOR(ImagesMetaVec, mImages);
}

void cImageAtlasInfo::addImage( cBuildSystem::ImageFileMeta* meta )
{
	mImages.push_back(meta);
}

void cImageAtlasInfo::removeImage( cBuildSystem::ImageFileMeta* meta )
{
	FOREACH(ImagesMetaVec, mImages, img) 
	{
		if ((*img)->mPath == meta->mPath)
		{
			safe_release(*img);
			mImages.erase(img);
			return;
		}
	}
}

CLOSE_O2_NAMESPACE