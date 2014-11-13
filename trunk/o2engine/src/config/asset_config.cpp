#include "asset_config.h"

OPEN_O2_NAMESPACE

asAssetConfig::asAssetConfig()
{
}

SERIALIZE_METHOD_IMPL(asAssetConfig)
{
	SERIALIZE_ID(&mLocation, "location");
	SERIALIZE_ID(mIncludeBuild, "includeBuild");
	return true;
}


asImageConfig::asImageConfig():
	mScale(1.0f)
{
}

SERIALIZE_INHERITED_METHOD_IMPL(asImageConfig) 
{
	SERIALIZE_ID(mScale, "scale");
	SERIALIZE_ID(mAtlas, "atlas");
	return true;
}


asPathConfig::asPathConfig()
{
}

SERIALIZE_INHERITED_METHOD_IMPL(asPathConfig)
{
	SERIALIZE_ID(mAssets, "assets");
}

CLOSE_O2_NAMESPACE