#include "asset.h"

#include "app/application.h"
#include "assets.h"
#include "util/file_system/file.h"

OPEN_O2_NAMESPACE

asAsset::asAsset():
	mData(NULL), mDataSize(0)
{
}

asAsset::asAsset(const string& location):
	mData(NULL), mDataSize(0)
{
	mLocation = assets()->getAssetFileLocation(location);

	cInFile* file = mnew cInFile(mLocation.mPath);

	if (!file->isOpened())
	{
		assets()->mLog->error("Failed to load asset: %s, can't open file.", location.c_str());
		return;
	}

	mDataSize = file->getDataSize();
	mData = mnew char[mDataSize];
	file->readData(mData, mDataSize);

	safe_release(file);
}

asAsset::asAsset(const cFileLocation& location):
	mData(NULL), mDataSize(0)
{
	mLocation = location;

	cInFile* file = mnew cInFile(mLocation.mPath);

	if (!file->isOpened())
	{
		assets()->mLog->error("Failed to load asset: %s, can't open file.", mLocation.mPath.c_str());
		return;
	}

	mDataSize = file->getDataSize();
	mData = mnew char[mDataSize];
	file->readData(mData, mDataSize);

	safe_release(file);
}

asAsset::asAsset(const asAsset& asset):
	mData(NULL), mDataSize(0)
{
	if (asset.mData == NULL)
		return;

	mDataSize = asset.mDataSize;
	mData = mnew char[mDataSize];
	memcpy(mData, asset.mData, mDataSize);
}

asAsset::~asAsset()
{
	safe_release(mData);
}

asAsset& asAsset::operator=(const asAsset& asset)
{
	safe_release(mData);
	mDataSize = 0;

	if (asset.mData == NULL)
		return *this;

	mDataSize = asset.mDataSize;
	mData = mnew char[mDataSize];
	memcpy(mData, asset.mData, mDataSize);

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

char* asAsset::getData() const
{
	return mData;
}

uint32 asAsset::getDataSize() const
{
	return mDataSize;
}

void asAsset::setData(char* data, uint32 size)
{
	safe_release(mData);
	mData = mnew char[size];
	memcpy(mData, data, size);
	mDataSize = size;
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

CLOSE_O2_NAMESPACE