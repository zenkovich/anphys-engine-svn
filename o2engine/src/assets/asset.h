#ifndef ASSET_H
#define ASSET_H

#include "public.h"
#include "util\file_system\file_info.h"
#include "util\ref_object.h"

OPEN_O2_NAMESPACE

class asAsset: public IRefObject
{
	friend class Assets;
	friend class asAssetRef;

protected:
	cFileLocation mLocation;
	char*         mData;
	uint32        mDataSize;

protected:
	asAsset();
	asAsset(const string& location);
	asAsset(const cFileLocation& location);
	asAsset(const asAsset& asset);
	virtual ~asAsset();

	asAsset& operator=(const asAsset& asset);

	cFileLocation getLocation() const;
	string getPath() const;
	uint32 getFileId() const;

	char* getData() const;
	uint32 getDataSize() const;

	void setData(char* data, uint32 size);

	void save(const string& path);

protected:
	virtual void saveData() {}

	void onZeroRefCount();
};

CLOSE_O2_NAMESPACE

#endif // ASSET_H
