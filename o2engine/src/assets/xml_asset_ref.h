#ifndef XML_ASSET_REF_H
#define XML_ASSET_REF_H

#include "xml_asset.h"

OPEN_O2_NAMESPACE

class asXmlAssetRef: public cObjectRef<asXmlDoc>
{
public:
	DEFINE_REF_TYPE();

	asXmlAssetRef();
	asXmlAssetRef(const string& location);
	asXmlAssetRef(const cFileLocation& location);

	pugi::xml_document& getXmlDoc();
};

asXmlAssetRef::asXmlAssetRef():
	cObjectRef(mnew asXmlDoc()) 
{}

asXmlAssetRef::asXmlAssetRef(const string& location):
	cObjectRef(mnew asXmlDoc(location)) 
{}

asXmlAssetRef::asXmlAssetRef(const cFileLocation& location):
	cObjectRef(mnew asXmlDoc(location)) 
{}

pugi::xml_document& asXmlAssetRef::getXmlDoc()
{
	return mObject->getXmlDoc();	
}

CLOSE_O2_NAMESPACE

#endif // XML_ASSET_REF_H
