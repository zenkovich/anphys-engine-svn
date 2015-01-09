#ifndef IMAGE_ASSET_H
#define IMAGE_ASSET_H

#include "asset.h"

OPEN_O2_NAMESPACE

class cImage;

class asImage: public asAsset, public cSerializable
{
	DEFINE_TYPE(asImage);

protected:
	cImage* mImage;
	string  mAtlas;

public:
	asImage();
	asImage(const string& location);
	asImage(const FileLocation& location);
	asImage(cImage* image);
	~asImage();

	asImage& operator=(const asImage& asset);

	cImage* getImage();
	void setImage(cImage* image);

	SERIALIZBLE_METHODS(asImage);

protected:
	void loadData();
	void saveData();
	void loadImage();
};

CLOSE_O2_NAMESPACE

#endif // IMAGE_ASSET_H
