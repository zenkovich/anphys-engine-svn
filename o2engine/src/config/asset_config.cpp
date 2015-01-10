#include "asset_config.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(asAssetConfig);

asAssetConfig::asAssetConfig()
{
}

SERIALIZE_METHOD_IMPL(asAssetConfig)
{
	SERIALIZE_ID(&mLocation, "location");
	SERIALIZE_ID(mIncludeBuild, "includeBuild");
	return true;
}

asAssetConfig::~asAssetConfig()
{
}


REGIST_TYPE(asImageConfig);

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


REGIST_TYPE(asFolderConfig);

asFolderConfig::asFolderConfig()
{
}

asFolderConfig::~asFolderConfig()
{
	release_array(AssetsConfigsArr, mInsideAssets);
}

SERIALIZE_INHERITED_METHOD_IMPL(asFolderConfig)
{
	SERIALIZE_ID(mInsideAssets, "assets");

	return true;
}

asAssetConfig* asFolderConfig::getAndRemoveAssetConfig(const FileLocation& location)
{
	foreach(AssetsConfigsArr, mInsideAssets, assetIt)
	{
		if ((*assetIt)->mLocation == location)
		{
			asAssetConfig* res = *assetIt;
			mInsideAssets.remove(assetIt);
			return res;
		}

		if ((*assetIt)->getType() == asFolderConfig::getStaticType())
		{
			asAssetConfig* res = static_cast<asFolderConfig*>(*assetIt)->getAndRemoveAssetConfig(location);
			if (res)
				return res;
		}
	}

	return NULL;
}

asAssetConfig* asFolderConfig::getAssetConfig( const FileLocation& location )
{
	foreach(AssetsConfigsArr, mInsideAssets, assetIt)
	{
		if ((*assetIt)->mLocation == location)
			return *assetIt;

		if ((*assetIt)->getType() == asFolderConfig::getStaticType())
		{
			asAssetConfig* res = static_cast<asFolderConfig*>(*assetIt)->getAndRemoveAssetConfig(location);
			if (res)
				return res;
		}
	}

	return NULL;
}


REGIST_TYPE(asAtlasConfig);

asAtlasConfig::asAtlasConfig():
	mMaxSize(2048, 2048), mAttachedToFolder(false)
{
}

SERIALIZE_INHERITED_METHOD_IMPL(asAtlasConfig)
{
	SERIALIZE_ID(mName, "name");
	SERIALIZE_ID(mMaxSize, "maxSize");
	SERIALIZE_ID(mAttachedToFolder, "attachedToFolder");
	SERIALIZE_ID(&mAttachFolderLocation, "attachoFolderLocation");

	return true;
}

CLOSE_O2_NAMESPACE