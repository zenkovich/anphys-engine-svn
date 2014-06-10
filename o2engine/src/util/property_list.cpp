#include "property_list.h"

OPEN_O2_NAMESPACE
	
cPropertyList::cPropertyList(const string& name /*= ""*/):
	mPropertyListName(name)
{
}

cPropertyList::~cPropertyList()
{
	if (mParentPropertyList)
		mParentPropertyList->removeChildPropertyList(this, false);

	FOREACH(PropertiesListsVec, mChildPropertyLists, child)
		safe_release(*child);
}

void cPropertyList::addChildPropertyList( const shared<cPropertyList>& propList )
{
	mChildPropertyLists.push_back(propList);
	propList->mParentPropertyList = this;
}

void cPropertyList::removeChildPropertyList( const shared<cPropertyList>& propList, bool release /*= true*/ )
{
	PropertiesListsVec::iterator fnd = FIND(mChildPropertyLists, propList);
	if (fnd == mChildPropertyLists.end())
		return;

	if (release)
		safe_release(*fnd);

	mChildPropertyLists.erase(fnd);
}

void cPropertyList::removeAllChildPropertyLists()
{
	mChildPropertyLists.clear();
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
		if ((*child)->mPropertyListName == pathPart)
		{
			return (*child)->getPropertyBase(pathName.substr(delPos + 1));
		}
	}

	return NULL;
}

void cPropertyList::setPropertyListName( const string& name )
{
	mPropertyListName = name;
}


string cPropertyList::IProperty::getPath() const
{
	string res = mName;
	shared<cPropertyList> propList = mPropertyList;
	while(propList->mParentPropertyList)
	{
		res = propList->mPropertyListName + "/" + res;
		propList = propList->mParentPropertyList;
	}

	return res;
}

CLOSE_O2_NAMESPACE