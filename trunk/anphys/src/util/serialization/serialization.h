#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include "data_object.h"
#include "util/other/string_utils.h"

#define autoSerialize(obj)                  serializeType(dataArchieve, achieveType, obj, #obj)
										   
#define serializeId(obj, id)                serializeType(dataArchieve, achieveType, obj, id)
										   
#define autoSerializeObj(obj)               (obj).serialize(dataArchieve, achieveType, #obj)
										   
#define serializeObjId(obj, id)             (obj).serialize(dataArchieve, achieveType, id)
										   
#define autoSerializeArr(obj, size)         serializeTypeArr(dataArchieve, achieveType, obj, size, #obj)
										   
#define serializeArrId(obj, size, id)       serializeTypeArr(dataArchieve, achieveType, obj, size, id)
										   
#define autoSerializeObjArr(obj, size)      serializeObjArr(dataArchieve, achieveType, obj, size, #obj)
										   
#define serializeObjArrId(obj, size, id)    serializeObjArr(dataArchieve, achieveType, obj, size, id)

#define autoSerializeObjPtrArr(obj, size)   serializeObjPtrArr(dataArchieve, achieveType, obj, size, #obj)

#define serializeObjPtrArrId(obj, size, id) serializeObjPtrArr(dataArchieve, achieveType, obj, size, id)


#define serializeMethodDecl(obj) \
virtual bool serialize(cDataObject& dataArchieve, ArchieveType achieveType, const std::string& id) \
{                                                                                   \
	printf("STD SERIALIZE %s\n", id.c_str());                                       \
	cDataObject* currentAchieveObject;                                              \
	if (achieveType == AT_OUTPUT) currentAchieveObject = dataArchieve.addChild(id); \
	else                          currentAchieveObject = dataArchieve.getChild(id); \
	if (!currentAchieveObject) return false;                                        \
	return obj::serialize_(*currentAchieveObject, achieveType);                     \
}                                                                                   \
virtual bool serialize_(cDataObject& dataArchieve, ArchieveType achieveType)

#define serializeMethodImpl(obj) \
bool obj::serialize_(cDataObject& dataArchieve, ArchieveType achieveType)

#define derivedSerializeMethodDecl(basicObject, obj) \
virtual bool serialize(cDataObject& dataArchieve, ArchieveType achieveType, const std::string& id) \
{                                                                                    \
	printf("DERIVED SERIALIZE %s\n", id.c_str());                                    \
	cDataObject* currentAchieveObject;                                               \
	if (achieveType == AT_OUTPUT) currentAchieveObject = dataArchieve.addChild(id);  \
	else                          currentAchieveObject = dataArchieve.getChild(id);  \
	if (!currentAchieveObject) return false;                                         \
	if (!basicObject::serialize((*currentAchieveObject), achieveType, #basicObject)) \
		return false;                                                                \
	return obj::serialize_(*currentAchieveObject, achieveType);                      \
}                                                                                    \
virtual bool serialize_(cDataObject& dataArchieve, ArchieveType achieveType)


void testDataObject();

#endif //SERIALIZATION_H