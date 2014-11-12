#ifndef IMAGE_ASSET_REF_H
#define IMAGE_ASSET_REF_H

#include "image_asset.h"

OPEN_O2_NAMESPACE

class asImageRef: public cObjectRef<asImage>
{
public:
	DEFINE_REF_TYPE();

	asImageRef();
	asImageRef(const string& location);
	asImageRef(const cFileLocation& location);
	asImageRef(cImage* image);

	cImage* getImage() const;
	void setImage(cImage* image);
};


asImageRef::asImageRef():
	cObjectRef(mnew asImage()) 
{}

asImageRef::asImageRef(const string& location):
	cObjectRef(mnew asImage(location)) 
{}

asImageRef::asImageRef(const cFileLocation& location):
	cObjectRef(mnew asImage(location)) 
{}

asImageRef::asImageRef(cImage* image):
	cObjectRef(mnew asImage(image)) 
{}

cImage* asImageRef::getImage() const 
{ 
	return mObject->getImage();
}

void asImageRef::setImage(cImage* image) 
{
	mObject->setImage(image); 
}

CLOSE_O2_NAMESPACE

#endif // IMAGE_ASSET_REF_H
