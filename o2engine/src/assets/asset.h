#ifndef ASSET_H
#define ASSET_H

#include "public.h"
#include "util/file_system/file_info.h"
#include "util/ref_object.h"
#include "util/type_indexation.h"
#include "util/time_utils.h"
#include "util/serialize_util.h"

OPEN_O2_NAMESPACE

class asAsset
{
	friend class Assets;
	friend class asAssetRef;

	DEFINE_TYPE(asAsset);

protected:
	FileLocation mLocation;
	char*         mData;
	uint32        mDataSize;

public:
	asAsset();
	asAsset(const string& location);
	asAsset(const FileLocation& location);
	asAsset(const asAsset& asset);
	virtual ~asAsset();

	asAsset& operator=(const asAsset& asset);

	FileLocation getLocation() const;
	string getPath() const;
	uint32 getFileId() const;

	void save(const string& path, bool rebuildAssetsImmediately = true);

protected:
	virtual void saveData() = 0;
};

struct asAssetInfo: public Serializable
{
public:
	enum Type { TP_FILE, TP_IMAGE, TP_ATLAS, TP_FOLDER };

	FileLocation mLocation;
	Type          mType;
	WideTime      mWriteTime;

public:
	asAssetInfo();
	asAssetInfo(const FileLocation& location, Type type, const WideTime& writeTime);

	bool operator==(const asAssetInfo& other) const;

	SERIALIZBLE_METHODS(asAssetInfo);
};
typedef array<asAssetInfo> AssetsInfosArr;

CLOSE_O2_NAMESPACE

#endif // ASSET_H
