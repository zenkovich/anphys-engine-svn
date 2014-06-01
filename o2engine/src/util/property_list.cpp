#include "property_list.h"

OPEN_O2_NAMESPACE
	
cPropertyList::cPropertyList(const string& name /*= ""*/):
	mName(name)
{
}

cPropertyList::~cPropertyList()
{
	FOREACH(PropertiesListsVec, mChildPropertyLists, child)
		safe_release(*child);
}

void cPropertyList::addChildPropertyList( const shared<cPropertyList>& propList )
{
	mChildPropertyLists.push_back(propList);
	propList->mParentPropertyList = tempShared(this);
}

string cPropertyList::getPropertyPath( const shared<IProperty>& prop )
{
	string res = prop->mName;
	shared<cPropertyList> propList = prop->mPropertyList;
	while(propList != this)
	{
		res = prop->mPropertyList->mName + "/" + res;
		propList = prop->mPropertyList->mParentPropertyList;
	}

	return res;
}

shared<cPropertyList::IProperty> cPropertyList::getPropertyBase( const string& pathName )
{
	int delPos = pathName.find("/");
	string pathPart = pathName.substr(0, delPos);

	if (delPos == pathName.npos)
	{
		FOREACH(PropertiesVec, mPropertiesList, prop)
			if ((*prop)->mName == pathPart)
				return *prop;
	}

	FOREACH(PropertiesListsVec, mChildPropertyLists, child)
	{
		if ((*child)->mName == pathPart)
		{
			return (*child)->getPropertyBase(pathName.substr(delPos + 1));
		}
	}

	return NULL;
}

CLOSE_O2_NAMESPACE