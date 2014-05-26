#ifndef PROPERTY_LIST_H
#define PROPERTY_LIST_H

#include "public.h"

#include "util/callback.h"

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
