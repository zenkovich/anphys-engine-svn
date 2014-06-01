#ifndef PROPERTY_LIST_H
#define PROPERTY_LIST_H

#include "public.h"

#include "util/callback.h"
#include "type_indexation.h"
#include "property.h"

OPEN_O2_NAMESPACE
	

/** Object property list class. Contains properties array, processing properties. */
class cPropertyList
{
public:
	class IProperty
	{
	public:	
		string                mName;         /** Name of property. */
		shared<cPropertyList> mPropertyList; /** Owner property list. NULL, if no owner. */
		cCallbackChain        onChangeEvent; /** Calls when calling setter. */

		IProperty(const string& name):
			mName(name) {} 
	};

	template<typename _type>
	class Property
	{
	public:
		/** Returns value. */
		virtual _type get() const
		{
			return mGetter != NULL ? (mClass->*mGetter)():(mClass->*mGetterNonConst)();
		}

		virtual void set(const _type& value)
		{
			if (mSetter != NULL)
				(mClass->*mSetter)(value);
			else
				(mClass->*mSetterNonConst)(value);

			onChangeEvent.call();
		}
	
		cProperty& operator=(const _type& value)
		{
			set(value);
			return *this;
		}
	
		cProperty& operator+=(const _type& value)
		{
			*this = *this + value;
			return *this;
		}

		_type operator+(const _type& value)
		{
			return get() + value;
		}
	
		cProperty& operator-=(const _type& value)
		{
			*this = *this - value;
			return *this;
		}

		_type operator-(const _type& value)
		{
			return get() - value;
		}
	
		cProperty& operator*=(const _type& value)
		{
			*this = *this * value;
			return *this;
		}

		_type operator*(const _type& value)
		{
			return get() * value;
		}
	
		cProperty& operator/=(const _type& value)
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
	shared< IProperty > mParentPropertyList; /** Parent property list. */
	PropertiesListsVec  mChildPropertyLists; /** Child properties list array. */
	PropertiesVec       mPropertiesList;     /** Properties array .*/

public:
	void addChildPropertyList(const shared<cPropertyList>& propList);

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
