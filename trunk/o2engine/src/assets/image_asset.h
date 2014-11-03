#ifndef IMAGE_ASSET_H
#define IMAGE_ASSET_H

#include "asset.h"

OPEN_O2_NAMESPACE

class cImage;

class asImage: public asAsset
{
protected:
	cImage* mImage;

public:
	asImage();
	asImage(const string& location);
	asImage(const cFileLocation& location);
	asImage(cImage* image);
	~asImage();

	cImage* getImage() const;
	void setImage(cImage* image);

protected:
	virtual void saveData();
};

CLOSE_O2_NAMESPACE

#endif // IMAGE_ASSET_H
