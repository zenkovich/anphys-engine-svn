#include "serialization.h"

#include "util/log/log_stream.h"
#include "util/log.h"

OPEN_O2_NAMESPACE

void cSerializableInterface::serialize( pugi::xml_node& xmlNode, cSerializeType::v type, cLogStream* log /*= NULL*/ )
{
	if (log)
		log->out("WARNING: Some serialize method wasn't declared!");
	else
		gLog->out("WARNING: Some serialize method wasn't declared!");
}

void cSerialization::loadData( pugi::xml_document& doc, const std::string& fileName, 
	                           cFileType::value fileType /*= cFileType::FT_FILE*/ )
{
	cInFile file(fileName, fileType);
	loadData(doc, file);
}

void cSerialization::loadData( pugi::xml_document& doc, cInFile& file )
{
	uint64 size = file.getDataSize();
	char* buf = new char[size];
	file.readFullData((void*)buf);
	doc.load_buffer(buf, size);
}

void cSerialization::saveData( pugi::xml_document& doc, const std::string& fileName, cFileType::value fileType /*= cFileType::FT_FILE*/ )
{
	/*cOutFile file(fileName, fileType);
	saveData(doc, file);*/
	doc.save_file(fileName.c_str());
}

void cSerialization::saveData( pugi::xml_document& doc, cInFile& file )
{
	//doc.save(
}

void cSerialization::serializeIn( pugi::xml_node& xmlNode, const std::string& id, cSerializableInterface* obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode;
	if (getNode(chNode, xmlNode, id, log))
	{
		obj->serialize(chNode, cSerializeType::INPUT, log);
	}
}

void cSerialization::serializeIn( pugi::xml_node& xmlNode, const std::string& id, int& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode;
	if (getNode(chNode, xmlNode, id, log))
	{
		chNode.append_attribute("v") = obj;
	}
}

void cSerialization::serializeIn( pugi::xml_node& xmlNode, const std::string& id, float& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode;
	if (getNode(chNode, xmlNode, id, log))
	{
		chNode.append_attribute("v") = obj;
	}
}

void cSerialization::serializeIn( pugi::xml_node& xmlNode, const std::string& id, std::string& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode;
	if (getNode(chNode, xmlNode, id, log))
	{
		chNode.append_attribute("v").set_value(obj.c_str());
	}
}

void cSerialization::serializeIn( pugi::xml_node& xmlNode, const std::string& id, vec2i& obj, cLogStream* log /*= NULL*/ )
{	
	pugi::xml_node chNode;
	if (getNode(chNode, xmlNode, id, log))
	{
		chNode.append_attribute("x") = obj.x;
		chNode.append_attribute("y") = obj.y;
	}
}

void cSerialization::serializeIn( pugi::xml_node& xmlNode, const std::string& id, vec2f& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode;
	if (getNode(chNode, xmlNode, id, log))
	{
		chNode.append_attribute("x") = obj.x;
		chNode.append_attribute("y") = obj.y;
	}
}

void cSerialization::serializeIn( pugi::xml_node& xmlNode, const std::string& id, fRect& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode;
	if (getNode(chNode, xmlNode, id, log))
	{
		chNode.append_attribute("left") = obj.left;
		chNode.append_attribute("top") = obj.top;
		chNode.append_attribute("right") = obj.right;
		chNode.append_attribute("bottom") = obj.down;
	}
}

void cSerialization::serializeIn( pugi::xml_node& xmlNode, const std::string& id, iRect& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode;
	if (getNode(chNode, xmlNode, id, log))
	{
		chNode.append_attribute("left") = obj.left;
		chNode.append_attribute("top") = obj.top;
		chNode.append_attribute("right") = obj.right;
		chNode.append_attribute("bottom") = obj.down;
	}
}

void cSerialization::serializeIn( pugi::xml_node& xmlNode, const std::string& id, color4& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode;
	if (getNode(chNode, xmlNode, id, log))
	{
		chNode.append_attribute("r") = obj.rf();
		chNode.append_attribute("g") = obj.gf();
		chNode.append_attribute("b") = obj.bf();
		chNode.append_attribute("a") = obj.af();
	}
}

void cSerialization::serializeOut( pugi::xml_node& xmlNode, const std::string& id, cSerializableInterface* obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode = xmlNode.append_child(id.c_str());
	obj->serialize(xmlNode, cSerializeType::INPUT, log);
}

void cSerialization::serializeOut( pugi::xml_node& xmlNode, const std::string& id, const int obj, cLogStream* log /*= NULL*/ )
{	
	pugi::xml_node chNode = xmlNode.append_child(id.c_str());
	chNode.append_attribute("v").
}

void cSerialization::serializeOut( pugi::xml_node& xmlNode, const std::string& id, const float obj, cLogStream* log /*= NULL*/ )
{

}

void cSerialization::serializeOut( pugi::xml_node& xmlNode, const std::string& id, const std::string& obj, cLogStream* log /*= NULL*/ )
{

}

void cSerialization::serializeOut( pugi::xml_node& xmlNode, const std::string& id, const vec2i& obj, cLogStream* log /*= NULL*/ )
{

}

void cSerialization::serializeOut( pugi::xml_node& xmlNode, const std::string& id, const vec2f& obj, cLogStream* log /*= NULL*/ )
{

}

void cSerialization::serializeOut( pugi::xml_node& xmlNode, const std::string& id, const fRect& obj, cLogStream* log /*= NULL*/ )
{

}

void cSerialization::serializeOut( pugi::xml_node& xmlNode, const std::string& id, const iRect& obj, cLogStream* log /*= NULL*/ )
{

}

void cSerialization::serializeOut( pugi::xml_node& xmlNode, const std::string& id, const color4& obj, cLogStream* log /*= NULL*/ )
{

}

bool cSerialization::getNode( pugi::xml_node& node, const pugi::xml_node& parent, const std::string& id, cLogStream* log )
{
	node = parent.child(id.c_str());
	if (node.empty())
	{
		cLogStream* safeLog = log;
		if (!safeLog) safeLog = gLog;

		safeLog->hout("WARNING: Can't find node '%s' at serialization!", id.c_str());
	}
}

CLOSE_O2_NAMESPACE
