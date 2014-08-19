#include "serialize_util.h"
#include <map>

OPEN_O2_NAMESPACE

cSerializer::cSerializer(SerializeType type /*= cSerializer::ST_SERIALIZE*/)
{
	mCurrentNode = mRootNode;
	mLog = gLog;
	mType = type;
}

cSerializer::cSerializer(const string& fileName, SerializeType type /*= ST_SERIALIZE*/)
{
	mCurrentNode = mRootNode;
	mLog = gLog;
	load(fileName);
}

cSerializer::cSerializer(pugi::xml_node& xmlNode, SerializeType type /*= cSerializer::ST_SERIALIZE*/)
{
	mCurrentNode = mRootNode.append_copy(xmlNode);
	mType = type;
}

cSerializer::~cSerializer()
{
}

bool cSerializer::load(const string& file)
{
	if (!cXmlTools::loadFromFile(file, mRootNode))
		return false;

	mCurrentNode = mRootNode;
	mType = ST_DESERIALIZE;

	return true;
}

bool cSerializer::save(const string& file)
{	
	return cXmlTools::saveToFile(file, mRootNode);
}

bool cSerializer::loadFromString(const string& data)
{
	return cXmlTools::loadFromString(data, mRootNode);
}

string cSerializer::saveToString()
{
	string data;
	cXmlTools::saveToString(data, mRootNode);
	return data;
}

void cSerializer::setLog(cLogStream* logStream)
{
	mLog = logStream;
}

void cSerializer::createNode(const string& id)
{
	mCurrentNode = mCurrentNode.append_child(id.c_str());
}

bool cSerializer::getNode(const string& id, bool errors /*= false*/)
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

void cSerializer::popNode()
{
	mCurrentNode = mCurrentNode.parent();
}

cSerializer::SerializeType cSerializer::getType() const
{
	return mType;
}

bool cSerializer::serialize(cSerializable* object, const string& id, bool errors /*= true*/)
{
	if (mType == ST_SERIALIZE)
	{
		createNode(id);
		object->onBeginSerialize.call();
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
		object->onDeserialized.call();
		popNode();
	}

	return true;
}

bool cSerializer::serialize(int object, const string& id, bool errors /*= true*/)
{
	return serializeTemp(object, id, errors);
}

bool cSerializer::serialize(unsigned int object, const string& id, bool errors /*= true*/)
{
	return serializeTemp(object, id, errors);
}

bool cSerializer::serialize(float object, const string& id, bool errors /*= true*/)
{
	return serializeTemp(object, id, errors);
}

bool cSerializer::serialize(bool object, const string& id, bool errors /*= true*/)
{
	return serializeTemp(object, id, errors);
}

bool cSerializer::serialize(string& object, const string& id, bool errors /*= true*/)
{
	return serializeTemp(object, id, errors);
}

bool cSerializer::serialize(vec2f& object, const string& id, bool errors /*= true*/)
{
	return serializeTemp(object, id, errors);
}

bool cSerializer::serialize(fRect& object, const string& id, bool errors /*= true*/)
{
	return serializeTemp(object, id, errors);
}

bool cSerializer::serialize(vec2i& object, const string& id, bool errors /*= true*/)
{
	return serializeTemp(object, id, errors);
}

bool cSerializer::serialize(iRect& object, const string& id, bool errors /*= true*/)
{
	return serializeTemp(object, id, errors);
}

bool cSerializer::serialize(color4& object, const string& id, bool errors /*= true*/)
{
	return serializeTemp(object, id, errors);
}

bool cSerializer::serialize(WideTime& object, const string& id, bool errors /*= true*/)
{
	return serializeTemp(object, id, errors);
}

cSerializable* cSerializer::createSerializableSample(const string& type)
{
	//return gSerializeTypesContainer::mSamples[type]->createSample();
	return NULL;
}

int gSerializeTypesContainer::xx;

CLOSE_O2_NAMESPACE