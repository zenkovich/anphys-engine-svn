#ifndef DATA_OBJECT_H
#define DATA_OBJECT_H

#include <vector>
#include <string>
#include "dependencies/pugixml/pugixml.hpp"

#include "../math/mmath.h"
#include "../math/rect.h"

template<typename T>
struct cDataObjectContainer;

/** Объект хранения упорядоченных данных. 
 ** Имеет строковый идентификатор и массив дочерних объектов*/
struct cDataObject
{
	typedef std::vector<cDataObject*> DataObjectsList;

	std::string     mId;
	DataObjectsList mChilds;
	cDataObject*    mParent;

	//functions
	cDataObject(const std::string& id = "default");
	cDataObject(const cDataObject& object);
	virtual ~cDataObject();
	
//adding childs
	cDataObject* addChild(const std::string& id);
	cDataObject* addChild(cDataObject* child);

	template<typename T>
	cDataObjectContainer<T>* addChild(const T& object, const std::string& id)
	{
		cDataObjectContainer<T>* newObject = new cDataObjectContainer<T>(object, id);

		mChilds.push_back(newObject);
		newObject->mParent = this;

		return newObject;
	}

	template<typename T>
	cDataObjectContainer<T>* addChild(const std::string& id)
	{
		cDataObjectContainer<T>* newObject = new cDataObjectContainer<T>(id);

		mChilds.push_back(newObject);
		newObject->mParent = this;

		return newObject;
	}

	cDataObjectContainer<std::string>* addChild(const char* str, const std::string& id)
	{
		return addChild<std::string>(str, id);
	}

//getting childs
	cDataObject* getChild(const std::string& id);

	template<typename T>
	cDataObjectContainer<T>* getChild(const std::string& id)
	{
		cDataObject* child = getChild(id);

		if (!child) 
			return NULL;

		return static_cast<cDataObjectContainer<T>*>(child);
	}

	template<typename T> T& getChildValue(const std::string& id)
	{ 
		cDataObjectContainer<T>* child = getChild<T>(id);

		assert(child, "");

		return child->mObject;
	}

	template<typename T> bool getChildValue(const std::string& id, T& value)
	{ 
		cDataObjectContainer<T>* child = getChild<T>(id);

		if (!child)
			return false;

		value = getChild<T>(id)->mObject;

		return true;
	}

//clearing childs
	bool removeChild(cDataObject* child);

	void clear();

	virtual cDataObject* clone() const;

//save load
	virtual void saveDataTo(pugi::xml_node& xmlNode);
	virtual void loadDataFrom(pugi::xml_node& xmlNode);

	unsigned long getBinData(unsigned char* dst) { return 0; }
	void loadBinData(unsigned char* data, unsigned long size) {  }

	static void getXmlValue(pugi::xml_node& xmlNode, float& value);
	static void setXmlvalue(pugi::xml_node& xmlNode, float value);

	static void getXmlValue(pugi::xml_node& xmlNode, std::string& value);
	static void setXmlvalue(pugi::xml_node& xmlNode, std::string value);

	static void getXmlValue(pugi::xml_node& xmlNode, int& value);
	static void setXmlvalue(pugi::xml_node& xmlNode, int value);

	static void getXmlValue(pugi::xml_node& xmlNode, unsigned int& value);
	static void setXmlvalue(pugi::xml_node& xmlNode, unsigned int value);

	static void getXmlValue(pugi::xml_node& xmlNode, bool& value);
	static void setXmlvalue(pugi::xml_node& xmlNode, bool value);

	static void getXmlValue(pugi::xml_node& xmlNode, vec2& value);
	static void setXmlvalue(pugi::xml_node& xmlNode, vec2 value);

	static void getXmlValue(pugi::xml_node& xmlNode, vec3& value);
	static void setXmlvalue(pugi::xml_node& xmlNode, vec3 value);

	static void getXmlValue(pugi::xml_node& xmlNode, color4& value);
	static void setXmlvalue(pugi::xml_node& xmlNode, color4 value);

	static void getXmlValue(pugi::xml_node& xmlNode, fRect& value);
	static void setXmlvalue(pugi::xml_node& xmlNode, fRect value);
};

struct cDataObjectInterface:public cDataObject
{
	cDataObjectInterface(const std::string& id = "default"):cDataObject(id) {}
	virtual ~cDataObjectInterface() {}

	virtual cDataObject* clone() const = 0;

	virtual void saveDataTo(pugi::xml_node& xmlNode) {}
	virtual void loadDataFrom(pugi::xml_node& xmlNode) {}

	unsigned long getBinData(unsigned char* dst) { return 0; }
	void loadBinData(unsigned char* data, unsigned long size) {  }
};

template<typename T>
struct cDataObjectContainer:public cDataObjectInterface
{
	T mObject;


	cDataObjectContainer(const std::string& id = "default"):cDataObjectInterface(id) {}

	cDataObjectContainer(const T& object, const std::string& id = "default") 
	{ 
		mObject = object;
		mId = id;
	}

	cDataObjectContainer(const cDataObjectContainer& objectContainer)
	{
		mObject = objectContainer.mObject;
		mId = objectContainer.mId;

		for (DataObjectsList::const_iterator it = objectContainer.mChilds.cbegin(); it != objectContainer.mChilds.cend(); ++it)
			mChilds.push_back((*it)->clone());
	}

	virtual cDataObject* clone() const { return new cDataObjectContainer(*this); }

	virtual void saveDataTo(pugi::xml_node& xmlNode);
	virtual void loadDataFrom(pugi::xml_node& xmlNode);
};

template<typename T>
void cDataObjectContainer<T>::loadDataFrom( pugi::xml_node& xmlNode )
{
	cDataObject::loadDataFrom(xmlNode);
	cDataObject::getXmlValue(xmlNode, mObject);
}

template<typename T>
void cDataObjectContainer<T>::saveDataTo( pugi::xml_node& xmlNode )
{
	xmlNode.set_name(mId.c_str());
	
	cDataObject::setXmlvalue(xmlNode, mObject);

	for (DataObjectsList::iterator it = mChilds.begin(); it != mChilds.end(); ++it)
		(*it)->saveDataTo(xmlNode.append_child());
}

enum ArchieveType { AT_INPUT = 0, AT_OUTPUT };

template<typename T>
bool serializeType(cDataObject& dataObject, ArchieveType archType, T& value, const std::string& id)
{
	if (archType == AT_INPUT) return dataObject.getChildValue(id, value);
	else                      dataObject.addChild(value, id);

	return true;
}

template<typename T>
bool serializeTypeArr(cDataObject& dataObject, ArchieveType archType, T& value, unsigned int size, const std::string& id)
{
	bool res = true;

	cDataObject* targetDataObject = NULL;
	if (archType == AT_INPUT) targetDataObject = dataObject.getChild(id);
	else                      targetDataObject = dataObject.addChild(id);

	if (!targetDataObject) return false;

	for (unsigned int i = 0; i < size; i++)
		if (!serializeType(*targetDataObject, archType, value[i], "v" + toStr((int)i))) res = false;

	return res;
}

template<typename T>
bool serializeObjArr(cDataObject& dataObject, ArchieveType archType, T& object, unsigned int size, const std::string& id)
{
	bool res = true;

	cDataObject* targetDataObject = NULL;
	if (archType == AT_INPUT) targetDataObject = dataObject.getChild(id);
	else                      targetDataObject = dataObject.addChild(id);

	if (!targetDataObject) return false;

	for (unsigned int i = 0; i < size; i++)
		if (!object[i].serialize(*targetDataObject, archType, "v" + toStr((int)i))) res = false;

	return res;
}

template<typename T>
bool serializeObjPtrArr(cDataObject& dataObject, ArchieveType archType, T& object, unsigned int size, const std::string& id)
{
	bool res = true;

	cDataObject* targetDataObject = NULL;
	if (archType == AT_INPUT) targetDataObject = dataObject.getChild(id);
	else                      targetDataObject = dataObject.addChild(id);

	if (!targetDataObject) return false;

	for (unsigned int i = 0; i < size; i++)
		if (!object[i]->serialize(*targetDataObject, archType, "v" + toStr((int)i))) res = false;

	return res;
}


#endif //DATA_OBJECT_H