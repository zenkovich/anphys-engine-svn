#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include "public.h"
#include "pugixml/pugixml.hpp"
#include "util/file_system/file.h"
#include "util/math/color.h"
#include "util/math/rect.h"
#include "util/math/vector2.h"

OPEN_O2_NAMESPACE

class cLogStream;

struct cSerializeType
{
	enum v { INPUT = 0, OUTPUT };
};

struct cSerializableObj
{
	virtual void serialize(pugi::xml_node& xmlNode, cSerializeType::v type, cLogStream* log = NULL) = 0;
};

struct cSerialization
{
	static void loadData(pugi::xml_document& doc, const std::string& fileName, 
		                 cFileType::value fileType = cFileType::FT_FILE, cLogStream* log = NULL);

	static void loadData(pugi::xml_document& doc, cInFile& file, cLogStream* log = NULL);

	static void saveData(pugi::xml_document& doc, const std::string& fileName, cFileType::value fileType = cFileType::FT_FILE);
	static void saveData(pugi::xml_document& doc, cOutFile& file);

	static void serializeIn(pugi::xml_node& xmlNode, const std::string& id, cSerializableObj* obj, cLogStream* log = NULL);	
	static void serializeIn(pugi::xml_node& xmlNode, const std::string& id, int& obj, cLogStream* log = NULL);	
	static void serializeIn(pugi::xml_node& xmlNode, const std::string& id, float& obj, cLogStream* log = NULL);	
	static void serializeIn(pugi::xml_node& xmlNode, const std::string& id, bool& obj, cLogStream* log = NULL);	
	static void serializeIn(pugi::xml_node& xmlNode, const std::string& id, std::string& obj, cLogStream* log = NULL);	
	static void serializeIn(pugi::xml_node& xmlNode, const std::string& id, vec2i& obj, cLogStream* log = NULL);	
	static void serializeIn(pugi::xml_node& xmlNode, const std::string& id, vec2f& obj, cLogStream* log = NULL);	
	static void serializeIn(pugi::xml_node& xmlNode, const std::string& id, fRect& obj, cLogStream* log = NULL);	
	static void serializeIn(pugi::xml_node& xmlNode, const std::string& id, iRect& obj, cLogStream* log = NULL);	
	static void serializeIn(pugi::xml_node& xmlNode, const std::string& id, color4& obj, cLogStream* log = NULL);
	
	static void serializeOut(pugi::xml_node& xmlNode, const std::string& id, cSerializableObj* obj, cLogStream* log = NULL);	
	static void serializeOut(pugi::xml_node& xmlNode, const std::string& id, const int obj, cLogStream* log = NULL);	
	static void serializeOut(pugi::xml_node& xmlNode, const std::string& id, const float obj, cLogStream* log = NULL);	
	static void serializeOut(pugi::xml_node& xmlNode, const std::string& id, const bool obj, cLogStream* log = NULL);	
	static void serializeOut(pugi::xml_node& xmlNode, const std::string& id, const std::string& obj, cLogStream* log = NULL);	
	static void serializeOut(pugi::xml_node& xmlNode, const std::string& id, const vec2i& obj, cLogStream* log = NULL);	
	static void serializeOut(pugi::xml_node& xmlNode, const std::string& id, const vec2f& obj, cLogStream* log = NULL);	
	static void serializeOut(pugi::xml_node& xmlNode, const std::string& id, const fRect& obj, cLogStream* log = NULL);	
	static void serializeOut(pugi::xml_node& xmlNode, const std::string& id, const iRect& obj, cLogStream* log = NULL);	
	static void serializeOut(pugi::xml_node& xmlNode, const std::string& id, const color4& obj, cLogStream* log = NULL);

	
	template<typename T>
	static void serializeArrIn(pugi::xml_node& xmlNode, const std::string& id, T* arr, int count, cLogStream* log = NULL)
	{
		pugi::xml_node node = xmlNode.child(id.c_str());
		for (int i = 0; i < count; i++)
		{
			char vId[32]; sprintf(vId, "v%i", i);
			std::string sv = vId;
			serializeIn(node, sv, arr[i]);
		}
	}
	
	template<typename T>
	static void serializeArrOut(pugi::xml_node& xmlNode, const std::string& id, const T* arr, int count, cLogStream* log = NULL)
	{
		pugi::xml_node node = xmlNode.append_child(id.c_str());
		for (int i = 0; i < count; i++)
		{
			char vId[32]; sprintf(vId, "v%i", i);
			serializeOut(node, vId, arr[i]);
		}
	}

protected:
	static bool getNode(pugi::xml_node& node, const pugi::xml_node& parent, const std::string& id, cLogStream* log);
};

#define SERIALIZE_METHOD_DECL() \
	virtual void serialize(pugi::xml_node& xmlNode, cSerializeType::v type, cLogStream* log = NULL)

#define SERIALIZE_METHOD_IMPL(CLASS) \
	void CLASS::serialize(pugi::xml_node& xmlNode, cSerializeType::v type, cLogStream* log)

#define SERIALIZE_INHERITED_METHOD_DECL(BASIC_CLASS, CLASS)                                             \
	virtual void serializeInh(pugi::xml_node& xmlNode, cSerializeType::v type, cLogStream* log = NULL); \
	virtual void serialize(pugi::xml_node& xmlNode, cSerializeType::v type, cLogStream* log = NULL)     \
	{                                                                                                   \
		pugi::xml_node basicClassNode = xmlNode.append_child(#BASIC_CLASS);                             \
		BASIC_CLASS::serialize(basicClassNode, type, log);                                              \
		CLASS::serializeInh(xmlNode, type, log);                                                        \
	}

#define SERIALIZE_INHERITED_METHOD_IMPL(CLASS) \
	void CLASS::serializeInh(pugi::xml_node& xmlNode, cSerializeType::v type, cLogStream* log)       

#define SERIALIZE_ID(obj, id) \
	if (type == cSerializeType::INPUT) cSerialization::serializeIn(xmlNode, id, obj, log); \
	else                               cSerialization::serializeOut(xmlNode, id, obj, log); 

#define SERIALIZE(obj) SERIALIZE_ID(obj, #obj)      

#define SERIALIZE_ARR_ID(obj, id, count) \
	std::string cid = id; \
	if (type == cSerializeType::INPUT) cSerialization::serializeArrIn(xmlNode, cid, obj, count, log); \
	else                               cSerialization::serializeArrOut(xmlNode, cid, obj, count, log); 

#define SERIALIZE_ARR(obj, count) SERIALIZE_ARR_ID(obj, #obj, count)


CLOSE_O2_NAMESPACE

#endif //SERIALIZATION_H