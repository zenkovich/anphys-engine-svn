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

asAssetConfig* abAssetInfo::getConfigs() const
{
	asAssetConfig* res = mnew asAssetConfig();
	res->mLocation = mLocation;
	return res;
}

bool abAssetInfo::isEquals(abAssetInfo* other)
{
	return mLocation == other->mLocation && mWriteTime == other->mWriteTime;
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
	SERIALIZE_ID(mAtlas, "atlas");
	SERIALIZE_ID(mScale, "scale");

	return true;
}

void abImageAssetInfo::getConfigs(asAssetConfig* config)
{
	asImageConfig* imgConfig = dynamic_cast<asImageConfig*>(config);

	if (imgConfig == NULL)
	{
		logError("Failed to get image configs: %s. Incorrect config type!", mLocation.mPath);
		return;
	}

	mAtlas = imgConfig->mAtlas;
	mScale = imgConfig->mScale;
}

asAssetConfig* abImageAssetInfo::getConfigs() const
{
	asImageConfig* res = mnew asImageConfig();
	res->mLocation = mLocation;
	res->mAtlas = mAtlas;
	res->mScale = mScale;
	return res;
}

bool abImageAssetInfo::isEquals(abAssetInfo* other)
{
	if (other->getType() != getType())
		return false;

	abImageAssetInfo* imgOther = static_cast<abImageAssetInfo*>(other);

	return abAssetInfo::isEquals(other) && mAtlas == imgOther->mAtlas && mScale == imgOther->mScale;
}

void abImageAssetInfo::copyFrom(const abAssetInfo* other)
{
	abAssetInfo::copyFrom(other);

	if (other->getType() != getType())
		return;

	const abImageAssetInfo* imgOther = static_cast<const abImageAssetInfo*>(other);

	mAtlas = imgOther->mAtlas;
	mScale = imgOther->mScale;
}

abImageAssetInfo::abImageAssetInfo():
	mScale(1.0f)
{
}

abAssetInfo* abImageAssetInfo::clone() const
{
	abAssetInfo* res = mnew abImageAssetInfo();
	res->copyFrom(this);
	return res;
}


REGIST_TYPE(abFolderInfo);

abFolderInfo::abFolderInfo()
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

asAssetConfig* abFolderInfo::getConfigs() const
{
	asPathConfig* res = mnew asPathConfig();
	res->mLocation = mLocation;
	return res;
}

abAssetInfo* abFolderInfo::getInsideAsset(const cFileLocation& location)
{
	foreach(abAssetsInfosArr, mInsideAssets, assetIt)
	{
		if ((*assetIt)->mLocation == location)
			return *assetIt;
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
	mInsideAssets.add(asset);
}


CLOSE_O2_NAMESPACE