#include "asset_config.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(asAssetConfig);

asAssetConfig::asAssetConfig():
	mIncludeBuild(true)
{
}

asAssetConfig::asAssetConfig( const FileLocation& location, bool includeBuild /*= true*/ ):
	mLocation(location), mIncludeBuild(includeBuild)
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

asAssetConfig* asAssetConfig::clone() const
{
	return mnew asAssetConfig(*this);
}


REGIST_TYPE(asImageConfig);

asImageConfig::asImageConfig():
	mScale(1.0f)
{
}

asImageConfig::asImageConfig( const FileLocation& location, const string& atlas /*= ""*/, float scale /*= 1.0f*/, 
		                      bool includeBuild /*= true*/ ):
	asAssetConfig(location, includeBuild), mAtlas(atlas), mScale(scale)
{
}

SERIALIZE_INHERITED_METHOD_IMPL(asImageConfig) 
{
	SERIALIZE_ID(mScale, "scale");
	SERIALIZE_ID(mAtlas, "atlas");
	return true;
}

asAssetConfig* asImageConfig::clone() const
{
	return mnew asImageConfig(*this);
}


REGIST_TYPE(asAtlasConfig);

asAtlasConfig::asAtlasConfig():
	mMaxSize(2048, 2048), mAttachedToFolder(false)
{
}

asAtlasConfig::asAtlasConfig( const FileLocation& location, const string& name, const vec2f maxSize /*= vec2f(2048, 2048)*/, 
	                          bool includeBuild /*= true*/ ):
	asAssetConfig(location, includeBuild), mName(name), mMaxSize(maxSize), mAttachedToFolder(false)
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

asAssetConfig* asAtlasConfig::clone() const
{
	return mnew asAtlasConfig(*this);
}

CLOSE_O2_NAMESPACE