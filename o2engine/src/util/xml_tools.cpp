#include "xml_tools.h"

#include "util/file_system/file.h"
#include "util/smart_ptrs.h"
#include "serialize_util.h"

OPEN_O2_NAMESPACE

bool XmlTools::loadFromFile( const string& fileName, pugi::xml_document& xmlDoc, bool isConfigFile /*= true*/ )
{
	InFile inFile(fileName, isConfigFile ? FileType::CONFIG:FileType::FILE);

	if (!inFile.isOpened())
		return false;

	uint32 dataSize = inFile.getDataSize();
	autoArr(char) buffer = mnew char[dataSize];

	inFile.readFullData(buffer);

	pugi::xml_parse_result res = xmlDoc.load_buffer(buffer, dataSize);

	return res.status == pugi::status_ok;
}

bool XmlTools::loadFromString(const string& data, pugi::xml_document& xmlDoc)
{
	pugi::xml_parse_result res = xmlDoc.load_buffer(data.c_str(), data.length());
	return res.status == pugi::status_ok;
}

bool XmlTools::saveToString(string& data, pugi::xml_document& xmlDoc)
{
	struct xmlWriter:public pugi::xml_writer
	{
		string* str;

		void write(const void* data, size_t size)
		{
			*str += (char*)data;
		}
	};

	xmlWriter writer;
	writer.str = &data;
	xmlDoc.save(writer);

	return true;
}

bool XmlTools::saveToFile(const string& fileName, pugi::xml_document& xmlDoc, bool isConfigFile /*= true*/)
{
	struct xmlWriter:public pugi::xml_writer
	{
		OutFile* file;

		void write(const void* data, size_t size)
		{
			file->writeData(data, size);
		}
	};

	OutFile outFile(fileName, isConfigFile ? FileType::CONFIG:FileType::FILE);

	if (!outFile.isOpened())
		return false;

	xmlWriter writer;
	writer.file = &outFile;
	xmlDoc.save(writer);

	outFile.close();

	return true;
}

pugi::xml_node XmlTools::getNode( const pugi::xml_node& node, const string& path )
{
	int slashPos = path.find('/');
	string currNode = path.substr(0, slashPos);
	
	if (slashPos == path.npos)
		return node.child(currNode.c_str());
	else
		return getNode(node.child(currNode.c_str()), path.substr(slashPos + 1));
}

void XmlTools::toXmlNode(int object, pugi::xml_node& node)
{
	node.append_attribute("v") = object;
}

void XmlTools::toXmlNode(unsigned int object, pugi::xml_node& node)
{
	node.append_attribute("v") = object;
}

void XmlTools::toXmlNode(float object, pugi::xml_node& node)
{
	node.append_attribute("v") = object;
}

void XmlTools::toXmlNode(string& object, pugi::xml_node& node)
{
	node.append_attribute("v") = object.c_str();
}

void XmlTools::toXmlNode(bool object, pugi::xml_node& node)
{
	node.append_attribute("v") = object;
}

void XmlTools::toXmlNode(vec2f& object, pugi::xml_node& node)
{
	node.append_attribute("x") = object.x;
	node.append_attribute("y") = object.y;
}

void XmlTools::toXmlNode(vec2i& object, pugi::xml_node& node)
{
	node.append_attribute("x") = object.x;
	node.append_attribute("y") = object.y;
}

void XmlTools::toXmlNode(fRect& object, pugi::xml_node& node)
{
	node.append_attribute("left") = object.left;
	node.append_attribute("top") = object.top;
	node.append_attribute("right") = object.right;
	node.append_attribute("down") = object.down;
}

void XmlTools::toXmlNode(iRect& object, pugi::xml_node& node)
{
	node.append_attribute("left") = object.left;
	node.append_attribute("top") = object.top;
	node.append_attribute("right") = object.right;
	node.append_attribute("down") = object.down;
}

void XmlTools::toXmlNode(color4& object, pugi::xml_node& node)
{
	node.append_attribute("r") = object.r;
	node.append_attribute("g") = object.g;
	node.append_attribute("b") = object.b;
	node.append_attribute("a") = object.a;
}

void XmlTools::toXmlNode(WideTime& object, pugi::xml_node& node)
{
	node.append_attribute("second") = object.mSecond;
	node.append_attribute("minute") = object.mMinute;
	node.append_attribute("hour")   = object.mHour;
	node.append_attribute("day")    = object.mDay;
	node.append_attribute("month")  = object.mMonth;
	node.append_attribute("year")   = object.mYear;
}

void XmlTools::fromXmlNode(int& object, pugi::xml_node& node)
{
	object = node.attribute("v").as_int();
}

void XmlTools::fromXmlNode(unsigned int& object, pugi::xml_node& node)
{
	object = node.attribute("v").as_uint();
}

void XmlTools::fromXmlNode(float& object, pugi::xml_node& node)
{
	object = node.attribute("v").as_float();
}

void XmlTools::fromXmlNode(string& object, pugi::xml_node& node)
{
	object = node.attribute("v").value();
}

void XmlTools::fromXmlNode(bool& object, pugi::xml_node& node)
{
	object = node.attribute("v").as_bool();
}

void XmlTools::fromXmlNode(vec2f& object, pugi::xml_node& node)
{
	object.x = node.attribute("x").as_float();
	object.y = node.attribute("y").as_float();
}

void XmlTools::fromXmlNode(vec2i& object, pugi::xml_node& node)
{
	object.x = node.attribute("x").as_int();
	object.y = node.attribute("y").as_int();
}

void XmlTools::fromXmlNode(fRect& object, pugi::xml_node& node)
{
	object.left = node.attribute("left").as_float();
	object.right = node.attribute("right").as_float();
	object.top = node.attribute("top").as_float();
	object.down = node.attribute("down").as_float();
}

void XmlTools::fromXmlNode(iRect& object, pugi::xml_node& node)
{
	object.left = node.attribute("left").as_int();
	object.right = node.attribute("right").as_int();
	object.top = node.attribute("top").as_int();
	object.down = node.attribute("down").as_int();
}

void XmlTools::fromXmlNode(color4& object, pugi::xml_node& node)
{
	object.r = node.attribute("r").as_int();
	object.g = node.attribute("g").as_int();
	object.b = node.attribute("b").as_int();
	object.a = node.attribute("a").as_int();
}

void XmlTools::fromXmlNode(WideTime& object, pugi::xml_node& node)
{
	object.mSecond = node.attribute("second").as_int();
	object.mMinute = node.attribute("minute").as_int();
	object.mHour   = node.attribute("hour").as_int();
	object.mDay    = node.attribute("day").as_int();
	object.mMonth  = node.attribute("month").as_int();
	object.mYear   = node.attribute("year").as_int();
}

CLOSE_O2_NAMESPACE