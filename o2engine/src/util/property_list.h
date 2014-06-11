#ifndef PROPERTY_LIST_H
#define PROPERTY_LIST_H

#include "public.h"

#include "util/callback.h"

OPEN_O2_NAMESPACE
	

/** Object property list class. Contains properties array, processing properties. */
class cPropertyList: public cShareObject
{
	friend class IProperty;

public:
	class IProperty: public cShareObject
	{
	public:	
		string                mName;         /** Name of property. */
		shared<cPropertyList> mPropertyList; /** Owner property list. NULL, if no owner. */
		cCallbackChain        onChangeEvent; /** Calls when calling setter. */

		IProperty(const string& name):
			mName(name) {} 

		virtual ~IProperty() {}

		string getPath() const;

	protected:
		template<typename _class>
		void checkPropertyList(_class* tclass)
		{
			cPropertyList* propList = dynamic_cast<cPropertyList*>(tclass);
			if (propList)
			{
				propList->mPropertiesList.push_back((this));
				mPropertyList = (propList);
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

	typedef vector< shared<IProperty> > PropertiesVec;
	typedef vector< shared<cPropertyList> > PropertiesListsVec;

protected:
	string                  mPropertyListName;   /** Property list name. */
	shared< cPropertyList > mParentPropertyList; /** Parent property list. */
	PropertiesListsVec      mChildPropertyLists; /** Child properties list array. */
	PropertiesVec           mPropertiesList;     /** Properties array .*/

public:
	cPropertyList(const string& name = "");
	virtual ~cPropertyList();

	void setPropertyListName(const string& name);
	void addChildPropertyList(const shared<cPropertyList>& propList);
	void removeChildPropertyList(const shared<cPropertyList>& propList, bool release = true);
	void removeAllChildPropertyLists();

	template<typename T>
	shared< Property<T> > getProperty(const string& pathName)
	{
		return getPropertyBase(pathName);
	}

protected:
	shared<IProperty> getPropertyBase(const string& pathName);
};

CLOSE_O2_NAMESPACE

#endif //PROPERTY_LIST_H
