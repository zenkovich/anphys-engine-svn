#include "data_object.h"

#include <string>
#include "../memory/mem_utils.h"
#include "../other/string_utils.h"


cDataObject::cDataObject(): mData(NULL), mDataSize(0) {}

cDataObject::cDataObject(const char* string)
{
	unsigned int length = strlen(string);
	mData = new char[length];
	strcpy(mData, string);
	mDataSize = length;
	mDataType = DT_STRING;
}

cDataObject::cDataObject(int value)
{
	mData = new char[sizeof(int)/sizeof(char)];
	mDataSize = sizeof(int);
	memcpy(mData, &value, mDataSize);
	mDataType = DT_INTEGER;
}

cDataObject::cDataObject(float value)
{
	mData = new char[sizeof(float)/sizeof(char)];
	mDataSize = sizeof(float);
	memcpy(mData, &value, mDataSize);
	mDataType = DT_FLOAT;
}

cDataObject::cDataObject(bool value)
{
	mData = new char[sizeof(int)/sizeof(char)];
	mDataSize = sizeof(int);
	memcpy(mData, &value, mDataSize);
	mDataType = DT_INTEGER;
}

cDataObject::cDataObject(const char* string, DataType dataType)
{
	if (dataType == DT_STRING) *this = string;
	if (dataType == DT_BOOL)
	{
		if (strcmp(string, "true") == 0 ||
			strcmp(string, "TRUE") == 0 ||
			strcmp(string, "True") == 0 ||
			strcmp(string, "yes") == 0 ||
			strcmp(string, "YES") == 0 ||
			strcmp(string, "1") == 0 ||
			strcmp(string, "fuckyeah") == 0) *this = true;
		else *this = false;
	}
	if (dataType == DT_INTEGER)
	{
		*this = atoi(string);
	}
	if (dataType == DT_FLOAT)
	{
		*this = (float)atof(string);
	}
	if (dataType == DT_VECTOR2)
	{
		*this = StrToVec2(string);
	}
	if (dataType == DT_VECTOR3)
	{
		*this = StrToVec3(string);
	}
}

void cDataObject::operator=(const char* string)
{
	unsigned int length = strlen(string);
	mData = new char[length];
	strcpy(mData, string);
	mDataSize = length;
	mDataType = DT_STRING;
}

void cDataObject::operator=(int value)
{
	mData = new char[sizeof(int)/sizeof(char)];
	mDataSize = sizeof(int);
	memcpy(mData, &value, mDataSize);
	mDataType = DT_INTEGER;
}

void cDataObject::operator=(float value)
{
	mData = new char[sizeof(float)/sizeof(char)];
	mDataSize = sizeof(float);
	memcpy(mData, &value, mDataSize);
	mDataType = DT_FLOAT;
}

void cDataObject::operator=(bool value)
{
	mData = new char[sizeof(int)/sizeof(char)];
	mDataSize = sizeof(int);
	memcpy(mData, &value, mDataSize);
	mDataType = DT_INTEGER;
}

void cDataObject::operator=(const vec2& value)
{
	mData = new char[sizeof(vec2)/sizeof(char)];
	mDataSize = sizeof(vec2);
	memcpy(mData, &value, mDataSize);
	mDataType = DT_VECTOR2;
}

void cDataObject::operator=(const vec3& value)
{
	mData = new char[sizeof(vec3)/sizeof(char)];
	mDataSize = sizeof(vec3);
	memcpy(mData, &value, mDataSize);
	mDataType = DT_VECTOR3;
}

std::string cDataObject::getString()
{
	if (mDataType == DT_BOOL) { if ((bool)(*mData)) return "true"; return "false"; }
	if (mDataType == DT_INTEGER) { char str[16]; itoa((int)(*mData), str, 10); return str; }
	if (mDataType == DT_FLOAT) { char str[16]; sprintf(str, "%f", (float)(*mData)); return str; }
	if (mDataType == DT_VECTOR2) 
	{ 
		char str[32]; 
		vec2 conv; memcpy(&conv, mData, sizeof(vec2));
		sprintf(str, "%f; %f", conv.x, conv.y); 
		return str; 
	}
	if (mDataType == DT_VECTOR3) 
	{ 
		char str[48]; 
		vec3 conv; memcpy(&conv, mData, sizeof(vec3));
		sprintf(str, "%f; %f; %f", conv.x, conv.y, conv.z); 
		return str; 
	}

	return mData;
}

int	cDataObject::getInteger()
{
	if (mDataType == DT_BOOL) { if ((bool)(*mData)) return 1; return 0; }
	if (mDataType == DT_INTEGER) { return (int)(*mData); }
	if (mDataType == DT_FLOAT) { return (int)( (float)(*mData) ); }
	if (mDataType == DT_VECTOR2) 
	{ 
		vec2 conv; memcpy(&conv, mData, sizeof(vec2));
		return (int)conv.len();
	}
	if (mDataType == DT_VECTOR3) 
	{ 
		vec3 conv; memcpy(&conv, mData, sizeof(vec3));
		return (int)conv.len();
	}
	if (mDataType == DT_STRING)
	{
		return atoi(mData);
	}

	return 0;
}

float cDataObject::getFloat()
{
	if (mDataType == DT_BOOL) { if ((bool)(*mData)) return 1.0f; return 0.0f; }
	if (mDataType == DT_INTEGER) { return (float)( (int)(*mData) ); }
	if (mDataType == DT_FLOAT) { return (float)(*mData); }
	if (mDataType == DT_VECTOR2) 
	{ 
		vec2 conv; memcpy(&conv, mData, sizeof(vec2));
		return (float)conv.len();
	}
	if (mDataType == DT_VECTOR3) 
	{ 
		vec3 conv; memcpy(&conv, mData, sizeof(vec3));
		return (float)conv.len();
	}
	if (mDataType == DT_STRING)
	{
		return (float)atof(mData);
	}

	return 0.0f;
}

bool cDataObject::getBool()
{
	if (mDataType == DT_BOOL) { return (bool)(*mData); }
	if (mDataType == DT_INTEGER) { return (bool)( (int)(*mData) ); }
	if (mDataType == DT_FLOAT) { return (bool)( (float)(*mData) ); }
	if (mDataType == DT_VECTOR2 || mDataType == DT_VECTOR3) 
	{ 
		return false;
	}
	if (mDataType == DT_STRING)
	{
		if (strcmp(mData, "true") == 0 ||
			strcmp(mData, "TRUE") == 0 ||
			strcmp(mData, "yes") == 0 ||
			strcmp(mData, "YES") == 0 ||
			strcmp(mData, "1") == 0 ||
			strcmp(mData, "fuckyeah") == 0) return true;
		return false;
	}

	return false;
}

vec2 cDataObject::getVector2()
{
	if (mDataType == DT_BOOL) { return vec2(0); }
	if (mDataType == DT_INTEGER) { return vec2( (float)( (int)(*mData) ) ); }
	if (mDataType == DT_FLOAT) { return vec2( (float)(*mData) ); }
	if (mDataType == DT_VECTOR2) 
	{ 
		vec2 conv; memcpy(&conv, mData, sizeof(vec2));
		return conv;
	}
	if (mDataType == DT_VECTOR3) 
	{ 
		vec3 conv; memcpy(&conv, mData, sizeof(vec3));
		return vec2(conv.x, conv.y);
	}
	if (mDataType == DT_STRING)
	{
		return StrToVec2(mData);
	}

	return vec2(0);
}

vec3 cDataObject::getVector3()
{
	if (mDataType == DT_BOOL) { return vec3(0); }
	if (mDataType == DT_INTEGER) { return vec3( (float)( (int)(*mData) ) ); }
	if (mDataType == DT_FLOAT) { return vec3( (float)(*mData) ); }
	if (mDataType == DT_VECTOR2) 
	{ 
		vec2 conv; memcpy(&conv, mData, sizeof(vec2));
		return vec3(conv.x, conv.y, 0);
	}
	if (mDataType == DT_VECTOR3) 
	{ 
		vec3 conv; memcpy(&conv, mData, sizeof(vec3));
		return conv;
	}
	if (mDataType == DT_STRING)
	{
		return StrToVec3(mData);
	}

	return vec3(0);
}

