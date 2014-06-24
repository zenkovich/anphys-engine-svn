#ifndef PROPERTY_LIST_H
#define PROPERTY_LIST_H

#include "public.h"
#include "util/callback.h"

OPEN_O2_NAMESPACE
	

/** Object property list class. Contains properties array, processing properties. */
class cPropertyList
{
	friend class IProperty;

public:
	class IProperty
	{
	public:	
		string         mName;         /** Name of property. */
		cPropertyList* mOwnerPropList; /** Owner property list. NULL, if no owner. */
		cCallbackChain onChangeEvent; /** Calls when calling setter. */

		IProperty(const string& name):
			mName(name) {} 

		virtual ~IProperty() {}

		string getPath();

	protected:
		template<typename _class>
		void checkPropertyList(_class* tclass)
		{
			cPropertyList* propList = dynamic_cast<cPropertyList*>(tclass);
			if (propList)
			{
				propList->mPropertiesList.push_back(this);
				mOwnerPropList = (propList);
			}
		}
	};

	template<typename _type>
	class Property: public IProperty
	{
	public:
		Property():IProperty("") {}

		/** Returns value. */
		virtual _type get() const = 0;
		virtual void set(const _type& value) = 0;
	
		Property& operator=(const _type& value)
		{
			set(value);
			return *this;
		}
	
		Property& operator+=(const _type& value)
		{
			*this = *this + value;
			return *this;
		}

		_type operator+(const _type& value)
		{
			return get() + value;
		}
	
		Property& operator-=(const _type& value)
		{
			*this = *this - value;
			return *this;
		}

		_type operator-(const _type& value)
		{
			return get() - value;
		}
	
		Property& operator*=(const _type& value)
		{
			*this = *this * value;
			return *this;
		}

		_type operator*(const _type& value)
		{
			return get() * value;
		}
	
		Property& operator/=(const _type& value)
		{
			*this = *this / value;
			return *this;
		}

		_type operator/(const _type& value)
		{
			return get() / value;
		}
	};

	typedef vector<IProperty*> PropertiesVec;
	typedef vector<cPropertyList*> PropertiesListsVec;

protected:
	string             mPropertyListName;   /** Property list name. */
	cPropertyList*     mParentPropertyList; /** Parent property list. */
	PropertiesListsVec mChildPropertyLists; /** Child properties list array. */
	PropertiesVec      mPropertiesList;     /** Properties array .*/

public:
	cPropertyList(const string& name = "");
	virtual ~cPropertyList();

	void setPropertyListName(const string& name);
	void addChildPropertyList(cPropertyList* propList);
	void removeChildPropertyList(cPropertyList* propList, bool release = true);
	void removeAllChildPropertyLists();

	template<typename T>
	Property<T>* getProperty(const string& pathName)
	{
		return static_cast< Property<T>* >(getPropertyBase(pathName));
	}

protected:
	IProperty* getPropertyBase(const string& pathName);
};

CLOSE_O2_NAMESPACE

#endif //PROPERTY_LIST_H
