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

asAsset::asAsset(const FileLocation& location):
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

FileLocation asAsset::getLocation() const
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

	saveData();

	assets()->rebuildAssets();
}


asAssetInfo::asAssetInfo():
	mType(TP_FILE)
{
}

asAssetInfo::asAssetInfo(const FileLocation& location, Type type, const WideTime& writeTime):
	mLocation(location), mType(type), mWriteTime(writeTime)
{
}

SERIALIZE_METHOD_IMPL(asAssetInfo)
{
	SERIALIZE_ID(&mLocation, "location");

	int type = (int)mType;
	SERIALIZE_ID(type, "type");
	mType = (Type)type;

	SERIALIZE_ID(mWriteTime, "writeTime");

	return true;
}

bool asAssetInfo::operator==(const asAssetInfo& other) const
{
	return mLocation == other.mLocation && mWriteTime == other.mWriteTime && mType == other.mType;
}

CLOSE_O2_NAMESPACE