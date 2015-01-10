#include "file_asset.h"

#include "app/application.h"
#include "util/file_system/file.h"
#include "assets.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(asFile);

asFile::asFile():
	asAsset(), mData(NULL), mDataSize(0)
{
}

asFile::asFile(const string& location):
	asAsset(location), mData(NULL), mDataSize(0)
{
	loadData();
}

asFile::asFile(const FileLocation& location):
	asAsset(location), mData(NULL), mDataSize(0)
{
	loadData();
}

asFile::asFile(const asFile& asset):
	asAsset(), mData(NULL), mDataSize(0)
{
	if (asset.mData == NULL)
		return;

	mDataSize = asset.mDataSize;
	mData = mnew char[mDataSize];
	memcpy(mData, asset.mData, mDataSize);
}

asFile::~asFile()
{
	safe_release(mData);
}

asFile& asFile::operator=(const asFile& asset)
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

char* asFile::getData() const
{
	return mData;
}

uint32 asFile::getDataSize() const
{
	return mDataSize;
}

void asFile::setData(char* data, uint32 size)
{
	safe_release(mData);
	mData = mnew char[size];
	memcpy(mData, data, size);
	mDataSize = size;
}

void asFile::saveData()
{
	OutFile file(assets()->getAssetSourceFullPath(mLocation.mPath));
	file.writeData(mData, mDataSize);
	file.close();
}

void asFile::loadData()
{
	InFile file(assets()->getAssetFullPath(mLocation.mPath));

	if (!file.isOpened())
	{
		assets()->mLog->error("Failed to load asset: %s, can't open file.", mLocation.mPath.c_str());
		return;
	}

	mDataSize = file.getDataSize();
	mData = mnew char[mDataSize];
	file.readData(mData, mDataSize);
}

CLOSE_O2_NAMESPACE