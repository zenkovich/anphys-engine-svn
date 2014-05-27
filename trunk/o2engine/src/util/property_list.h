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
	/** Property object. Contains pointer to some value, type of value, id and callback on changing value. */
	struct Property
	{
		enum Type { TP_INT = 0, TP_FLOAT, TP_STRING, TP_VEC2 };

		void*          mObjectPtr; /** Object value pointer. */
		string         mId;        /** Identificator. */
		Type           mType;      /** Value type. */
		cCallbackChain mOnChange;  /** On change callbacks. */

		/** ctor. */
		Property(void* ptr, const string& id, Property::Type type);

		/** Returns casted pointer. */
		template<typename T>
		T* ptr() const
		{
			return (T*)mObjectPtr;
		}
	};

	struct IProperty
	{
		string mId;
		cCallbackChain mOnChange;

		DEFINE_TYPE(IProperty);

		IProperty(const string& id):mId(id) {}
	};

	template<typename T>
	struct DataProperty: public IProperty
	{
		T  mLastDataValue;
		T* mDataPtr;

		DEFINE_TEMPLATE_TYPE(DataProperty, T);

		DataProperty(T* dataPtr, const string& id):
			IProperty(id)
		{ 
			mDataPtr = dataPtr;
			mLastDataValue = *dataPtr;
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



	typedef vector<Property> PropertiesVec;

protected:
	PropertiesVec mPropertiesList;

	/** Initialization properties func. Must be implemented. */
	virtual void initializePropertiesList() = 0;

	/** Registering property in list. */
	void registProperty(void* ptr, const string& id, Property::Type type);

	/** Registering int property in list. */
	void registProperty(int& value, const string& id);

	/** Registering float property in list. */
	void registProperty(float& value, const string& id);

	/** Registering string property in list. */
	void registProperty(string& value, const string& id);

	/** Registering vector property in list. */
	void registProperty(vec2f& value, const string& id);


public:
	/** Returns property by id. */
	shared<Property> getProperty(const string& id);
};

CLOSE_O2_NAMESPACE

#endif //PROPERTY_LIST_H
