#include "serialize_util.h"
#include <map>

OPEN_O2_NAMESPACE

Serializer::Serializer(SerializeType type /*= cSerializer::ST_SERIALIZE*/)
{
	mCurrentNode = mRootNode;
	mLog = gLog;
	mType = type;
}

Serializer::Serializer(const string& fileName, SerializeType type /*= ST_SERIALIZE*/)
{
	mCurrentNode = mRootNode;
	mLog = gLog;
	load(fileName);
}

Serializer::Serializer(pugi::xml_node& xmlNode, SerializeType type /*= cSerializer::ST_SERIALIZE*/)
{
	mCurrentNode = mRootNode.append_copy(xmlNode);
	mType = type;
}

Serializer::~Serializer()
{
}

bool Serializer::load(const string& file, bool stdConfigExt /*= true*/)
{
	if (!XmlTools::loadFromFile(file, mRootNode, stdConfigExt))
		return false;

	mCurrentNode = mRootNode;
	mType = ST_DESERIALIZE;

	return true;
}

bool Serializer::save(const string& file, bool stdConfigExt /*= true*/)
{	
	return XmlTools::saveToFile(file, mRootNode, stdConfigExt);
}

bool Serializer::loadFromString(const string& data)
{
	return XmlTools::loadFromString(data, mRootNode);
}

string Serializer::saveToString()
{
	string data;
	XmlTools::saveToString(data, mRootNode);
	return data;
}

void Serializer::setLog(LogStream* logStream)
{
	mLog = logStream;
}

void Serializer::createNode(const string& id)
{
	mCurrentNode = mCurrentNode.append_child(id.c_str());
}

bool Serializer::getNode(const string& id, bool errors /*= false*/)
{
	pugi::xml_node node = mCurrentNode.child(id.c_str());
	if (!node)
	{
		if (errors)
			mLog->error("Failed to get serialize node: %s", id.c_str());

		return false;
	}

	mCurrentNode = node;
	return true;
}

void Serializer::popNode()
{
	mCurrentNode = mCurrentNode.parent();
}

Serializer::SerializeType Serializer::getType() const
{
	return mType;
}

bool Serializer::serialize(Serializable* object, const string& id, bool errors /*= true*/)
{
	if (mType == ST_SERIALIZE)
	{
		createNode(id);
		object->onBeginSerialize();
		object->onBeginSerializeEvent.call();
		mCurrentNode.append_attribute("type") = object->getTypeName().c_str();
		object->serialize(this);
		popNode();
		return true;
	}
	else
	{
		if (!getNode(id, errors))
			return false;

		object->serialize(this);
		object->onDeserialized();
		object->onDeserializedEvent.call();
		popNode();
	}

	return true;
}

bool Serializer::serialize(int& object, const string& id, bool errors /*= true*/)
{
	return serializeTemp(object, id, errors);
}

bool Serializer::serialize(unsigned int& object, const string& id, bool errors /*= true*/)
{
	return serializeTemp(object, id, errors);
}

bool Serializer::serialize(float& object, const string& id, bool errors /*= true*/)
{
	return serializeTemp(object, id, errors);
}

bool Serializer::serialize(bool& object, const string& id, bool errors /*= true*/)
{
	return serializeTemp(object, id, errors);
}

bool Serializer::serialize(string& object, const string& id, bool errors /*= true*/)
{
	return serializeTemp(object, id, errors);
}

bool Serializer::serialize(vec2f& object, const string& id, bool errors /*= true*/)
{
	return serializeTemp(object, id, errors);
}

bool Serializer::serialize(fRect& object, const string& id, bool errors /*= true*/)
{
	return serializeTemp(object, id, errors);
}

bool Serializer::serialize(vec2i& object, const string& id, bool errors /*= true*/)
{
	return serializeTemp(object, id, errors);
}

bool Serializer::serialize(iRect& object, const string& id, bool errors /*= true*/)
{
	return serializeTemp(object, id, errors);
}

bool Serializer::serialize(color4& object, const string& id, bool errors /*= true*/)
{
	return serializeTemp(object, id, errors);
}

bool Serializer::serialize(WideTime& object, const string& id, bool errors /*= true*/)
{
	return serializeTemp(object, id, errors);
}

Serializable* Serializer::createSerializableSample(const string& type)
{
	return gSerializeTypesContainer::getSample(type);
}

void gSerializeTypesContainer::regType(Serializable* type)
{
	mSamples[type->getTypeName()] = type;
}

Serializable* gSerializeTypesContainer::getSample(const string& typeName)
{
	return mSamples[typeName]->createSample();
}

void gSerializeTypesContainer::outputRegisteredSamples()
{
	FOREACH(SamplesMap, mSamples, sam)
		hlog("Sample: %s", sam->second->getTypeName().c_str());
}

CLOSE_O2_NAMESPACE