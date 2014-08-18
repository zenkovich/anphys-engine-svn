#ifndef XML_TOOLS_H
#define XML_TOOLS_H

#include "public.h"

#include "pugixml/pugixml.hpp"
#include "serialize_util.h"

OPEN_O2_NAMESPACE

/** Some helping functions for working with xml. */
struct cXmlTools
{
	/** Loads xml file. */
	static bool loadFromFile(const string& fileName, pugi::xml_document& xmlDoc);
	
	/** Loads xml from buffer. */
	static bool loadFromString(const string& data, pugi::xml_document& xmlDoc);

	/** Loads xml file. */
	static bool saveToFile(const string& fileName, pugi::xml_document& xmlDoc);

	/** Loads xml buffer. */
	static bool saveToString(string& data, pugi::xml_document& xmlDoc);

	/** Returns node by path, Path format "node/node/node". */
	static pugi::xml_node getNode(const pugi::xml_node& node, const string& path);
	
	/** Saving data from object to xml node. */
	static void toXmlNode(cSerializable& object, pugi::xml_node& node);
	
	/** Saving data from object to xml node. */
	static void toXmlNode(cSerializable* object, pugi::xml_node& node);
	
	/** Saving data from object to xml node. */
	static void toXmlNode(int            object, pugi::xml_node& node);
	
	/** Saving data from object to xml node. */
	static void toXmlNode(unsigned int   object, pugi::xml_node& node);
	
	/** Saving data from object to xml node. */
	static void toXmlNode(float          object, pugi::xml_node& node);
	
	/** Saving data from object to xml node. */
	static void toXmlNode(string         object, pugi::xml_node& node);
	
	/** Saving data from object to xml node. */
	static void toXmlNode(bool           object, pugi::xml_node& node);
	
	/** Saving data from object to xml node. */
	static void toXmlNode(vec2f&         object, pugi::xml_node& node);
	
	/** Saving data from object to xml node. */
	static void toXmlNode(vec2i&         object, pugi::xml_node& node);
	
	/** Saving data from object to xml node. */
	static void toXmlNode(fRect&         object, pugi::xml_node& node);
	
	/** Saving data from object to xml node. */
	static void toXmlNode(iRect&         object, pugi::xml_node& node);
	
	/** Saving data from object to xml node. */
	static void toXmlNode(color4&        object, pugi::xml_node& node);
	
	/** Saving data from object to xml node. */
	static void toXmlNode(WideTime&      object, pugi::xml_node& node);
	
	
	/** Getting data to object from xml node. */
	static void fromXmlNode(cSerializable& object, pugi::xml_node& node);
	
	/** Getting data to object from xml node. */
	static void fromXmlNode(cSerializable* object, pugi::xml_node& node);
	
	/** Getting data to object from xml node. */
	static void fromXmlNode(int&           object, pugi::xml_node& node);
	
	/** Getting data to object from xml node. */
	static void fromXmlNode(unsigned int&  object, pugi::xml_node& node);
	
	/** Getting data to object from xml node. */
	static void fromXmlNode(float&         object, pugi::xml_node& node);
	
	/** Getting data to object from xml node. */
	static void fromXmlNode(string&        object, pugi::xml_node& node);
	
	/** Getting data to object from xml node. */
	static void fromXmlNode(bool&          object, pugi::xml_node& node);
	
	/** Getting data to object from xml node. */
	static void fromXmlNode(vec2f&         object, pugi::xml_node& node);
	
	/** Getting data to object from xml node. */
	static void fromXmlNode(vec2i&         object, pugi::xml_node& node);
	
	/** Getting data to object from xml node. */
	static void fromXmlNode(fRect&         object, pugi::xml_node& node);
	
	/** Getting data to object from xml node. */
	static void fromXmlNode(iRect&         object, pugi::xml_node& node);
	
	/** Getting data to object from xml node. */
	static void fromXmlNode(color4&        object, pugi::xml_node& node);
	
	/** Getting data to object from xml node. */
	static void fromXmlNode(WideTime&      object, pugi::xml_node& node);

	/** Returns specific data from xml node. */
	template<typename T>
	static T get(pugi::xml_node& node)
	{
		T res; fromXmlNode(res, node);
		return res;
	}
	
	/** Saving data from object to xml node. */
	template<typename T>
	static void toXmlNode(T* array, int count, pugi::xml_node& node)
	{
		for (int i = 0; i < count; i++)
		{
			char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
			pugi::xml_node elemNode = node.append_child(elemNodeName);

			toXmlNode(array[i], elemNode);
		}
	}
	
	/** Saving data from object to xml node. */
	template<typename T>
	static void toXmlNode(vector<T>& array, pugi::xml_node& node)
	{
		for (int i = 0; i < count; i++)
		{
			char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
			pugi::xml_node elemNode = node.append_child(elemNodeName);

			toXmlNode(array[i], elemNode);
		}
	}
	
	/** Saving data from object to xml node. */
	template<typename T>
	static void toXmlNode(vector<T*>& array, pugi::xml_node& node)
	{
		for (int i = 0; i < count; i++)
		{
			char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
			pugi::xml_node elemNode = node.append_child(elemNodeName);

			toXmlNode(&(array[i]), elemNode);
		}
	}
	
	
	/** Getting data to object from xml node. */
	template<typename T>
	static void fromXmlNode(T* array, int count, pugi::xml_node& node)
	{		
		for (int i = 0; i < count; i++)
		{
			char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
			pugi::xml_node elemNode = node.child(elemNodeName);

			if (elemNode)
				toXmlNode(array[i], elemNode);
		}
	}
	
	
	/** Getting data to object from xml node. */
	template<typename T>
	static void fromXmlNode(vector<T>& array, pugi::xml_node& node)
	{
		for (int i = 0; i < count; i++)
		{
			char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
			pugi::xml_node elemNode = node.child(elemNodeName);

			if (elemNode) 
			{
				T elem;
				toXmlNode(elem, elemNode);
				array.push_back(elem);
			}
			else break;
		}
	}
	
	
	/** Getting data to object from xml node. */
	template<typename T>
	static void fromXmlNode(vector<T*>& array, pugi::xml_node& node)
	{
		for (int i = 0; i < count; i++)
		{
			char elemNodeName[32]; sprintf(elemNodeName, "elem%i", i);
			pugi::xml_node elemNode = node.child(elemNodeName);

			if (elemNode) 
			{
				T* elem = mnew T();
				toXmlNode(elem, elemNode);
				array.push_back(elem);
			}
			else break;
		}
	}
};

CLOSE_O2_NAMESPACE

#endif //XML_TOOLS_H