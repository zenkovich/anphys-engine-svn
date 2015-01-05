#include "asset_file_info.h"

#include "app/application.h"
#include "assets/assets.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(abAssetInfo);

SERIALIZE_METHOD_IMPL(abAssetInfo)
{
	SERIALIZE_ID(&mLocation, "location");
	SERIALIZE_ID(mWriteTime, "writeTime");

	return true;
}

asAssetConfig* abAssetInfo::getConfigsSample() const
{
	asAssetConfig* res = mnew asAssetConfig();
	res->mLocation = mLocation;
	return res;
}

bool abAssetInfo::isEquals(abAssetInfo* other)
{
	return mLocation.mId == other->mLocation.mId && mLocation.mPath == other->mLocation.mPath && 
		   mWriteTime == other->mWriteTime;
}

void abAssetInfo::copyFrom(const abAssetInfo* other)
{
	mLocation = other->mLocation;
	mWriteTime = other->mWriteTime;
}

abAssetInfo* abAssetInfo::clone() const
{
	abAssetInfo* res = mnew abAssetInfo();
	res->copyFrom(this);
	return res;
}

REGIST_TYPE(abImageAssetInfo);

SERIALIZE_INHERITED_METHOD_IMPL(abImageAssetInfo)
{
	SERIALIZE_ID(mAtlasName, "atlas");
	SERIALIZE_ID(mScale, "scale");

	return true;
}

void abImageAssetInfo::initFromConfigs(asAssetConfig* config)
{
	asImageConfig* imgConfig = dynamic_cast<asImageConfig*>(config);

	if (imgConfig == NULL)
	{
		logError("Failed to get image configs: %s. Incorrect config type!", mLocation.mPath);
		return;
	}

	mAtlasName = imgConfig->mAtlas;
	mScale = imgConfig->mScale;
}

asAssetConfig* abImageAssetInfo::getConfigsSample() const
{
	asImageConfig* res = mnew asImageConfig();
	res->mLocation = mLocation;
	res->mAtlas = mAtlasName;
	res->mScale = mScale;
	return res;
}

bool abImageAssetInfo::isEquals(abAssetInfo* other)
{
	if (other->getType() != getType())
		return false;

	abImageAssetInfo* imgOther = static_cast<abImageAssetInfo*>(other);

	return abAssetInfo::isEquals(other) && mAtlasName == imgOther->mAtlasName && mScale == imgOther->mScale;
}

void abImageAssetInfo::copyFrom(const abAssetInfo* other)
{
	abAssetInfo::copyFrom(other);

	if (other->getType() != getType())
		return;

	const abImageAssetInfo* imgOther = static_cast<const abImageAssetInfo*>(other);

	mAtlasName = imgOther->mAtlasName;
	mScale = imgOther->mScale;
}

abImageAssetInfo::abImageAssetInfo():
	mScale(1.0f), mAtlas(NULL)
{
}

abAssetInfo* abImageAssetInfo::clone() const
{
	abAssetInfo* res = mnew abImageAssetInfo();
	res->copyFrom(this);
	return res;
}


REGIST_TYPE(abAtlasAssetInfo);

abAtlasAssetInfo::abAtlasAssetInfo():
	mMaxSize(2048.0f, 2048.0f), mName("unnamed"), mAttachedToFolder(false), mAttachFolder(NULL)
{
}

void abAtlasAssetInfo::initFromConfigs(asAssetConfig* config)
{
	asAtlasConfig* atlConfig = dynamic_cast<asAtlasConfig*>(config);

	if (atlConfig == NULL)
	{
		logError("Failed to get atlas configs: %s. Incorrect config type!", mLocation.mPath);
		return;
	}

	mName = atlConfig->mName;
	mMaxSize = atlConfig->mMaxSize;
}

asAssetConfig* abAtlasAssetInfo::getConfigsSample() const
{
	asAtlasConfig* res = mnew asAtlasConfig();
	res->mLocation = mLocation;
	res->mName = mName;
	res->mMaxSize= mMaxSize;
	return res;
}

bool abAtlasAssetInfo::isEquals(abAssetInfo* other)
{
	if (other->getType() != getType())
		return false;

	abAtlasAssetInfo* atlOther = static_cast<abAtlasAssetInfo*>(other);

	if (!(abAssetInfo::isEquals(other) && mName == atlOther->mName && mMaxSize == atlOther->mMaxSize && 
		  mImages.count() == atlOther->mImages.count()))
		return false;

	foreach(abImageAssetsInfosArr, mImages, imgIt)
	{
		abImageAssetInfo* otherImg = NULL;
		foreach(abImageAssetsInfosArr, atlOther->mImages, imgIt2)
		{
			if ((*imgIt)->mLocation == (*imgIt2)->mLocation)
			{
				otherImg = *imgIt2;
				break;
			}
		}

		if (!otherImg)
			return false;

		if (!otherImg->isEquals(*imgIt))
			return false;
	}

	return true;
}

void abAtlasAssetInfo::copyFrom(const abAssetInfo* other)
{
	abAssetInfo::copyFrom(other);

	if (other->getType() != getType())
		return;

	const abAtlasAssetInfo* atlOther = static_cast<const abAtlasAssetInfo*>(other);

	mMaxSize = atlOther->mMaxSize;
	mName = atlOther->mName;
	mAttachedToFolder = atlOther->mAttachedToFolder;
	mAttachFolderLocation = atlOther->mAttachFolderLocation;
}

abAssetInfo* abAtlasAssetInfo::clone() const
{
	abAssetInfo* res = mnew abAtlasAssetInfo();
	res->copyFrom(this);
	return res;
}

SERIALIZE_INHERITED_METHOD_IMPL(abAtlasAssetInfo)
{
	SERIALIZE_ID(mName, "name");
	SERIALIZE_ID(mMaxSize, "maxSize");
	SERIALIZE_ID(mAttachedToFolder, "attachedToFolder");
	SERIALIZE_ID(&mAttachFolderLocation, "attachoFolderLocation");

	return true;
}


REGIST_TYPE(abFolderInfo);

abFolderInfo::abFolderInfo():
	mAttachedAtlas(NULL), mParentFolder(NULL)
{
}

abFolderInfo::~abFolderInfo()
{
	clear();
}

void abFolderInfo::clear()
{
	release_array(abAssetsInfosArr, mInsideAssets);
}

SERIALIZE_INHERITED_METHOD_IMPL(abFolderInfo)
{
	SERIALIZE_ID(mInsideAssets, "insideAssets");

	return true;
}

asAssetConfig* abFolderInfo::initFromConfigs() const
{
	asPathConfig* res = mnew asPathConfig();
	res->mLocation = mLocation;
	return res;
}

abAssetInfo* abFolderInfo::getInsideAsset(const cFileLocation& location, bool recursive /*= false*/)
{
	if (location == cFileLocation())
		return this;

	foreach(abAssetsInfosArr, mInsideAssets, assetIt)
	{
		if ((*assetIt)->mLocation == location)
			return *assetIt;
	}

	if (!recursive)
		return NULL;

	
	foreach(abAssetsInfosArr, mInsideAssets, assetIt)
	{
		if ((*assetIt)->getType() == abFolderInfo::getStaticType())
		{
			abAssetInfo* res = static_cast<abFolderInfo*>(*assetIt)->getInsideAsset(location, true);
			if (res)
				return res;
		}
	}

	return NULL;
}

abAssetInfo* abFolderInfo::clone() const
{
	abAssetInfo* res = mnew abFolderInfo();
	res->copyFrom(this);
	return res;
}

void abFolderInfo::addInsideAsset(abAssetInfo* asset)
{
	if (asset->getType() == abFolderInfo::getStaticType())
		static_cast<abFolderInfo*>(asset)->mParentFolder = this;

	mInsideAssets.add(asset);
}

abAssetsInfosArr abFolderInfo::getAllInsideAssets() const
{
	abAssetsInfosArr res;
	foreach_const(abAssetsInfosArr, mInsideAssets, assetIt)
	{
		res.add(*assetIt);
		if ((*assetIt)->getType() == abFolderInfo::getStaticType())
			res.add((static_cast<abFolderInfo*>(*assetIt))->getAllInsideAssets());
	}

	return res;
}

void abFolderInfo::linkAtlases()
{
	linkChildFolders(NULL);

	abAtlasAssetsInfosArr allAtlases;
	abImageAssetsInfosArr allImages;
	abAssetsInfosArr insideAssets = getAllInsideAssets();

	mAttachedAtlas = NULL;

	foreach(abAssetsInfosArr, insideAssets, assetIt)
	{
		if ((*assetIt)->getType() == abAtlasAssetInfo::getStaticType())
		{
			abAtlasAssetInfo* atlas = static_cast<abAtlasAssetInfo*>(*assetIt);
			atlas->mAttachFolder = NULL;
			allAtlases.add(atlas);
		}

		if ((*assetIt)->getType() == abImageAssetInfo::getStaticType())
		{
			abImageAssetInfo* image = static_cast<abImageAssetInfo*>(*assetIt);
			image->mAtlas = NULL;
			allImages.add(image);
		}

		if ((*assetIt)->getType() == abFolderInfo::getStaticType())
		{
			abFolderInfo* folder = static_cast<abFolderInfo*>(*assetIt);
			folder->mAttachedAtlas = NULL;
		}
	}

	foreach(abAtlasAssetsInfosArr, allAtlases, atlasIt)
	{
		if ((*atlasIt)->mAttachedToFolder)
		{
			abFolderInfo* folder = static_cast<abFolderInfo*>(getInsideAsset((*atlasIt)->mAttachFolderLocation, true));
			(*atlasIt)->mAttachFolder = folder;
			folder->mAttachedAtlas = *atlasIt;
		}
	}

	foreach(abImageAssetsInfosArr, allImages, imageIt)
	{
		if (!(*imageIt)->mAtlasName.empty())
		{
			foreach(abAtlasAssetsInfosArr, allAtlases, atlasIt)
			{
				if ((*atlasIt)->mName == (*imageIt)->mAtlasName)
				{
					(*imageIt)->mAtlas = *atlasIt;
					break;
				}
			}
		}
	}

	linkImages();
}

void abFolderInfo::linkImages()
{
	abAtlasAssetInfo* upAtlas = NULL;
	abFolderInfo* upAtlasFolder = this;
	while(upAtlas == NULL && upAtlasFolder != NULL)
	{
		if (upAtlasFolder->mAttachedAtlas)
		{
			upAtlas = upAtlasFolder->mAttachedAtlas;
			break;
		}

		upAtlasFolder = upAtlasFolder->mParentFolder;
	}

	if (!upAtlas)
		return;

	foreach(abAssetsInfosArr, mInsideAssets, assetIt)
	{
		if ((*assetIt)->getType() == abImageAssetInfo::getStaticType())
		{
			abImageAssetInfo* image = static_cast<abImageAssetInfo*>(*assetIt);
			if (image->mAtlasName.empty())
			{
				image->mAtlas = upAtlas;
				upAtlas->mImages.add(image);
			}
		}

		if ((*assetIt)->getType() == abFolderInfo::getStaticType())
			static_cast<abFolderInfo*>(*assetIt)->linkImages();
	}
}

void abFolderInfo::linkChildFolders( abFolderInfo* parentFolder )
{
	mParentFolder = parentFolder;

	foreach(abAssetsInfosArr, mInsideAssets, assetIt)
	{
		if ((*assetIt)->getType() == abFolderInfo::getStaticType())
			static_cast<abFolderInfo*>(*assetIt)->linkChildFolders(this);
	}
}

CLOSE_O2_NAMESPACE