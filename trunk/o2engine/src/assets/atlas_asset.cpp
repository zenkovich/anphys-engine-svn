#include "atlas_asset.h"
#include "config/asset_config.h"
#include "assets.h"
#include "app/application.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(asAtlas);

asAtlas::asAtlas():
	asAsset(), mAttachedToFolder(false)
{
}

asAtlas::asAtlas( const string& location ):
	asAsset(location)
{
	loadData();
}

asAtlas::asAtlas( const FileLocation& location ):
	asAsset(location)
{
	loadData();
}

asAtlas::~asAtlas()
{
}

asAtlas& asAtlas::operator=( const asAtlas& asset )
{
	return *this;
}

void asAtlas::addImage( const FileLocation& imageLocation )
{
	foreach(ImagesDefsArr, mImages, imgIt)
		if (imgIt->mLocation == imageLocation)
			return;

	mImages.add(ImageDef(imageLocation, true));
}

bool asAtlas::removeImage( const FileLocation& imageLocation )
{
	foreach(ImagesDefsArr, mImages, imgIt)
	{
		if (imgIt->mLocation == imageLocation)
		{
			if (!imgIt->mLinkedDirectly)
				return false; //because you can't remove image from atlas which linked from attached folder

			mImages.remove(imgIt);
			return true;
		}
	}

	return false;
}

asAtlas::ImagesDefsArr asAtlas::getImagesArray() const
{
	return mImages;
}

void asAtlas::setMaxSize( const vec2f& maxSize )
{
	mMaxSize = maxSize;
}

vec2f asAtlas::getMaxSize() const
{
	return mMaxSize;
}

void asAtlas::setName( const string& name )
{
	//@@@TODO check atlas name!
	mAtlasName = name;
}

string asAtlas::getName() const
{
	return mAtlasName;
}

void asAtlas::attachToFolder( const FileLocation& folderLocation )
{
	mAttachFolder = folderLocation;
	mAttachedToFolder = true;
}

FileLocation asAtlas::getAttachedFolder() const
{
	return mAttachFolder;
}

bool asAtlas::isAttachedToFolder() const
{
	return mAttachedToFolder;
}

void asAtlas::deattachFolder()
{
	mAttachedToFolder = false;
}

void asAtlas::loadData()
{
	//load basic parametres from config
	asAtlasConfig* config = static_cast<asAtlasConfig*>(assets()->mAssetsConfigs.getAssetConfig(mLocation));
	mMaxSize = config->mMaxSize;
	mAtlasName = config->mName;
	mAttachedToFolder = config->mAttachedToFolder;
	mAttachFolder = config->mAttachFolderLocation;

	//load images list from builded asset
	string buildedAssetPath = assets()->getAssetFullPath(mLocation);
	Serializer serializer;
	if (serializer.load(buildedAssetPath))
		serializer.serialize(mImages, "images");
}

void asAtlas::saveData()
{
	//save config	
	asAtlasConfig* config = static_cast<asAtlasConfig*>(assets()->mAssetsConfigs.getAssetConfig(mLocation));
	config->mMaxSize = mMaxSize;
	config->mName = mAtlasName;
	config->mAttachedToFolder = mAttachedToFolder;
	config->mAttachFolderLocation = mAttachFolder;

	foreach(ImagesDefsArr, mImages, imgIt)
	{
		if (!imgIt->mLinkedDirectly)
			continue;

		asImage imageAsset(imgIt->mLocation);
		imageAsset.setAtlasName(mAtlasName);
		imageAsset.save(imageAsset.getLocation().mPath, false);
	}
}

SERIALIZE_METHOD_IMPL(asAtlas)
{
	SERIALIZE_ID(mAtlasName, "atlasName");
	SERIALIZE_ID(mMaxSize, "maxSize");
	SERIALIZE_ID(mAttachedToFolder, "attachedToFolder");
	SERIALIZE_ID(&mAttachFolder, "attachoFolderLocation");

	return true;
}


asAtlas::ImageDef::ImageDef( const FileLocation& location /*= FileLocation()*/, bool linkedDirectly /*= true*/ ):
	mLocation(location), mLinkedDirectly(linkedDirectly)
{
}

bool asAtlas::ImageDef::operator=( const ImageDef& other )
{
	return mLocation == other.mLocation && mLinkedDirectly == other.mLinkedDirectly;
}

SERIALIZE_METHOD_IMPL(asAtlas::ImageDef) 
{
	SERIALIZE_ID(&mLocation, "location");
	SERIALIZE_ID(mLinkedDirectly, "linkedDirectly");
	return true;
}

CLOSE_O2_NAMESPACE