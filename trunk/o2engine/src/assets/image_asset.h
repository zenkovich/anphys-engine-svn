#ifndef IMAGE_ASSET_H
#define IMAGE_ASSET_H

#include "asset.h"

OPEN_O2_NAMESPACE

class Bitmap;

class asImage: public asAsset, public Serializable
{
	DEFINE_TYPE(asImage);

protected:
	Bitmap* mImage;
	string  mAtlas;

public:
	asImage();
	asImage(const string& location);
	asImage(const FileLocation& location);
	asImage(Bitmap* image);
	~asImage();

	asImage& operator=(const asImage& asset);

	Bitmap* getImage();
	void setImage(Bitmap* image);

	void setAtlasName(const string& atlasName);

	SERIALIZBLE_METHODS(asImage);

protected:
	void loadData();
	void saveData();
	void loadImage();
};

CLOSE_O2_NAMESPACE

#endif // IMAGE_ASSET_H
