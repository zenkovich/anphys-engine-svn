#ifndef ASSET_CONFIG_H
#define ASSET_CONFIG_H

#include "public.h"
#include "util/file_system/file_info.h"
#include "util/serialize_util.h"
#include "util/type_indexation.h"

OPEN_O2_NAMESPACE

class asAssetConfig: public Serializable
{
public:
	DEFINE_TYPE(asAssetConfig);

	FileLocation mLocation;
	bool         mIncludeBuild;

public:
	asAssetConfig();
	virtual ~asAssetConfig();

	SERIALIZBLE_METHODS(asAssetConfig);
};
typedef array<asAssetConfig*> AssetsConfigsArr;

class asFolderConfig: public asAssetConfig
{
	friend class AssetBuildSystem;

public:
	DEFINE_TYPE(asFolderConfig);

	AssetsConfigsArr mInsideAssets;

public:
	asFolderConfig();
	~asFolderConfig();
	
	asAssetConfig* getAssetConfig(const FileLocation& location);

	SERIALIZBLE_INHERITED_METHODS(asFolderConfig, asAssetConfig);

private:
	asAssetConfig* getAndRemoveAssetConfig(const FileLocation& location);
};

class asImageConfig: public asAssetConfig
{
public:
	DEFINE_TYPE(asImageConfig);

	float  mScale;
	string mAtlas;

public:
	asImageConfig();

	SERIALIZBLE_INHERITED_METHODS(asImageConfig, asAssetConfig);
};

class asAtlasConfig: public asAssetConfig
{
public:
	DEFINE_TYPE(asAtlasConfig);

	vec2f        mMaxSize;
	string       mName;
	bool         mAttachedToFolder;
	FileLocation mAttachFolderLocation;

public:
	asAtlasConfig();
	
	SERIALIZBLE_INHERITED_METHODS(asAtlasConfig, asAssetConfig);
};

CLOSE_O2_NAMESPACE

#endif // ASSET_CONFIG_H
