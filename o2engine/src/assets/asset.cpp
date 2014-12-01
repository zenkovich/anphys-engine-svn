#include "asset.h"

#include "app/application.h"
#include "assets.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(asAsset);

asAsset::asAsset():
	mData(NULL), mDataSize(0)
{
}

asAsset::asAsset(const string& location):
	mData(NULL), mDataSize(0)
{
	mLocation = assets()->getAssetFileLocation(location);
}

asAsset::asAsset(const cFileLocation& location):
	mData(NULL), mDataSize(0)
{
	mLocation = location;
}

asAsset::asAsset(const asAsset& asset):
	mData(NULL), mDataSize(0), mLocation()
{
}

asAsset::~asAsset()
{
}

asAsset& asAsset::operator=(const asAsset& asset)
{
	return *this;
}

cFileLocation asAsset::getLocation() const
{
	return mLocation;
}

string asAsset::getPath() const
{
	return mLocation.mPath;
}

uint32 asAsset::getFileId() const
{
	return mLocation.mId;
}

void asAsset::save(const string& path)
{
	if (mLocation.mPath == path)
		return;

	mLocation.mPath = path;
	mLocation.mId = assets()->generateFileId();
}

void asAsset::onZeroRefCount()
{
	assets()->assetUnused(this);
}


asAssetInfo::asAssetInfo()
{
}

asAssetInfo::asAssetInfo(const cFileLocation& location, const string& typeName, const WideTime& writeTime):
	mLocation(location), mTypeName(typeName), mWriteTime(writeTime)
{
}

SERIALIZE_METHOD_IMPL(asAssetInfo)
{
	SERIALIZE_ID(&mLocation, "location");
	SERIALIZE_ID(mTypeName, "type");
	SERIALIZE_ID(mWriteTime, "writeTime");

	return true;
}

bool asAssetInfo::operator==(const asAssetInfo& other) const
{
	return mLocation == other.mLocation && mWriteTime == other.mWriteTime;
}

CLOSE_O2_NAMESPACE