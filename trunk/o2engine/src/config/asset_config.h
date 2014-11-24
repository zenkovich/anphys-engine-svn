#ifndef ASSET_CONFIG_H
#define ASSET_CONFIG_H

#include "public.h"
#include "util/file_system/file_info.h"
#include "util/serialize_util.h"

OPEN_O2_NAMESPACE

class asAssetConfig: public cSerializable
{
public:
	cFileLocation mLocation;
	bool          mIncludeBuild;

public:
	asAssetConfig();

	SERIALIZBLE_METHODS(asAssetConfig);
};
typedef array<asAssetConfig*> AssetsConfigsArr;

class asPathConfig: public asAssetConfig
{
public:
	AssetsConfigsArr mInsideAssets;

public:
	asPathConfig();
	~asPathConfig();

	asAssetConfig* getAssetConfig(const string& path);

	SERIALIZBLE_INHERITED_METHODS(asPathConfig, asAssetConfig);
};

class asImageConfig: public asAssetConfig
{
public:
	float  mScale;
	string mAtlas;

public:
	asImageConfig();

	SERIALIZBLE_INHERITED_METHODS(asImageConfig, asAssetConfig);
};

CLOSE_O2_NAMESPACE

#endif // ASSET_CONFIG_H
