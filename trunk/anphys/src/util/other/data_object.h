#ifndef DATA_OBJECT_H
#define DATA_OBJECT_H

#include "../math/vec2.h"
#include "../math/vector.h"

#include <string>

struct cDataObject
{
	enum DataType { DT_BYTES = 0, DT_STRING, DT_INTEGER, DT_FLOAT, DT_BOOL, DT_VECTOR2, DT_VECTOR3 };

	char*			mData;
	unsigned int	mDataSize;
	DataType		mDataType;

	cDataObject();
	cDataObject(const char* string);
	cDataObject(int value);
	cDataObject(float value);
	cDataObject(bool value);
	cDataObject(const vec2& value);
	cDataObject(const vec3& value);
	
	cDataObject(const char* string, DataType dataType);
	
	void operator=(const char* string);
	void operator=(int value);
	void operator=(float value);
	void operator=(bool value);
	void operator=(const vec2& value);
	void operator=(const vec3& value);

	std::string getString();
	int			getInteger();
	float		getFloat();
	bool		getBool();
	vec2		getVector2();
	vec3		getVector3();

	DataType getType() { return mDataType; }
};

#endif //DATA_OBJECT_H