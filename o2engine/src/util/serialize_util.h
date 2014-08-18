#ifndef SERIALIZE_UTIL_H
#define SERIALIZE_UTIL_H

#include "public.h"
#include "xml_tools.h"
#include "callback.h"
#include "time_utils.h"

OPEN_O2_NAMESPACE

class cLogStream;
class cSerializer;

class cSerializable 
{
public:
	cCallbackChain onBeginSerialize;
	cCallbackChain onDeserialized;

	virtual bool serialize(cSerializer* serializer) = 0;
};

class cSerializer
{
public:
	enum SerializeType { ST_SERIALIZE = 0, ST_DESERIALIZE };

private:
	cLogStream*        mLog;
	pugi::xml_document mRootNode;
	pugi::xml_node     mCurrentNode;
	SerializeType      mType;

public:
	cSerializer(SerializeType type = cSerializer::ST_SERIALIZE);
	cSerializer(const string& fileName, SerializeType type = cSerializer::ST_SERIALIZE);
	~cSerializer();

	bool load(const string& file);
	bool save(const string& file);
	bool loadFromString(const string& data);
	string saveToString();

	void setLog(cLogStream* logStream);

	void createNode(const string& id);
	bool getNode(const string& id, bool errors = false);
	void popNode();
	
	template<typename T>
	void serialize(T& object, const string& id)
	{
		createNode(id);
		cXmlTools::toXmlNode(object, mCurrentNode);
		popNode();
	}
	
	template<typename T>
	bool deserialize(T& object, const string& id, bool errors = false)
	{
		if (!getNode(id, errors))
			return false;

		cXmlTools::fromXmlNode(object, mCurrentNode);
		popNode();
	}

	/*template<typename T>
	void serializeArr(T* array, int count, const string& id)
	{
	}

	template<typename T>
	void serializeArr(vector<T>& array, const string& id)
	{
	}

	template<typename T>
	void serializeArr(vector<T*>& array, const string& id)
	{
	}

	template<typename T>
	bool deserializeArr(T* array, const string& id)
	{
	}

	template<typename T>
	bool deserializeArr(vector<T>& array, const string& id)
	{
	}

	template<typename T>
	bool deserializeArr(vector<T*>& array, const string& id)
	{
	}*/
};

/** Declare serialization method. */
#define _SERIALIZE_METHOD_DECL() \
	virtual bool serialize(cSerializer* serializer)

/** Implementation of serialize method. You must define class. */
#define _SERIALIZE_METHOD_IMPL(CLASS) \
	bool CLASS::serialize(cSerializer* serializer)

/** Declare serialize method for inherited class. You must define parent and target class. */
#define _SERIALIZE_INHERITED_METHOD_DECL(BASIC_CLASS, CLASS)                                                      \
	virtual bool serializeInh(cSerializer* serializer);                                                           \
	virtual bool serialize(cSerializer* serializer)                                                               \
	{                                                                                                             \
	    pugi::xml_node basicClassNode =                                                                           \
			(type == cSerializeType::INPUT) ? (xmlNode.child(#BASIC_CLASS)):(xmlNode.append_child(#BASIC_CLASS)); \
		if (!BASIC_CLASS::serialize(basicClassNode, type, log)) return false;                                     \
		return CLASS::serializeInh(xmlNode, type, log);                                                           \
	}

/** Implementation of serialize method for inherited class. */
#define _SERIALIZE_INHERITED_METHOD_IMPL(CLASS) \
	bool CLASS::serializeInh(pugi::xml_node& xmlNode, cSerializeType::v type, cLogStream* log)       

/** Serialize object with ID. */
#define _SERIALIZE_ID(obj, id) cSerialization::serialize(xmlNode, id, obj, type, log)

/** Serialize @obj with name "obj". */
#define _SERIALIZE(obj) SERIALIZE_ID(obj, #obj)      

/** Serialize array of object with ID. */
#define _SERIALIZE_ARR_ID(obj, count, id) cSerialization::serializeArr(xmlNode, id, obj, count, type, log)

/** Serialize array of objects with name "obj". */
#define _SERIALIZE_ARR(obj, count) SERIALIZE_ARR_ID(obj, count, #obj)

CLOSE_O2_NAMESPACE

#endif //SERIALIZE_UTIL_H
