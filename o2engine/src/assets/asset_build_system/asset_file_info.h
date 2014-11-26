#ifndef ASSET_FILE_INFO_H
#define ASSET_FILE_INFO_H

#include "util/file_system/file_info.h"
#include "config/asset_config.h"
#include "util/type_indexation.h"

OPEN_O2_NAMESPACE

class abAssetInfo: public cSerializable
{
public:
	DEFINE_TYPE(abAssetInfo);

	cFileLocation mLocation;
	WideTime      mWriteTime;

	virtual void getConfigs(asAssetConfig* config) {}
	virtual asAssetConfig* getConfigs() const;
	virtual bool isEquals(abAssetInfo* other);
	virtual void copyFrom(const abAssetInfo* other);
	virtual abAssetInfo* clone() const;

	SERIALIZBLE_METHODS(abAssetInfo);

};
typedef array<abAssetInfo*> abAssetsInfosArr;

class abImageAssetInfo: public abAssetInfo
{
public:
	abImageAssetInfo();

	DEFINE_TYPE(abImageAssetInfo);

	string mAtlas;
	float  mScale;
	
	virtual void getConfigs(asAssetConfig* config);
	virtual asAssetConfig* getConfigs() const;
	virtual bool isEquals(abAssetInfo* other);
	virtual void copyFrom(const abAssetInfo* other);
	virtual abAssetInfo* clone() const;

	SERIALIZBLE_INHERITED_METHODS(abImageAssetInfo, abAssetInfo);
};

class abFolderInfo: public abAssetInfo
{
public: 
	DEFINE_TYPE(abFolderInfo);

	abAssetsInfosArr mInsideAssets;

public:
	abFolderInfo();
	~abFolderInfo();

	abAssetInfo* getInsideAsset(const cFileLocation& location);
	void addInsideAsset(abAssetInfo* asset);
	void clear();
	virtual asAssetConfig* getConfigs() const;
	virtual abAssetInfo* clone() const;

	SERIALIZBLE_INHERITED_METHODS(abFolderInfo, abAssetInfo);
};

CLOSE_O2_NAMESPACE

#endif // ASSET_FILE_INFO_H
