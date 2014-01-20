#ifndef XML_TOOLS_H
#define XML_TOOLS_H

#include "public.h"

#include "pugixml/pugixml.hpp"

#include "util/math/vector2.h"
#include "util/math/rect.h"

OPEN_O2_NAMESPACE

/** Some helping functions for working with xml. */
struct cXmlTools
{
	/** Loads xml file. */
	static bool loadFromFile(const std::string& fileName, pugi::xml_document& xmlDoc);

	/** Returns node by path, Path format "node/node/node". */
	static pugi::xml_node getNode(const pugi::xml_node& node, const std::string& path);

	/** Getting vec2f data from xml node. */
	static vec2f node2vecf(const pugi::xml_node& node);

	/** Getting vec2i data from xml node. */
	static vec2i node2veci(const pugi::xml_node& node);

	/** Getting rectf data from xml node. */
	static fRect node2rectf(const pugi::xml_node& node);
};

CLOSE_O2_NAMESPACE

#endif //XML_TOOLS_H