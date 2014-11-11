#ifndef ASSET_REF_H
#define ASSET_REF_H

#include "asset.h"

OPEN_O2_NAMESPACE
	
class asAssetRef: public cObjectRef<asAsset>
{
public:
	asAssetRef();
	asAssetRef(const string& location);
	asAssetRef(const cFileLocation& location);
	asAssetRef(const asAssetRef& asset);

	cFileLocation getLocation() const;
	string getPath() const;
	uint32 getFileId() const;

	char* getData() const;
	uint32 getDataSize() const;

	void setData(char* data, uint32 size);

	bool save(const string& path);
};


bool asAssetRef::save(const string& path)
{
	return mObject->save(path);
}

void asAssetRef::setData(char* data, uint32 size)
{
	mObject->setData(data, size);
}

uint32 asAssetRef::getDataSize() const
{
	return mObject->getDataSize();
}

char* asAssetRef::getData() const
{
	return mObject->getData();
}

uint32 asAssetRef::getFileId() const
{
	return mObject->getFileId();
}

string asAssetRef::getPath() const
{
	return mObject->getPath();
}

cFileLocation asAssetRef::getLocation() const
{
	return mObject->getLocation();
}

asAssetRef::asAssetRef(const asAssetRef& asset) :
	cObjectRef(mnew asAsset(*asset.mObject))
{}

asAssetRef::asAssetRef(const cFileLocation& location) :
	cObjectRef(mnew asAsset(location))
{}

asAssetRef::asAssetRef(const string& location) :
	cObjectRef(mnew asAsset(location))
{}

asAssetRef::asAssetRef() :
	cObjectRef(mnew asAsset())
{}

CLOSE_O2_NAMESPACE

#endif // ASSET_REF_H
