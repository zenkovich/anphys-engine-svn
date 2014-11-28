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

asPathConfig::~asPathConfig()
{
	release_array(AssetsConfigsArr, mInsideAssets);
}

SERIALIZE_INHERITED_METHOD_IMPL(asPathConfig)
{
	SERIALIZE_ID(mInsideAssets, "assets");

	return true;
}

asAssetConfig* asPathConfig::getAssetConfig(const string& path)
{
	foreach(AssetsConfigsArr, mInsideAssets, assetIt)
		if ((*assetIt)->mLocation.mPath == path)
			return *assetIt;

	return NULL;
}


asAtlasConfig::asAtlasConfig():
	mMaxSize(2048, 2048)
{
}

SERIALIZE_INHERITED_METHOD_IMPL(asAtlasConfig)
{
	SERIALIZE_ID(mName, "name");
	SERIALIZE_ID(mMaxSize, "maxSize");

	return true;
}

CLOSE_O2_NAMESPACE