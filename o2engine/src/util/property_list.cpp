#include "property_list.h"

OPEN_O2_NAMESPACE
	
cObjectWithPropertyList::Property::Property(void* ptr, const string& id, Property::Type type):
	mObjectPtr(ptr), mId(id), mType(type)
{
}

void cObjectWithPropertyList::registProperty(void* ptr, const string& id, Property::Type type)
{
	mPropertiesList.push_back(Property(ptr, id, type));
}

void cObjectWithPropertyList::registProperty(int& value, const string& id)
{
	mPropertiesList.push_back(Property(&value, id, Property::TP_INT));
}

void cObjectWithPropertyList::registProperty(float& value, const string& id)
{
	mPropertiesList.push_back(Property(&value, id, Property::TP_FLOAT));
}

void cObjectWithPropertyList::registProperty(string& value, const string& id)
{
	mPropertiesList.push_back(Property(&value, id, Property::TP_STRING));
}

void cObjectWithPropertyList::registProperty(vec2f& value, const string& id)
{
	mPropertiesList.push_back(Property(&value, id, Property::TP_VEC2));
}

cObjectWithPropertyList::Property cObjectWithPropertyList::getProperty(const string& id)
{
	FOREACH(PropertiesVec, mPropertiesList, prop)
		if (prop->mId == id)
			return *prop;

	return Property(NULL, "", Property::TP_INT);
}

CLOSE_O2_NAMESPACE