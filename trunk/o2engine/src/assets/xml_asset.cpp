#include "xml_asset.h"

#include "app/application.h"
#include "assets.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(asXmlDoc);

asXmlDoc::asXmlDoc():
	asAsset()
{
}

asXmlDoc::asXmlDoc(const string& location):
	asAsset(location)
{
	loadData();
}

asXmlDoc::asXmlDoc(const FileLocation& location):
	asAsset(location)
{
	loadData();
}

asXmlDoc::~asXmlDoc()
{
}

asXmlDoc& asXmlDoc::operator=(const asXmlDoc& asset)
{
	return *this;
}

pugi::xml_document& asXmlDoc::getXmlDoc()
{
	return mXmlDoc;
}

void asXmlDoc::saveData()
{
	cXmlTools::saveToFile(assets()->getAssetSourceFullPath(mLocation.mPath), mXmlDoc);
}

void asXmlDoc::loadData()
{
	if (!cXmlTools::loadFromFile(assets()->getAssetFullPath(mLocation.mPath), mXmlDoc))
	{
		assets()->mLog->error("Filed to load xml asset: %s", mLocation.mPath.c_str());
	}
}

CLOSE_O2_NAMESPACE