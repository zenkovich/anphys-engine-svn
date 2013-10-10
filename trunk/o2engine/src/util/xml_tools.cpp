#include "xml_tools.h"

#include "util/file_system/file.h"

OPEN_O2_NAMESPACE

bool cXmlTools::loadFromFile( const std::string& fileName, pugi::xml_document& xmlDoc )
{
	cInFile inFile(fileName, cFileType::FT_CONFIG);

	if (!inFile.isOpened())
		return false;

	uint32 dataSize = inFile.getDataSize();
	char* buffer = mnew char[dataSize];

	inFile.readFullData(buffer);

	pugi::xml_parse_result res = xmlDoc.load_buffer(buffer, dataSize);

	safe_release_arr(buffer);

	return res == pugi::status_ok;
}

pugi::xml_node& cXmlTools::getNode( const pugi::xml_node& node, const std::string& path )
{
	int slashPos = path.find('/');
	/*if (slashPos == )
	std::string currNode*/
	return node.child("asd");
}

vec2i cXmlTools::node2veci( const pugi::xml_node& node )
{
	return vec2i();
}

vec2f cXmlTools::node2vecf( const pugi::xml_node& node )
{
	return vec2f();
}

CLOSE_O2_NAMESPACE