#ifndef ASSET_H
#define ASSET_H

#include "public.h"
#include "util\file_system\file_info.h"

OPEN_O2_NAMESPACE

class asAsset
{
protected:
	cFileLocation mLocation;
	char*         mData;
	uint32        mDataSize;

public:
	asAsset();
	asAsset(const string& location);
	asAsset(const cFileLocation& location);
	virtual ~asAsset();

	cFileLocation getLocation() const;
	string getPath() const;
	uint32 getFileId() const;

	char* getData() const;
	uint32 getDataSize() const;

	void setData(char* data, uint32 size);

	bool save();
	bool save(const string& path);

protected:
	virtual void saveData();
};

CLOSE_O2_NAMESPACE

#endif // ASSET_H
