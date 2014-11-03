#ifndef XML_ASSET_H
#define XML_ASSET_H

#include "asset.h"
#include "util/xml_tools.h"

OPEN_O2_NAMESPACE

class asXmlAsset: public asAsset
{
protected:
	pugi::xml_document mXmlDoc;

public:
	asXmlAsset();
	asXmlAsset(const string& location);
	asXmlAsset(const cFileLocation& location);
	asXmlAsset(const pugi::xml_document& xmlDoc);
	~asXmlAsset();

	pugi::xml_document& getXmlDoc();

protected:
	virtual void saveData();
};

CLOSE_O2_NAMESPACE

#endif // XML_ASSET_H
