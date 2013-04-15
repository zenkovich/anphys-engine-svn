#include "data_object.h"

#include <algorithm>

#include "util/memory/mem_utils.h"


cDataObject::cDataObject(const std::string& id /* = ""*/):mId(id), mParent(NULL)
{
}

cDataObject::cDataObject( const cDataObject& object )
{
	mId = object.mId;

	mParent = NULL;
	
	for (DataObjectsList::const_iterator it = object.mChilds.cbegin(); it != object.mChilds.cend(); ++it)
		mChilds.push_back((*it)->clone());
}

cDataObject::~cDataObject()
{
	clear();
}

cDataObject* cDataObject::addChild( cDataObject* child )
{
	mChilds.push_back(child);
	child->mParent = this;
	return child;
}

cDataObject* cDataObject::addChild(const std::string& id /* = ""*/)
{
	if (id == "") return this;

	cDataObject* newChild = new cDataObject(id);

	mChilds.push_back(newChild);
	newChild->mParent = this;

	return newChild;
}

bool cDataObject::removeChild( cDataObject* child )
{
	DataObjectsList::iterator fnd = std::find(mChilds.begin(), mChilds.end(), child);
	if (fnd == mChilds.end()) return false;
	
	safe_release(child);

	mChilds.erase(fnd);

	return true;
}

void cDataObject::clear()
{
	for (DataObjectsList::iterator it = mChilds.begin(); it != mChilds.end(); ++it)
		safe_release(*it);

	mChilds.clear();
}

cDataObject* cDataObject::clone() const
{
	return new cDataObject(*this);
}

cDataObject* cDataObject::getChild(const std::string& id)
{
	if (id == "") return this;

	int slashPos = id.find('/');

	std::string searchId;
	if (slashPos == id.npos) searchId = id;
	else                     searchId = id.substr(0, slashPos);

	if (searchId == "..")
	{
		if (!mParent)
			return NULL;
		else
			return mParent->getChild(id.substr(slashPos + 1));
	}

	for (DataObjectsList::iterator it = mChilds.begin(); it != mChilds.end(); ++it)
	{
		if ((*it)->mId == searchId) 
		{
			if (slashPos == id.npos)
				return *it;
			else
				return (*it)->getChild(id.substr(slashPos + 1));
		}
	}

	return NULL;
}

void cDataObject::saveDataTo( pugi::xml_node& xmlNode )
{
	xmlNode.set_name(mId.c_str());

	xmlNode.append_attribute("type") = "obj";

	for (DataObjectsList::iterator it = mChilds.begin(); it != mChilds.end(); ++it)
		(*it)->saveDataTo(xmlNode.append_child());
}

void cDataObject::loadDataFrom( pugi::xml_node& xmlNode )
{
	clear();

	mId = xmlNode.name();

	for (pugi::xml_node::iterator it = xmlNode.begin(); it != xmlNode.end(); it++)
	{
		std::string valueType = it->attribute("type").value();
		std::string name = it->name();

		cDataObject* newChild = NULL;

		if (valueType == "obj")         newChild = new cDataObject(name);
		else if (valueType == "float")  newChild = new cDataObjectContainer<float>(name);
		else if (valueType == "int")    newChild = new cDataObjectContainer<int>(name);
		else if (valueType == "uint")   newChild = new cDataObjectContainer<unsigned int>(name);
		else if (valueType == "string") newChild = new cDataObjectContainer<std::string>("", name);
		else if (valueType == "bool")   newChild = new cDataObjectContainer<bool>(name);
		else if (valueType == "vec2")   newChild = new cDataObjectContainer<vec2>(name);
		else if (valueType == "vec3")   newChild = new cDataObjectContainer<vec3>(name);
		else if (valueType == "color4") newChild = new cDataObjectContainer<color4>(name);
		else if (valueType == "rect")   newChild = new cDataObjectContainer<fRect>(name);
		else continue;

		newChild->loadDataFrom(*it);
		mChilds.push_back(newChild);
		newChild->mParent = this;
	}
}

void cDataObject::getXmlValue( pugi::xml_node& xmlNode, float& value )
{
	value = xmlNode.attribute("v").as_float();
}

void cDataObject::getXmlValue( pugi::xml_node& xmlNode, std::string& value )
{
	value = xmlNode.attribute("v").value();
}

void cDataObject::getXmlValue( pugi::xml_node& xmlNode, int& value )
{
	value = xmlNode.attribute("v").as_int();
}

void cDataObject::getXmlValue( pugi::xml_node& xmlNode, unsigned int& value )
{
	value = xmlNode.attribute("v").as_uint();
}

void cDataObject::getXmlValue( pugi::xml_node& xmlNode, bool& value )
{
	value = xmlNode.attribute("v").as_bool();
}

void cDataObject::getXmlValue( pugi::xml_node& xmlNode, vec2& value )
{
	value.x = xmlNode.attribute("x").as_float();
	value.y = xmlNode.attribute("y").as_float();
}

void cDataObject::getXmlValue( pugi::xml_node& xmlNode, vec3& value )
{
	value.x = xmlNode.attribute("x").as_float();
	value.y = xmlNode.attribute("y").as_float();
	value.z = xmlNode.attribute("z").as_float();
}

void cDataObject::getXmlValue( pugi::xml_node& xmlNode, color4& value )
{
	value.r = xmlNode.attribute("r").as_int();
	value.g = xmlNode.attribute("g").as_int();
	value.b = xmlNode.attribute("b").as_int();
	value.a = xmlNode.attribute("a").as_int();
}

void cDataObject::getXmlValue( pugi::xml_node& xmlNode, fRect& value )
{
	value.leftTop.x = xmlNode.attribute("left").as_float();
	value.leftTop.y = xmlNode.attribute("top").as_float();
	value.rightDown.x = xmlNode.attribute("right").as_float();
	value.rightDown.y = xmlNode.attribute("bottom").as_float();
}

void cDataObject::setXmlvalue( pugi::xml_node& xmlNode, float value )
{
	xmlNode.append_attribute("type") = "float";
	xmlNode.append_attribute("v") = value;
}

void cDataObject::setXmlvalue( pugi::xml_node& xmlNode, std::string value )
{
	xmlNode.append_attribute("type") = "string";
	xmlNode.append_attribute("v") = value.c_str();
}

void cDataObject::setXmlvalue( pugi::xml_node& xmlNode, int value )
{
	xmlNode.append_attribute("type") = "int";
	xmlNode.append_attribute("v") = value;
}

void cDataObject::setXmlvalue( pugi::xml_node& xmlNode, unsigned int value )
{
	xmlNode.append_attribute("type") = "uint";
	xmlNode.append_attribute("v") = value;
}

void cDataObject::setXmlvalue( pugi::xml_node& xmlNode, bool value )
{
	xmlNode.append_attribute("type") = "bool";
	xmlNode.append_attribute("v") = value;
}

void cDataObject::setXmlvalue( pugi::xml_node& xmlNode, vec2 value )
{
	xmlNode.append_attribute("type") = "vec2";
	xmlNode.append_attribute("x") = value.x;
	xmlNode.append_attribute("y") = value.y;
}

void cDataObject::setXmlvalue( pugi::xml_node& xmlNode, vec3 value )
{
	xmlNode.append_attribute("type") = "vec3";
	xmlNode.append_attribute("x") = value.x;
	xmlNode.append_attribute("y") = value.y;
	xmlNode.append_attribute("z") = value.z;
}

void cDataObject::setXmlvalue( pugi::xml_node& xmlNode, color4 value )
{
	xmlNode.append_attribute("type") = "color4";
	xmlNode.append_attribute("r") = value.r;
	xmlNode.append_attribute("g") = value.g;
	xmlNode.append_attribute("b") = value.b;
	xmlNode.append_attribute("a") = value.a;
}

void cDataObject::setXmlvalue( pugi::xml_node& xmlNode, fRect value )
{
	xmlNode.append_attribute("type") = "rect";
	xmlNode.append_attribute("left") = value.leftTop.x;
	xmlNode.append_attribute("top") = value.leftTop.y;
	xmlNode.append_attribute("right") = value.rightDown.x;
	xmlNode.append_attribute("bottom") = value.rightDown.y;
}
