#ifndef ATLAS_ASSET_H
#define ATLAS_ASSET_H

#include "asset.h"
#inc

OPEN_O2_NAMESPACE
	
class asAtlas: public asAsset, public cSerializable
{
	DEFINE_TYPE(asAtlas);

protected:
	string         mAtlasName;
	vec2f          mMaxSize;
	bool           mAttachedToFolder;
	cFileLocation  mAttachFolder;
	AssetsInfosArr mImages;
	bool           mImagesLoaded;

public:
	asAtlas();
	asAtlas(const string& location);
	asAtlas(const cFileLocation& location);
	~asAtlas();

	asAtlas& operator=(const asAtlas& asset);

	void addImage(const cFileLocation& imageLocation);
	void addImage(const asAssetInfo& imageInfo);
	void addImage(const asImage& image);

	SERIALIZBLE_METHODS(asAtlas);

protected:
	void loadData();
	void saveData();
	void checkLoadingImages();
};

CLOSE_O2_NAMESPACE

#endif // ATLAS_ASSET_H
