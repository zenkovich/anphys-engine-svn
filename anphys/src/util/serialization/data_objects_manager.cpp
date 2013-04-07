#include "data_objects_manager.h"

#include "dependencies/pugixml/pugixml.hpp"

#include "data_object.h"
#include "util/file_system/file_system.h"
#include "util/file_system/file.h"

CREATE_SINGLETON(cDataObjectsManager)

cDataObjectsManager::cDataObjectsManager()
{
}

cDataObjectsManager::~cDataObjectsManager()
{
	flush();
}

cDataObject* cDataObjectsManager::loadDataObject( const std::string& filename, DataObjectType type, bool cache /*= true*/ )
{
	if (cache)
	{
		for (DataObjectsList::iterator it = mCachedDataObjects.begin(); it != mCachedDataObjects.end(); ++it)
		{
			if ((*it)->mFileType != type || (*it)->mFileName != filename) continue;

			return (*it)->mDataObject;
		}
	}

	if (type == DOT_XML) return loadFromXml(filename, cache);

	return NULL;
}

void cDataObjectsManager::flush()
{
	for (DataObjectsList::iterator it = mCachedDataObjects.begin(); it != mCachedDataObjects.end(); ++it)
		safe_release(*it);

	mCachedDataObjects.clear();
}

cDataObject* cDataObjectsManager::loadFromXml( const std::string& filename, bool cache )
{
	cFile* xmlFile = getFileSystem().openFileRead(filename + ".xml");

	if (!xmlFile)
	{
		getFileSystem().mLog->fout(1, "ERROR: Can't load file '%s' for xml data object", filename.c_str());
		return NULL;
	}

	pugi::xml_document xmlDoc;

	unsigned long size = xmlFile->getDataSize();
	unsigned char* data = new unsigned char[size];
	xmlFile->readFullData(data);

	pugi::xml_parse_result res = xmlDoc.load_buffer(data, size);
	if (res.status != pugi::status_ok)
	{
		getFileSystem().mLog->fout(1, "ERROR: Can't load xml file '%s': %s", filename.c_str(), res.description());

		safe_release_arr(data);

		return NULL;
	}

	cDataObject* dataObject = new cDataObject;
	dataObject->loadDataFrom(xmlDoc.first_child());

	if (cache)
		mCachedDataObjects.push_back(new DataObject(dataObject, filename, DOT_XML));

	safe_release_arr(data);

	getFileSystem().closeFile(xmlFile);

	return dataObject;
}

void cDataObjectsManager::saveDataObject( const std::string& filename, DataObjectType type, cDataObject& dataObject )
{
	if (type == DOT_XML) saveToXml(filename, dataObject);
}

void cDataObjectsManager::saveToXml( const std::string& filename, cDataObject& dataObject )
{
	pugi::xml_document xmlDoc;
	dataObject.saveDataTo(xmlDoc.append_child());
	xmlDoc.save_file(filename.c_str());
}

cDataObjectsManager::DataObject::~DataObject()
{
	safe_release(mDataObject);
}
