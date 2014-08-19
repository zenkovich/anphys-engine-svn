#ifndef SERIALIZE_UTIL_H
#define SERIALIZE_UTIL_H

#include "public.h"
#include "xml_tools.h"
#include "callback.h"
#include "time_utils.h"

OPEN_O2_NAMESPACE

class cLogStream;
class cSerializer;

/** Serializable object interface. */
class cSerializable 
{
public:
	cCallbackChain onBeginSerialize; /** Calls when serialization begins. */
	cCallbackChain onDeserialized;   /** Calls when serialization complete. */

	/** Serialization function. */
	virtual bool serialize(cSerializer* serializer) = 0; 
};

/** Basic serialize object, need to serialize data structures. */
class cSerializer
{
public:
	enum SerializeType { ST_SERIALIZE = 0, ST_DESERIALIZE };

private:
	cLogStream*        mLog;         /** Serialization log, where puts errors. */
	pugi::xml_document mRootNode;    /** Root srializtion xml document. */
	pugi::xml_node     mCurrentNode; /** Current xml node. */
	SerializeType      mType;        /** Serialization type. */

public:
	/** ctor. */
	cSerializer(SerializeType type = cSerializer::ST_SERIALIZE);

	/** ctor. Loading file and setting deserialize type. */
	cSerializer(const string& fileName, SerializeType type = cSerializer::ST_SERIALIZE);

	/** ctor. Getting xml node and setting type. */
	cSerializer(pugi::xml_node& xmlNode, SerializeType type = cSerializer::ST_SERIALIZE);

	/** dtor. */
	~cSerializer();

	/** Returns serialization type. */
	SerializeType getType() const;

	/** Loading file, sets type deserialization and returns a flag of successful execution. */
	bool load(const string& file);

	/** Saving data to file. */
	bool save(const string& file);

	/** Loads data from file and returns a flag of successful execution. */
	bool loadFromString(const string& data);

	/** Saving data to string. */
	string saveToString();

	/** Sets log. If log not setted - using global log. */
	void setLog(cLogStream* logStream);

	/** Creates inherited node. */
	void createNode(const string& id);

	/** Trying to get inherited node. */
	bool getNode(const string& id, bool errors = false);

	/** Returns to parent node. */
	void popNode();
	
	/** Serialize object. */
	template<typename T>
	bool serialize(T& object, const string& id, bool errors = true)
	{
		if (mType == ST_SERIALIZE)
		{
			createNode(id);
			cXmlTools::toXmlNode(object, mCurrentNode);
			popNode();
			return true;
		}
		else
		{
			if (!getNode(id, errors))
				return false;

			cXmlTools::fromXmlNode(object, mCurrentNode);
			popNode();
		}
			
		return true;
	}

	/** Serialize object. */
	bool serialize(cSerializable& object, const string& id, bool errors = true)
	{
		if (mType == ST_SERIALIZE)
		{
			createNode(id);
			object.onBeginSerialize.call();
			object.serialize(this);
			popNode();
			return true;
		}
		else
		{
			if (!getNode(id, errors))
				return false;
			
			object.serialize(this);
			object.onDeserialized.call();
			popNode();
		}
			
		return true;
	}
};

/** Declare serialization method. */
#define SERIALIZE_METHOD_DECL() \
	virtual bool serialize(cSerializer* serializer)

/** Implementation of serialize method. You must define class. */
#define SERIALIZE_METHOD_IMPL(CLASS) \
	bool CLASS::serialize(cSerializer* serializer)

/** Declare serialize method for inherited class. You must define parent and target class. */
#define SERIALIZE_INHERITED_METHOD_DECL(BASIC_CLASS)            \
	virtual bool serializeInh(cSerializer* serializer);          \
	virtual bool serialize(cSerializer* serializer)              \
	{                               							 \
		if (serializer->getType() == cSerializer::ST_SERIALIZE)	 \
			serializer->createNode(#BASIC_CLASS);				 \
		else													 \
			serializer->getNode(#BASIC_CLASS, true);			 \
																 \
		if (!BASIC_CLASS::serialize(serializer)) 				 \
		{														 \
			serializer->popNode();								 \
			return false;										 \
		}														 \
																 \
		serializer->popNode();									 \
																 \
		return serializeInh(serializer);						 \
	}															 \

/** Implementation of serialize method for inherited class. */
#define SERIALIZE_INHERITED_METHOD_IMPL(CLASS) \
	bool CLASS::serializeInh(cSerializer* serializer)       

/** Serialize object with ID. */
#define SERIALIZE_ID(obj, id) \
	serializer->serialize(obj, id);

/** Serialize @obj with name "obj". */
#define SERIALIZE(obj) \
	SERIALIZE_ID(obj, #obj)      

// /** Serialize array of object with ID. */
// #define SERIALIZE_ARR_ID(obj, count, id) \
// 	cSerializer::serializeArr(xmlNode, id, obj, count, type, log)
// 
// /** Serialize array of objects with name "obj". */
// #define SERIALIZE_ARR(obj, count) \
// 	SERIALIZE_ARR_ID(obj, count, #obj)

CLOSE_O2_NAMESPACE

#endif //SERIALIZE_UTIL_H
