#ifndef PROPERTY_LIST_H
#define PROPERTY_LIST_H

#include "public.h"

#include "util/callback.h"
#include "type_indexation.h"
#include "property.h"

OPEN_O2_NAMESPACE

/** Object property list class. Contains properties array, processing properties. */
class cObjectWithPropertyList
{
public:
	struct IProperty
	{
		string mId;
		cCallbackChain mOnChange;

		IProperty(const string& id):mId(id) {}
	};

	template<typename T>
	struct DataProperty: public IProperty
	{
		T* mDataPtr;

		virtual void set(const T& value)
		{
			if (equals(value, get()))
				return;

			*mDataPtr = value;

			mOnChange.call();
		}

		virtual T get() const
		{
			return *mDataPtr;
		}

		DataProperty(T* dataPtr, const string& id):
			IProperty(id)
		{ 
			mDataPtr = dataPtr;
		}

		DataProperty& operator=(const T& value)
		{
			set(value);
			return *this;
		}
	
		DataProperty& operator+=(const T& value)
		{
			*this = *this + value;
			return *this;
		}

		T operator+(const T& value)
		{
			return get() + value;
		}
	
		DataProperty& operator-=(const T& value)
		{
			*this = *this - value;
			return *this;
		}

		T operator-(const T& value)
		{
			return get() - value;
		}
	
		DataProperty& operator*=(const T& value)
		{
			*this = *this * value;
			return *this;
		}

		T operator*(const T& value)
		{
			return get() * value;
		}
	
		DataProperty& operator/=(const T& value)
		{
			*this = *this / value;
			return *this;
		}

		T operator/(const T& value)
		{
			return get() / value;
		}
	};

	template<typename _class, typename _type>
	struct FuncProperty: public DataProperty<_type>
	{
		cProperty<_class, _type> mProperty;

		FuncProperty(const cProperty<_class, _type>& prop, const string& id):
			DataProperty(NULL, id)
		{
			prop.copy(mProperty);
		}

		FuncProperty(_class* tclass, void* setterFunc, void* getterFunc, bool setterConst = true, bool getterConst = true):
			DataProperty(NULL, id)
		{
			mProperty = cProperty<_class, _type>(tclass, setterFunc, getterFunc, setterConst, getterConst);
		}

		virtual void set(const _type& value)
		{
			if (equals(get(), value))
				return;

			mProperty = value;

			mOnChange.call();
		}

		virtual _type get() const
		{
			return mProperty.get();
		}
	};


	typedef vector< shared<IProperty> > PropertiesVec;

protected:
	PropertiesVec mPropertiesList;

	/** Initialization properties func. Must be implemented. */
	virtual void initializePropertiesList() = 0;

	/** Registering data pointer property. */
	template<typename T>
	void registProperty(T* valuePtr, const string& id)
	{
		mPropertiesList.push_back( mnew DataProperty<T>(valuePtr, id) );
	}

	/** Registering property property %). */
	template<typename _class, typename _type>
	void registProperty(const cProperty<_class, _type>& prop, const string& id)
	{
		mPropertiesList.push_back( mnew FuncProperty<_class, _type>(prop, id) );
	}

	/** Registering property by set/get functions. */
	template<typename _class, typename _type>
	void registProperty(_class* tclass, void* setterFunc, void* getterFunc, bool setterConst = true, bool getterConst = true)
	{
		mPropertiesList.push_back( mnew FuncProperty<_class, _type>(tclass, setterFunc, getterFunc, setterConst, getterConst) );
	}

public:
	/** Returns property by id. */
	template<typename T>
	shared< DataProperty<T> > getProperty(const string& id)
	{
		FOREACH(PropertiesVec, mPropertiesList, prop)
			if ((*prop)->mId == id)
				return *prop;

		return NULL;
	}
};

CLOSE_O2_NAMESPACE

#endif //PROPERTY_LIST_H
