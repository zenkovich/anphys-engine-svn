#ifndef ASSET_CONFIG_H
#define ASSET_CONFIG_H

#include "public.h"
#include "util/file_system/file_info.h"
#include "util/serialize_util.h"

OPEN_O2_NAMESPACE

class cAssetConfig: public cSerializable
{
public:
	cFileLocation mLocation;
	bool          mIncludeBuild;

public:
	cAssetConfig();

	SERIALIZBLE_METHODS(cAssetConfig);
};

class cImageAssetConfig: public cAssetConfig
{
public:
	float  mScale;
	string mAtlas;

public:
	cImageAssetConfig();

	SERIALIZBLE_INHERITED_METHODS(cImageAssetConfig, cAssetConfig);
};

CLOSE_O2_NAMESPACE

#endif // ASSET_CONFIG_H
