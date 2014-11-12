#ifndef FILE_ASSET_H
#define FILE_ASSET_H

#include "asset.h"

OPEN_O2_NAMESPACE

class asFile: public asAsset
{
	DEFINE_TYPE(asFile);

protected:
	char*  mData;
	uint32 mDataSize;

protected:
	asFile();
	asFile(const string& location);
	asFile(const cFileLocation& location);
	asFile(const asFile& asset);
	virtual ~asFile();

	asFile& operator=(const asFile& asset);

	char* getData() const;
	uint32 getDataSize() const;

	void setData(char* data, uint32 size);

protected:
	void loadData();
	void saveData();
};


CLOSE_O2_NAMESPACE

#endif // FILE_ASSET_H
