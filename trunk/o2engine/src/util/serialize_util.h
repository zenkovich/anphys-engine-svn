#ifndef SERIALIZE_UTIL_H
#define SERIALIZE_UTIL_H

#include <map>

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
	friend class cSerializer;

public:
	cCallbackChain onBeginSerializeEvent; /** Calls when serialization begins. */
	cCallbackChain onDeserializedEvent;   /** Calls when serialization complete. */

	/** Serialization function. */
	virtual bool serialize(cSerializer* serializer) = 0; 
	virtual cSerializable* createSample() const = 0;
	virtual string getTypeName() const = 0;

protected:
	virtual void onBeginSerialize() {}
	virtual void onDeserialized() {}
};

struct gSerializeTypesContainer
{
	typedef std::map<string, cSerializable*> SamplesMap;

	static SamplesMap mSamples;
	static void regType(cSerializable* type);
	static cSerializable* getSample(const string& typeName);
	static void outputRegisteredSamples();
};

template<typename T>
struct gSerializeTypesInitializer
{
	gSerializeTypesInitializer()
	{
		T* sample = new T();
		gSerializeTypesContainer::regType(sample);
	}
};

/** Basic serialize object, need to serialize data structures. */
class cSerializer
{
public:
	enum SerializeType { ST_SERIALIZE = 0, ST_DESERIALIZE };

private:
	cLogStream*        mLog;         /** Serialization log, where puts errors. */
	pugi::xml_document mRootNode;    /** Root serialization xml document. */
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
	bool serialize(cSerializable* object, const string& id, bool errors = true);

	/** Serialize object. */
	bool serialize(int& object, const string& id, bool errors = true);

	/** Serialize object. */
	bool serialize(unsigned int& object, const string& id, bool errors = true);

	/** Serialize object. */
	bool serialize(float& object, const string& id, bool errors = true);

	/** Serialize object. */
	bool serialize(bool& object, const string& id, bool errors = true);

	/** Serialize object. */
	bool serialize(string& object, const string& id, bool errors = true);

	/** Serialize object. */
	bool serialize(vec2f& object, const string& id, bool errors = true);

	/** Serialize object. */
	bool serialize(fRect& object, const string& id, bool errors = true);

	/** Serialize object. */
	bool serialize(vec2i& object, const string& id, bool errors = true);

	/** Serialize object. */
	bool serialize(iRect& object, const string& id, bool errors = true);

	/** Serialize object. */
	bool serialize(color4& object, const string& id, bool errors = true);

	/** Serialize object. */
	bool serialize(WideTime& object, const string& id, bool errors = true);
	
	/** Serialize object. */
	template<typename T>
	bool serializeTemp(T& object, const string& id, bool errors = true)
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
	
	/** Saving data from object to xml node. */
	template<typename T>
	bool serialize(vector<T>& array, const string& id, bool errors = true)
	{
		if (mType == ST_SERIALIZE)
		{
			createNode(id);

			mCurrentNode.append_attribute("count") = array.size();
			for (int i = 0; i < (int)array.size(); i++)
			{
				char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
				T* arrElem = &(array[i]);
				serialize(arrElem, elemNodeName, errors);
			}

			popNode();
			return true;
		}
		else
		{
			if (!getNode(id, errors))
				return false;
			
			array.clear();
			int srCount = mCurrentNode.attribute("count").as_int();
			for (int i = 0; i < srCount; i++)
			{
				char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
				array.push_back(T());
				T* elemPtr = &array.back();
				serialize(elemPtr, elemNodeName, errors);
			}

			popNode();
		}
			
		return true;
	}
	
	/** Saving data from object to xml node. */
	template<typename T>
	bool serialize(vector<T*>& array, const string& id, bool errors = true)
	{
		if (mType == ST_SERIALIZE)
		{
			createNode(id);

			mCurrentNode.append_attribute("count") = array.size();
			for (int i = 0; i < (int)array.size(); i++)
			{
				char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
				T* elem = array[i];
				serialize(elem, elemNodeName, errors);
			}

			popNode();
			return true;
		}
		else
		{
			if (!getNode(id, errors))
				return false;
			
			array.clear();
			int srCount = mCurrentNode.attribute("count").as_int();
			for (int i = 0; i < srCount; i++)
			{
				char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
				string type = mCurrentNode.child(elemNodeName).attribute("type").value();
				T* elem = static_cast<T*>(createSerializableSample(type));
				serialize(elem, elemNodeName, errors);
				array.push_back(elem);
			}

			popNode();
		}
			
		return true;
	}
	
	/** Saving data from object to xml node. */
	template<typename T>
	bool serialize(array<T>& arr, const string& id, bool errors = true)
	{
		if (mType == ST_SERIALIZE)
		{
			createNode(id);

			mCurrentNode.append_attribute("count") = arr.count();
			for (int i = 0; i < (int)arr.count(); i++)
			{
				char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
				T* arrElem = &(arr[i]);
				serialize(arrElem, elemNodeName, errors);
			}

			popNode();
			return true;
		}
		else
		{
			if (!getNode(id, errors))
				return false;
			
			arr.clear();
			int srCount = mCurrentNode.attribute("count").as_int();
			for (int i = 0; i < srCount; i++)
			{
				char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
				arr.add(T());
				T* elemPtr = &arr.last();
				serialize(elemPtr, elemNodeName, errors);
			}

			popNode();
		}
			
		return true;
	}
	
	/** Saving data from object to xml node. */
	template<typename T>
	bool serialize(array<T*>& arr, const string& id, bool errors = true)
	{
		if (mType == ST_SERIALIZE)
		{
			createNode(id);

			mCurrentNode.append_attribute("count") = arr.count();
			for (int i = 0; i < (int)arr.count(); i++)
			{
				char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
				T* elem = arr[i];
				serialize(elem, elemNodeName, errors);
			}

			popNode();
			return true;
		}
		else
		{
			if (!getNode(id, errors))
				return false;
			
			arr.clear();
			int srCount = mCurrentNode.attribute("count").as_int();
			for (int i = 0; i < srCount; i++)
			{
				char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
				string type = mCurrentNode.child(elemNodeName).attribute("type").value();
				T* elem = static_cast<T*>(createSerializableSample(type));
				serialize(elem, elemNodeName, errors);
				arr.add(elem);
			}

			popNode();
		}
			
		return true;
	}

private:
	static cSerializable* createSerializableSample(const string& type);
};

/** Implementation of serialize method. You must define class. */
#define SERIALIZE_METHOD_IMPL(CLASS)                  \
	gSerializeTypesInitializer<CLASS> CLASS::RegType; \
	bool CLASS::serialize(cSerializer* serializer)

/** Implementation of serialize method for inherited class. */
#define SERIALIZE_INHERITED_METHOD_IMPL(CLASS)        \
	gSerializeTypesInitializer<CLASS> CLASS::RegType; \
	bool CLASS::serializeInh(cSerializer* serializer)  

/** Declaration of serialize methods. */
#define SERIALIZBLE_METHODS(CLASS)                                       \
	static gSerializeTypesInitializer<CLASS> RegType;                    \
	virtual cSerializable* createSample() const { return mnew CLASS(); } \
	virtual string getTypeName() const { return #CLASS; }                \
	bool CLASS::serialize(cSerializer* serializer)

/** Declaration of inherited serialize methods. */
#define SERIALIZBLE_INHERITED_METHODS(CLASS, BASIC_CLASS)                \
	static gSerializeTypesInitializer<CLASS> RegType;           \
	virtual cSerializable* createSample() const { return mnew CLASS(); } \
	virtual string getTypeName() const { return #CLASS; }                \
	virtual bool serializeInh(cSerializer* serializer);                  \
	virtual bool serialize(cSerializer* serializer)                      \
	{                               							         \
		if (serializer->getType() == cSerializer::ST_SERIALIZE)	         \
			serializer->createNode(#BASIC_CLASS);				         \
		else													         \
			serializer->getNode(#BASIC_CLASS, true);			         \
																         \
		if (!BASIC_CLASS::serialize(serializer)) 				         \
		{														         \
			serializer->popNode();								         \
			return false;										         \
		}														         \
																         \
		serializer->popNode();									         \
																         \
		return serializeInh(serializer);						         \
	}															         

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
