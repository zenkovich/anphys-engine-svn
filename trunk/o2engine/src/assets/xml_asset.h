#ifndef XML_ASSET_H
#define XML_ASSET_H

#include "asset.h"
#include "util/xml_tools.h"

OPEN_O2_NAMESPACE

class asXmlDoc: public asAsset
{
	DEFINE_TYPE(asXmlDoc);

protected:
	pugi::xml_document mXmlDoc;

public:
	asXmlDoc();
	asXmlDoc(const string& location);
	asXmlDoc(const FileLocation& location);
	~asXmlDoc();	

	pugi::xml_document& getXmlDoc();

private:
	asXmlDoc& operator=(const asXmlDoc& asset);

protected:
	void loadData();
	void saveData();
};

CLOSE_O2_NAMESPACE

#endif // XML_ASSET_H
