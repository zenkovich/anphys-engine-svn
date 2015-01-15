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
	asAssetConfig(const FileLocation& location, bool includeBuild = true);
	virtual ~asAssetConfig();

	virtual asAssetConfig* clone() const;

	SERIALIZBLE_METHODS(asAssetConfig);
};
typedef array<asAssetConfig*> AssetsConfigsArr;


class asImageConfig: public asAssetConfig
{
public:
	DEFINE_TYPE(asImageConfig);

	float  mScale;
	string mAtlas;

public:
	asImageConfig();
	asImageConfig(const FileLocation& location, const string& atlas = "", float scale = 1.0f, bool includeBuild = true);

	virtual asAssetConfig* clone() const;

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
	asAtlasConfig(const FileLocation& location, const string& name, const vec2f maxSize = vec2f(2048, 2048), 
		          bool includeBuild = true);

	virtual asAssetConfig* clone() const;
	
	SERIALIZBLE_INHERITED_METHODS(asAtlasConfig, asAssetConfig);
};

CLOSE_O2_NAMESPACE

#endif // ASSET_CONFIG_H
