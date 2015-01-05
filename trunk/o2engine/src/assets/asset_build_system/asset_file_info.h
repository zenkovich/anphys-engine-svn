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

	virtual void initFromConfigs(asAssetConfig* config) {}
	virtual asAssetConfig* getConfigsSample() const;
	virtual bool isEquals(abAssetInfo* other);
	virtual void copyFrom(const abAssetInfo* other);
	virtual abAssetInfo* clone() const;

	SERIALIZBLE_METHODS(abAssetInfo);

};
typedef array<abAssetInfo*> abAssetsInfosArr;

class abAtlasAssetInfo;
class abFolderInfo;

class abImageAssetInfo: public abAssetInfo
{
	friend class abFolderInfo;

	abAtlasAssetInfo* mAtlas;

public:
	abImageAssetInfo();

	DEFINE_TYPE(abImageAssetInfo);

	string mAtlasName;
	float  mScale;
	
	virtual void initFromConfigs(asAssetConfig* config);
	virtual asAssetConfig* getConfigsSample() const;
	virtual bool isEquals(abAssetInfo* other);
	virtual void copyFrom(const abAssetInfo* other);
	virtual abAssetInfo* clone() const;

	SERIALIZBLE_INHERITED_METHODS(abImageAssetInfo, abAssetInfo);
};
typedef array<abImageAssetInfo*> abImageAssetsInfosArr;

class abAtlasAssetInfo: public abAssetInfo
{
	friend class abFolderInfo;
	friend class AssetBuildSystem;
	friend class asAssetAtlasBuildingConvertor;

	abImageAssetsInfosArr mImages;
	bool                  mAttachedToFolder;
	cFileLocation         mAttachFolderLocation;
	abFolderInfo*         mAttachFolder;

public:
	abAtlasAssetInfo();

	DEFINE_TYPE(abAtlasAssetInfo);

	string mName;
	vec2f  mMaxSize;
	
	virtual void initFromConfigs(asAssetConfig* config);
	virtual asAssetConfig* getConfigsSample() const;
	virtual bool isEquals(abAssetInfo* other);
	virtual void copyFrom(const abAssetInfo* other);
	virtual abAssetInfo* clone() const;

	SERIALIZBLE_INHERITED_METHODS(abAtlasAssetInfo, abAssetInfo);
};
typedef array<abAtlasAssetInfo*> abAtlasAssetsInfosArr;

class abFolderInfo: public abAssetInfo
{
	friend class AssetBuildSystem;

public: 
	DEFINE_TYPE(abFolderInfo);

	abFolderInfo*     mParentFolder;
	abAssetsInfosArr  mInsideAssets;
	abAtlasAssetInfo* mAttachedAtlas;

public:
	abFolderInfo();
	~abFolderInfo();

	abAssetsInfosArr getAllInsideAssets() const;
	abAssetInfo* getInsideAsset(const cFileLocation& location, bool recursive = false);
	void addInsideAsset(abAssetInfo* asset);
	void clear();
	virtual asAssetConfig* initFromConfigs() const;
	virtual abAssetInfo* clone() const;

	SERIALIZBLE_INHERITED_METHODS(abFolderInfo, abAssetInfo);

private:
	void linkAtlases();
	void linkImages();
	void linkChildFolders(abFolderInfo* parentFolder);
};

CLOSE_O2_NAMESPACE

#endif // ASSET_FILE_INFO_H
