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
}

void cImageAtlasInfo::clear()
{
	FOREACH(BuildImageInfoVec, mImages, image)
		(*image)->setAtlas(NULL);

	mImages.clear();
}

void cImageAtlasInfo::refreshImages()
{
	if (!mAttachedPath)
		return;

	clear();
	gatherImagesFromAttachedPath();
}

void cImageAtlasInfo::addImage( cBuildImageInfo* image )
{
	if (image->mAtlas)
		image->mAtlas->removeImage(image);

	mImages.push_back(image);
	image->mAtlas = this;
	image->mAtlasName = mName;
}

void cImageAtlasInfo::removeImage( cBuildImageInfo* image )
{
	FOREACH(BuildImageInfoVec, mImages, img) 
	{
		if ((*img)->mLocation == image->mLocation)
		{
			mImages.erase(img);
			return;
		}
	}
}

cBuildImageInfo* cImageAtlasInfo::getImage(const cFileLocation& location)
{
	FOREACH(BuildImageInfoVec, mImages, img) 
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

	FOREACH(BuildImageInfoVec, mImages, img)
		(*img)->mAtlasName = mName;
}

string cImageAtlasInfo::getName() const
{
	return mName;
}

void cImageAtlasInfo::attachPath(cBuildPathInfo* path)
{
	if (path == mAttachedPath)
		return;

	clear();

	if (mAttachedPath)
		mAttachedPath->attachAtlas(NULL);

	mAttachedPath = path;

	if (path != NULL) 
	{
		mAttachedPathLoc = path->mLocation;
		mAttachedPath->mAttachedAtlasName = mName;
		mAttachedPath->mAttachedAtlas = this;
		gatherImagesFromAttachedPath();
	}
	else 
	{
		mAttachedPathLoc = cFileLocation();
	}
}

void cImageAtlasInfo::unattachPath()
{
	attachPath(NULL);
}

void cImageAtlasInfo::gatherImagesFromAttachedPath()
{
	clear();

	if (!mAttachedPath)
		return;

	searchPathImages(mAttachedPath);
}

void cImageAtlasInfo::searchPathImages(cBuildPathInfo* path)
{
	FOREACH(BuildFileInfoVec, path->mFiles, file) 
	{
		if ((*file)->mType == cBuildFileInfo::MT_IMAGE)
		{
			cBuildImageInfo* image = static_cast<cBuildImageInfo*>(*file);
			if (!image->getAtlas())
				addImage(image);
		}

		if ((*file)->mType == cBuildFileInfo::MT_FOLDER)
		{
			cBuildPathInfo* path = static_cast<cBuildPathInfo*>(*file);
			if (!path->getAttachedAtlas())
				searchPathImages(path);
		}
	}
}

CLOSE_O2_NAMESPACE