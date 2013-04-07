#ifndef DATA_OBJECTS_MANAGER_H
#define DATA_OBJECTS_MANAGER_H

#include <string>
#include <vector>

#include "util/other/singleton.h"

struct cDataObject;

struct cDataObjectsManager:public cSingleton<cDataObjectsManager>
{
	enum DataObjectType { DOT_XML = 0, DOT_BIN };

	struct DataObject
	{
		cDataObject*   mDataObject;
		std::string    mFileName;
		DataObjectType mFileType;

		DataObject():mDataObject(NULL), mFileType(DOT_XML) {}
		DataObject(cDataObject* dataObject, const std::string& filename, DataObjectType type):
			mDataObject(dataObject), mFileName(filename), mFileType(type) {}
		~DataObject();
	};
	typedef std::vector<DataObject*> DataObjectsList;

	DataObjectsList mCachedDataObjects;

//functions
	cDataObjectsManager();
	~cDataObjectsManager();

	cDataObject* loadDataObject(const std::string& filename, DataObjectType type, bool cache = true);
	void         saveDataObject(const std::string& filename, DataObjectType type, cDataObject& dataObject);

	void flush();

protected:
	cDataObject* loadFromXml(const std::string& filename, bool cache);
	void         saveToXml(const std::string& filename, cDataObject& dataObject);
};

#define getDataObjectsManager() cDataObjectsManager::instance()

#endif //DATA_OBJECTS_MANAGER_H