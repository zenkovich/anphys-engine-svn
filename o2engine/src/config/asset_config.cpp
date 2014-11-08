#include "asset_config.h"

OPEN_O2_NAMESPACE

cAssetConfig::cAssetConfig()
{
}

SERIALIZE_METHOD_IMPL(cAssetConfig)
{
	SERIALIZE_ID(&mLocation, "location");
	SERIALIZE_ID(mIncludeBuild, "includeBuild");
	return true;
}


cImageAssetConfig::cImageAssetConfig()
{
}

SERIALIZE_INHERITED_METHOD_IMPL(cImageAssetConfig) 
{
	SERIALIZE_ID(mScale, "scale");
	SERIALIZE_ID(mAtlas, "atlas");
	return true;
}

CLOSE_O2_NAMESPACE