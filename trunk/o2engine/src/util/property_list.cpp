#include "property_list.h"

OPEN_O2_NAMESPACE
	
PropertyList::PropertyList(const string& name /*= ""*/):
	mPropertyListName(name), mParentPropertyList(NULL)
{
}

PropertyList::~PropertyList()
{
	if (mParentPropertyList)
		mParentPropertyList->removeChildPropertyList(this, false);

	FOREACH(PropertiesListsVec, mChildPropertyLists, child)
		safe_release(*child);
}

void PropertyList::addChildPropertyList(PropertyList* propList)
{
	mChildPropertyLists.push_back(propList);
	propList->mParentPropertyList = this;
}

void PropertyList::removeChildPropertyList(PropertyList* propList, bool release /*= true*/)
{
	PropertiesListsVec::iterator fnd = FIND(mChildPropertyLists, propList);
	if (fnd == mChildPropertyLists.end())
		return;

	if (release)
		safe_release(*fnd);

	mChildPropertyLists.erase(fnd);
}

void PropertyList::removeAllChildPropertyLists()
{
	mChildPropertyLists.clear();
}

PropertyList::IProperty* PropertyList::getPropertyBase( const string& pathName )
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
		if ((*child)->mPropertyListName == pathPart)
		{
			return (*child)->getPropertyBase(pathName.substr(delPos + 1));
		}
	}

	return NULL;
}

void PropertyList::setPropertyListName( const string& name )
{
	mPropertyListName = name;
}


string PropertyList::IProperty::getPath()
{
	string res = mName;
	PropertyList* propList = mOwnerPropList;
	while(propList->mParentPropertyList)
	{
		res = propList->mPropertyListName + "/" + res;
		propList = propList->mParentPropertyList;
	}

	return res;
}

CLOSE_O2_NAMESPACE