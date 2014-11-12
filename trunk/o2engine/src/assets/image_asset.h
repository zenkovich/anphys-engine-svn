#ifndef IMAGE_ASSET_H
#define IMAGE_ASSET_H

#include "asset.h"

OPEN_O2_NAMESPACE

class cImage;

class asImage: public asAsset
{
	DEFINE_TYPE(asImage);

protected:
	cImage* mImage;

protected:
	asImage();
	asImage(const string& location);
	asImage(const cFileLocation& location);
	asImage(cImage* image);
	~asImage();

	asImage& operator=(const asImage& asset);

	cImage* getImage() const;
	void setImage(cImage* image);

protected:
	void saveData();
};

CLOSE_O2_NAMESPACE

#endif // IMAGE_ASSET_H
