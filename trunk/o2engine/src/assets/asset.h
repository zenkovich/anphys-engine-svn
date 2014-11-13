#ifndef ASSET_H
#define ASSET_H

#include "public.h"
#include "util/file_system/file_info.h"
#include "util/ref_object.h"
#include "util/type_indexation.h"
#include "util/time_utils.h"
#include "util/serialize_util.h"

OPEN_O2_NAMESPACE

class asAsset: public IRefObject
{
	friend class Assets;
	friend class asAssetRef;

	DEFINE_TYPE(asAsset);

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

	void save(const string& path);

protected:
	virtual void saveData() = 0;

	void onZeroRefCount();
};

struct asAssetInfo: public cSerializable
{
	cFileLocation mLocation;
	string        mTypeName;
	WideTime      mWriteTime;

	SERIALIZBLE_METHODS(asAssetInfo);
};

CLOSE_O2_NAMESPACE

#endif // ASSET_H
