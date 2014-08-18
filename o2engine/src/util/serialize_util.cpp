#include "serialize_util.h"

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

CLOSE_O2_NAMESPACE