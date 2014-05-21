#ifndef PROPERTY_LIST_H
#define PROPERTY_LIST_H

#include "public.h"

OPEN_O2_NAMESPACE

class cObjectWithPropertyList
{
public:
	struct Property
	{
		enum Type { TP_INT = 0, TP_FLOAT, TP_STRING, TP_VEC2 };

		void*  mObjectPtr;
		string mId;
		Type   mType;

		Property(void* ptr, const string& id, Property::Type type);

		template<typename T>
		T* ptr() const
		{
			return (T*)mObjectPtr;
		}
	};
	typedef vector<Property> PropertiesVec;

protected:
	PropertiesVec mPropertiesList;

	virtual void initializePropertiesList() = 0;
	void registProperty(void* ptr, const string& id, Property::Type type);
	void registProperty(int& value, const string& id);
	void registProperty(float& value, const string& id);
	void registProperty(string& value, const string& id);
	void registProperty(vec2f& value, const string& id);


public:
	Property getProperty(const string& id);
};

CLOSE_O2_NAMESPACE

#endif //PROPERTY_LIST_H
