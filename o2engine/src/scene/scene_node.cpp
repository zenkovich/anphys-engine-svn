#include "scene_node.h"

OPEN_O2_NAMESPACE



	scNode::scNode()
{

}

scNode::scNode(const scNode& node)
{

}

scNode::~scNode()
{

}

scNode& scNode::operator=(const scNode& node)
{

}

scNode::addNode(const shared(scNode)& node)
{

}

bool scNode::removeNode(const shared(scNode)& node)
{

}

bool scNode::removeAllNodes()
{

}

scNode::getNode(const string& id) const
{

}

scNode::addComponent(const shared(scComponent)& compontent)
{

}

bool scNode::removeComponent(const shared(scComponent)& compontent)
{

}

bool scNode::removeAllComponents()
{

}

void scNode::setId(const string& id)
{

}

ENGINE_NAMESPACE_NAME::string scNode::getId() const
{

}

void scNode::setParent(const shared(scNode)& parent)
{

}

scNode::getParent() const
{

}

void scNode::setTransform(const basisDef& def)
{

}

ENGINE_NAMESPACE_NAME::basisDef scNode::getTransform() const
{

}

void scNode::setLocalTransform(const basisDef& def)
{

}

ENGINE_NAMESPACE_NAME::basisDef scNode::getLocalTransform() const
{

}

void scNode::setBasis(const basis& basis_)
{

}

ENGINE_NAMESPACE_NAME::basisDef scNode::getBasis() const
{

}

void scNode::setLocalBasis(const basis& basis_)
{

}

ENGINE_NAMESPACE_NAME::basisDef scNode::getLocalBasis() const
{

}

void scNode::setPosition(const vec2f& position)
{

}

vec2f scNode::getPosition() const
{

}

void scNode::setLocalPosition(const vec2f& position)
{

}

vec2f scNode::getLocalPosition() const
{

}

void scNode::setScale(const vec2f& scale)
{

}

vec2f scNode::getScale() const
{

}

void scNode::setLocalScale(const vec2f& scale)
{

}

vec2f scNode::getLocalScale() const
{

}

void scNode::setSize(const vec2f& size)
{

}

vec2f scNode::getSize() const
{

}

void scNode::setLocalSize(const vec2f& size)
{

}

vec2f scNode::getLocalSize() const
{

}

void scNode::setRotation(const float& angle)
{

}

float scNode::getRotation()
{

}

void scNode::setLocalRotation(const float& angle)
{

}

float scNode::getLocalRotation()
{

}

void scNode::setShift(const float& shift)
{

}

float scNode::getShift() const
{

}

void scNode::setLocalShift(const float& shift)
{

}

float scNode::getLocalShift() const
{

}

void scNode::setEnable(const bool& enable)
{

}

bool scNode::isEnable() const
{

}

void scNode::setVisible(const bool& visible)
{

}

bool scNode::isVisible() const
{

}

void scNode::setDepth(const float& depth)
{

}

float scNode::getDepth() const
{

}

void scNode::setColor(const color4& color)
{

}

ENGINE_NAMESPACE_NAME::color4 scNode::getColor() const
{

}

void scNode::initializeProperties()
{

}

void scNode::update(float dt)
{

}

void scNode::draw()
{

}

CLOSE_O2_NAMESPACE