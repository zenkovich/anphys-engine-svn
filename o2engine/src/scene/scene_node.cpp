#include "scene_node.h"

OPEN_O2_NAMESPACE

scNode::scNode():
	mEnabled(true), mVisible(true), mDepth(0), mColor(255, 255, 255, 255), mParent(NULL), mOnEnabled(NULL), mOnDisabled(NULL),
	mWorldTransfDefActual(false)
{
	initializeProperties();
	updateTransform();
}

scNode::scNode(const scNode& node)
{
	initializeProperties();

	mId = node.mId;
	mEnabled = node.mEnabled;
	mVisible = node.mVisible;
	mTransform = node.mTransform;
	mTransformDef = node.mTransformDef;
	mDepth = node.mDepth;
	mColor = node.mColor;
	setParent(node.mParent);
	mOnEnabled = node.mOnEnabled;
	mOnDisabled = node.mOnDisabled;

	FOREACH_CONST(ComponentsVec, node.mComponents, comp)
		addComponent((*comp)->clone());

	FOREACH_CONST(NodesVec, node.mChilds, child)
		addNode(mnew scNode(*(*child)));

	mWorldTransfDefActual = false;

	updateTransform();
}

scNode::~scNode()
{
	setParent(NULL);
	removeAllNodes();
	removeAllComponents();
}

scNode& scNode::operator=(const scNode& node)
{
	setParent(NULL);
	removeAllNodes();
	removeAllComponents();

	mId = node.mId;
	mEnabled = node.mEnabled;
	mVisible = node.mVisible;
	mTransform = node.mTransform;
	mTransformDef = node.mTransformDef;
	mDepth = node.mDepth;
	mColor = node.mColor;
	setParent(node.mParent);
	mOnEnabled = node.mOnEnabled;
	mOnDisabled = node.mOnDisabled;

	FOREACH_CONST(ComponentsVec, node.mComponents, comp)
		addComponent((*comp)->clone());

	FOREACH_CONST(NodesVec, node.mChilds, child)
		addNode(mnew scNode(*(*child)));

	mNeedUpdateTransform = false;
	updateTransform();

	return *this;
}

scNode* scNode::addNode(scNode* node)
{
	mChilds.push_back(node);
	node->mParent = this;
	return node;
}

bool scNode::removeNode(scNode* node)
{
	NodesVec::iterator fnd = FIND(mChilds, node);
	if (fnd == mChilds.end())
		return false;

	mChilds.erase(fnd);
	safe_release(node);
	return true;
}

bool scNode::removeAllNodes()
{
	if (mChilds.size() == 0)
		return false;

	FOREACH(NodesVec, mChilds, child)
		safe_release(*child);

	mChilds.clear();

	return true;
}

scNode* scNode::getNode(const string& id) const
{
	int delPos = id.find("/");
	string pathPart = id.substr(0, delPos);

	if (pathPart == "..")
	{
		if (mParent)
		{
			if (delPos == id.npos)
				return mParent;
			else
				return mParent->getNode(id.substr(delPos + 1));
		}

		return NULL;
	}

	FOREACH_CONST(NodesVec, mChilds, it)
	{
		if ((*it)->mId == pathPart)
		{
			if (delPos == id.npos)
				return (*it);
			else
				return (*it)->getNode(id.substr(delPos + 1));
		}
	}

	return NULL;
}

scComponent* scNode::addComponent(scComponent* component)
{
	mComponents.push_back(component);
	component->mOwnerNode = this;
	return component;
}

bool scNode::removeComponent(scComponent* component)
{
	ComponentsVec::iterator fnd = FIND(mComponents, component);
	
	if (fnd == mComponents.end())
		return false;
	
	safe_release(component);
	mComponents.erase(fnd);
	return true;
}

bool scNode::removeAllComponents()
{
	if (mComponents.size() == 0)
		return false;

	FOREACH(ComponentsVec, mComponents, comp)
		safe_release(*comp);

	mComponents.clear();

	return true;
}

void scNode::setId(const string& id)
{
	mId = id;
}

string scNode::getId() const
{
	return mId;
}

void scNode::setParent(scNode* parent)
{	
	if (mParent)
	{
		NodesVec::iterator fnd = FIND(mParent->mChilds, this );
		if (fnd != mParent->mChilds.end())
			mParent->mChilds.erase(fnd);
	}

	mParent = parent;

	if (mParent)
		mParent->addNode(this);

	updateTransform();
}

scNode* scNode::getParent() const
{
	return mParent;
}

void scNode::setTransform(const basisDef& def)
{
	mWorldTransformDef = def;
	mWorldTransform = def.build();
	mTransform = mParent ? mWorldTransform*mParent->getBasis().inverted():mWorldTransform;
	mTransformDef = basis2def(mTransform);
	mNeedUpdateTransform = false;
	mWorldTransfDefActual = false;
}

basisDef scNode::getTransform()
{
	if (!mWorldTransfDefActual)
	{
		mWorldTransfDefActual = true;
		mWorldTransformDef = basis2def(mWorldTransform);
	}

	return mWorldTransformDef;
}

void scNode::setLocalTransform(const basisDef& def)
{
	mTransformDef = def;
	updateTransform();
}
basisDef scNode::getLocalTransform() const
{
	return mTransformDef;
}

void scNode::setBasis(const basis& basis_)
{
	mWorldTransform = basis_;
	mTransform = mParent ? mWorldTransform*mParent->getBasis().inverted():mWorldTransform;
	mTransformDef = basis2def(mTransform);
	mNeedUpdateTransform = false;
	mWorldTransfDefActual = false;
}

basis scNode::getBasis() const
{
	return mWorldTransform;
}

void scNode::setLocalBasis(const basis& basis_)
{
	mTransform = basis_;
	mTransformDef = basis2def(mTransform);
	updateWorldTransform();
	mNeedUpdateTransform = false;
	mWorldTransfDefActual = false;
}

basis scNode::getLocalBasis() const
{
	return mTransform;
}

void scNode::setPosition(const vec2f& position)
{
	mTransformDef.mPosition = mParent ? mParent->getBasis().inverted().transform(position):position;
	updateTransform();
}

vec2f scNode::getPosition()
{
	return getTransform().mPosition;
}

void scNode::setLocalPosition(const vec2f& position)
{
	mTransformDef.mPosition = position;
	updateTransform();
}

vec2f scNode::getLocalPosition() const
{
	return mTransformDef.mPosition;
}

void scNode::setScale(const vec2f& scale)
{
	if (mParent)
	{
		vec2f parentScale = mParent->getScale();
		mTransformDef.mScale = scale.scale(vec2f(1.0f/parentScale.x, 1.0f/parentScale.y));
	}
	else
		mTransformDef.mScale = scale;

	updateTransform();
}

vec2f scNode::getScale() const
{
	return ( mParent ? mTransformDef.mScale.scale(mParent->getScale()):mTransformDef.mScale );
}

void scNode::setLocalScale(const vec2f& scale)
{
	mTransformDef.mScale = scale;
	updateTransform();
}

vec2f scNode::getLocalScale() const
{
	return mTransformDef.mScale;
}

void scNode::setSize(const vec2f& size)
{

}

vec2f scNode::getSize() const
{
	return vec2f();
}

void scNode::setLocalSize(const vec2f& size)
{

}

vec2f scNode::getLocalSize() const
{
	return vec2f();
}

void scNode::setRotation(float angle)
{

}

float scNode::getRotation()
{
	return 0;
}

void scNode::setLocalRotation(float angle)
{

}

float scNode::getLocalRotation()
{
	return 0;
}

void scNode::setShift(float shift)
{

}

float scNode::getShift() const
{
	return 0;
}

void scNode::setLocalShift(float shift)
{

}

float scNode::getLocalShift() const
{
	return 0;
}

void scNode::setEnable(bool enable)
{
	mEnabled = enable;

	if (mOnEnabled)
		mOnEnabled->call();
}

bool scNode::isEnable() const
{
	return mEnabled;
}

void scNode::setVisible(bool visible)
{
	mVisible = visible;
}

bool scNode::isVisible() const
{
	return mVisible;
}

void scNode::setDepth(float depth)
{
	mDepth = depth;
}

float scNode::getDepth() const
{
	return mDepth;
}

void scNode::setColor(const color4& color)
{
	mColor = color;
}

color4 scNode::getColor() const
{
	return mColor;
}

void scNode::initializeProperties()
{
	REG_PROPERTY(scNode, id, setId, getId);
	REG_PROPERTY_SETTER_NONCONST(scNode, parent, setParent, getParent);
	REG_PROPERTY_SETTER_NONCONST(scNode, enabled, setEnable, isEnable);
	REG_PROPERTY_SETTER_NONCONST(scNode, visible, setVisible, isVisible);
	REG_PROPERTY(scNode, basis_, setBasis, getBasis);
	REG_PROPERTY(scNode, localBasis, setLocalBasis, getLocalBasis);
	REG_PROPERTY_GETTER_NONCONST(scNode, transform, setTransform, getTransform);
	REG_PROPERTY(scNode, localTransform, setLocalTransform, getLocalTransform);
	REG_PROPERTY_GETTER_NONCONST(scNode, position, setPosition, getPosition);
	REG_PROPERTY(scNode, localPosition, setLocalPosition, getLocalPosition);
	REG_PROPERTY(scNode, scale, setScale, getScale);
	REG_PROPERTY(scNode, localScale, setLocalScale, getLocalScale);
	REG_PROPERTY(scNode, size, setSize, getSize);
	REG_PROPERTY(scNode, localSize, setLocalSize, getLocalSize);
	REG_PROPERTY_NONCONST(scNode, rotation, setRotation, getRotation);
	REG_PROPERTY_NONCONST(scNode, localRotation, setLocalRotation, getLocalRotation);
	REG_PROPERTY_SETTER_NONCONST(scNode, shift, setShift, getShift);
	REG_PROPERTY_SETTER_NONCONST(scNode, depth, setDepth, getDepth);
	REG_PROPERTY(scNode, color, setColor, getColor);
}

void scNode::update(float dt)
{

}

void scNode::draw()
{

}

scNode* scNode::loadFromFile(const string& fileName)
{
	return mnew scNode();
}

void scNode::updateTransform()
{
	mNeedUpdateTransform = false;
	mTransform = mTransformDef.build();
	mWorldTransfDefActual = false;
	updateWorldTransform();
}

void scNode::checkTransform()
{
	if (mNeedUpdateTransform)
		updateTransform();
}

void scNode::updateWorldTransform()
{
	mWorldTransform = mParent ? mTransform*mParent->getBasis():mTransform;
}

CLOSE_O2_NAMESPACE