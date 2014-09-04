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
	meta->mAtlas = mName;
	mImages.push_back(meta);
}

void cImageAtlasInfo::removeImage( cBuildSystem::ImageFileMeta* meta )
{
	FOREACH(ImagesMetaVec, mImages, img) 
	{
		if ((*img)->mLocation == meta->mLocation)
		{
			safe_release(*img);
			mImages.erase(img);
			return;
		}
	}
}

cBuildSystem::ImageFileMeta* cImageAtlasInfo::getImage(const cFileLocation& location)
{
	FOREACH(ImagesMetaVec, mImages, img) 
	{
		if ((*img)->mLocation == location)
		{
			return *img;
		}
	}

	return NULL;
}

void cImageAtlasInfo::setName(const string& name)
{
	mName = name;

	FOREACH(ImagesMetaVec, mImages, img)
		(*img)->mAtlas = mName;
}

string cImageAtlasInfo::getName() const
{
	return mName;
}

void cImageAtlasInfo::attachPath(const cFileLocation& pathLocation)
{

}

void cImageAtlasInfo::unattachPath()
{

}

CLOSE_O2_NAMESPACE