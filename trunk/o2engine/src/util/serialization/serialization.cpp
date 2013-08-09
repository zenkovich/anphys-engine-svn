#include "serialization.h"

#include "util/log/log_stream.h"
#include "util/log.h"

OPEN_O2_NAMESPACE

void cSerialization::loadData( pugi::xml_document& doc, const std::string& fileName, 
	                           cFileType::value fileType /*= cFileType::FT_FILE*/, cLogStream* log /*= NULL*/ )
{
	cInFile file(fileName, fileType);

	if (!file.isOpened())
	{
		cLogStream* nlog = log;
		if (!nlog)
			nlog = gLog;

		nlog->out("ERROR: Failed to open data file: '%s'", fileName.c_str());
		return;
	}

	loadData(doc, file, log);
}

void cSerialization::loadData( pugi::xml_document& doc, cInFile& file, cLogStream* log /*= NULL*/ )
{
	if (!file.isOpened())
		return;

	uint64 size = file.getDataSize();
	char* buf = new char[(unsigned int)size];
	file.readFullData((void*)buf);

	pugi::xml_parse_result res = doc.load_buffer(buf, (unsigned int)size);
	if (res.status != pugi::status_ok)
	{
		cLogStream* nlog = log;
		if (!nlog)
			nlog = gLog;

		nlog->out("ERROR: Failed to load config file '%s': %s", file.getFilename().c_str(), res.description());
	}
}

void cSerialization::saveData( pugi::xml_document& doc, const std::string& fileName, cFileType::value fileType /*= cFileType::FT_FILE*/ )
{
	cOutFile file(fileName, fileType);
	saveData(doc, file);
}

void cSerialization::saveData( pugi::xml_document& doc, cOutFile& file )
{
	struct xmlWriter:public pugi::xml_writer
	{
		cOutFile* file;

		void write(const void* data, size_t size)
		{
			file->writeData(data, size);
		}
	};

	xmlWriter writer;
	writer.file = &file;
	doc.save(writer);

	file.close();
}

void cSerialization::serializeIn( pugi::xml_node& xmlNode, const std::string& id, cSerializableObj* obj, cLogStream* log /*= NULL*/ )
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
		obj = chNode.attribute("v").as_int();
	}
}

void cSerialization::serializeIn( pugi::xml_node& xmlNode, const std::string& id, float& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode;
	if (getNode(chNode, xmlNode, id, log))
	{
		obj = chNode.attribute("v").as_float();
	}
}

void cSerialization::serializeIn( pugi::xml_node& xmlNode, const std::string& id, std::string& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode;
	if (getNode(chNode, xmlNode, id, log))
	{
		obj = chNode.attribute("v").value();
	}
}

void cSerialization::serializeIn( pugi::xml_node& xmlNode, const std::string& id, vec2i& obj, cLogStream* log /*= NULL*/ )
{	
	pugi::xml_node chNode;
	if (getNode(chNode, xmlNode, id, log))
	{
		obj.x = chNode.attribute("x").as_int();
		obj.y = chNode.attribute("y").as_int();
	}
}

void cSerialization::serializeIn( pugi::xml_node& xmlNode, const std::string& id, vec2f& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode;
	if (getNode(chNode, xmlNode, id, log))
	{
		obj.x = chNode.attribute("x").as_float();
		obj.y = chNode.attribute("y").as_float();
	}
}

void cSerialization::serializeIn( pugi::xml_node& xmlNode, const std::string& id, fRect& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode;
	if (getNode(chNode, xmlNode, id, log))
	{
		obj.left = chNode.attribute("left").as_float();
		obj.top = chNode.attribute("top").as_float();
		obj.right = chNode.attribute("right").as_float();
		obj.down = chNode.attribute("bottom").as_float();
	}
}

void cSerialization::serializeIn( pugi::xml_node& xmlNode, const std::string& id, iRect& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode;
	if (getNode(chNode, xmlNode, id, log))
	{
		obj.left = chNode.attribute("left").as_int();
		obj.top = chNode.attribute("top").as_int();
		obj.right = chNode.attribute("right").as_int();
		obj.down = chNode.attribute("bottom").as_int();
	}
}

void cSerialization::serializeIn( pugi::xml_node& xmlNode, const std::string& id, color4& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode;
	if (getNode(chNode, xmlNode, id, log))
	{
		float r, g, b, a;
		r = chNode.attribute("r").as_float();
		g = chNode.attribute("g").as_float();
		b = chNode.attribute("b").as_float();
		a = chNode.attribute("a").as_float();
		obj = color4(r, g, b, a);
	}
}

void cSerialization::serializeIn( pugi::xml_node& xmlNode, const std::string& id, bool& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode;
	if (getNode(chNode, xmlNode, id, log))
	{
		std::string value = chNode.attribute("v").value();
		obj = value == "true" || value == "TRUE" || value == "YES" || value == "1";
	}
}

void cSerialization::serializeOut( pugi::xml_node& xmlNode, const std::string& id, cSerializableObj* obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode = xmlNode.append_child(id.c_str());
	obj->serialize(xmlNode, cSerializeType::INPUT, log);
}

void cSerialization::serializeOut( pugi::xml_node& xmlNode, const std::string& id, const int obj, cLogStream* log /*= NULL*/ )
{	
	pugi::xml_node chNode = xmlNode.append_child(id.c_str());
	chNode.append_attribute("v") = obj;
}

void cSerialization::serializeOut( pugi::xml_node& xmlNode, const std::string& id, const float obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode = xmlNode.append_child(id.c_str());
	chNode.append_attribute("v") = obj;
}

void cSerialization::serializeOut( pugi::xml_node& xmlNode, const std::string& id, const std::string& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode = xmlNode.append_child(id.c_str());
	chNode.append_attribute("v") = obj.c_str();
}

void cSerialization::serializeOut( pugi::xml_node& xmlNode, const std::string& id, const vec2i& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode = xmlNode.append_child(id.c_str());
	chNode.append_attribute("x") = obj.x;
	chNode.append_attribute("y") = obj.y;
}

void cSerialization::serializeOut( pugi::xml_node& xmlNode, const std::string& id, const vec2f& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode = xmlNode.append_child(id.c_str());
	chNode.append_attribute("x") = obj.x;
	chNode.append_attribute("y") = obj.y;
}

void cSerialization::serializeOut( pugi::xml_node& xmlNode, const std::string& id, const fRect& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode = xmlNode.append_child(id.c_str());
	chNode.append_attribute("left") = obj.left;
	chNode.append_attribute("top") = obj.top;
	chNode.append_attribute("right") = obj.right;
	chNode.append_attribute("bottom") = obj.down;
}

void cSerialization::serializeOut( pugi::xml_node& xmlNode, const std::string& id, const iRect& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode = xmlNode.append_child(id.c_str());
	chNode.append_attribute("left") = obj.left;
	chNode.append_attribute("top") = obj.top;
	chNode.append_attribute("right") = obj.right;
	chNode.append_attribute("bottom") = obj.down;
}

void cSerialization::serializeOut( pugi::xml_node& xmlNode, const std::string& id, const color4& obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode = xmlNode.append_child(id.c_str());
	chNode.append_attribute("r") = obj.rf();
	chNode.append_attribute("g") = obj.gf();
	chNode.append_attribute("b") = obj.bf();
	chNode.append_attribute("a") = obj.af();
}

void cSerialization::serializeOut( pugi::xml_node& xmlNode, const std::string& id, const bool obj, cLogStream* log /*= NULL*/ )
{
	pugi::xml_node chNode = xmlNode.append_child(id.c_str());
	chNode.append_attribute("v") = obj ? "true":"false";
}

bool cSerialization::getNode( pugi::xml_node& node, const pugi::xml_node& parent, const std::string& id, cLogStream* log )
{
	node = parent.child(id.c_str());
	if (node.empty())
	{
		cLogStream* safeLog = log;
		if (!safeLog) safeLog = gLog;

		safeLog->hout("WARNING: Can't find node '%s' at serialization!", id.c_str());

		return false;
	}

	return true;
}

CLOSE_O2_NAMESPACE
