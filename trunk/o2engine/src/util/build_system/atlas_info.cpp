#include "atlas_info.h"

OPEN_O2_NAMESPACE

SERIALIZE_METHOD_IMPL(cImageAtlasInfo)
{
	SERIALIZE_ID(mName, "name");
	SERIALIZE_ID(mMaxSize, "maxSize");

	return true;
}

cImageAtlasInfo::cImageAtlasInfo(cBuildInfo* buildInfo):
	mOwnerBuildInfo(buildInfo), mAttachedPath(NULL)
{
}

cImageAtlasInfo::~cImageAtlasInfo()
{
}

void cImageAtlasInfo::clear()
{
	FOREACH(BuildImageInfoArr, mImages, image)
	{
		(*image)->mAtlas = NULL;
		(*image)->mAtlasName = "";
	}


	mImages.clear();
}

void cImageAtlasInfo::refreshImagesList()
{
	clear();

	if (!mAttachedPath) 
		searchImagesAsNamedAtlas();
	else
		searchImagesAsAttachedPath();
}

void cImageAtlasInfo::addImage( cBuildImageInfo* image )
{
	if (image->mAtlas)
		image->mAtlas->removeImage(image);

	mImages.add(image);
	image->mAtlas = this;
	image->mAtlasName = mName;
}

void cImageAtlasInfo::removeImage( cBuildImageInfo* image )
{
	foreach(BuildImageInfoArr, mImages, img) 
	{
		if ((*img)->mLocation == image->mLocation)
		{
			image->mAtlas = NULL;
			image->mAtlasName = "";
			mImages.remove(img);
			return;
		}
	}
}

cBuildImageInfo* cImageAtlasInfo::getImage(const cFileLocation& location)
{
	FOREACH(BuildImageInfoArr, mImages, img) 
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

	FOREACH(BuildImageInfoArr, mImages, img)
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
		searchImagesAsAttachedPath();
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

void cImageAtlasInfo::searchImagesAsAttachedPath()
{
	clear();

	if (!mAttachedPath)
		return;

	searchImagesInPath(mAttachedPath);
}

void cImageAtlasInfo::searchImagesInPath(cBuildPathInfo* path)
{
	FOREACH(BuildFileInfoArr, path->mFiles, file) 
	{
		if ((*file)->mType == cBuildFileInfo::MT_IMAGE)
		{
			cBuildImageInfo* image = static_cast<cBuildImageInfo*>(*file);
			if (!image->getAtlas() || image->getAtlas()->mAttachedPath != NULL)
				addImage(image);
		}

		if ((*file)->mType == cBuildFileInfo::MT_FOLDER)
		{
			cBuildPathInfo* path = static_cast<cBuildPathInfo*>(*file);
			if (path->getAttachedAtlas() != this)
				searchImagesInPath(path);
		}
	}
}

void cImageAtlasInfo::searchImagesAsNamedAtlas()
{
	if (!mOwnerBuildInfo)
		return;

	FOREACH(BuildFileInfoArr, mOwnerBuildInfo->mFileInfos, file) 
	{
		if ((*file)->mType == cBuildFileInfo::MT_IMAGE)
		{
			cBuildImageInfo* image = static_cast<cBuildImageInfo*>(*file);

			if (image->mAtlasName == mName)
				addImage(image);
		}
	}
}

bool cImageAtlasInfo::operator==(const cImageAtlasInfo& atlas)
{
	if (mImages.count() != atlas.mImages.count())
		return false;

	foreach(BuildImageInfoArr, mImages, imgIt)
	{
		bool found = false;
		foreach_const(BuildImageInfoArr, mImages, atlImIt)
		{
			if (**imgIt == **atlImIt)
			{
				found = true;
				break;
			}
		}

		if (!found)
			return false;
	}

	return true;
}

bool cImageAtlasInfo::operator!=(const cImageAtlasInfo& atlas)
{
	return !(*this == atlas);
}

CLOSE_O2_NAMESPACE