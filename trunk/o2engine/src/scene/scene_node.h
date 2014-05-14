#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include "public.h"

#include "node_component.h"
#include "util/property.h"
#include "util/math/basis.h"
#include "util/callback.h"

OPEN_O2_NAMESPACE

class scNode
{
	typedef vector< shared(scComponent) > ComponentsVec;
	typedef vector< scNode > NodesVec;

protected:
	string            mId;
	bool              mEnabled;
	bool              mVisible;
	basis             mTransform;
	basisDef          mTransformDef;
	float             mDepth;
	color4            mColor;
	shared(scNode)    mParent;
	ComponentsVec     mComponents;
	NodesVec          mChilds;

	shared(ICallback) mOnEnabled;
	shared(ICallback) mOnDisabled;

public:
	PROPERTY(scNode, string)         id;
	PROPERTY(scNode, shared(scNode)) parent;
	PROPERTY(scNode, bool)           enabled;
	PROPERTY(scNode, bool)           visible;
	PROPERTY(scNode, basis)          basis_;
	PROPERTY(scNode, basis)          localBasis;
	PROPERTY(scNode, basisDef)       transform;
	PROPERTY(scNode, basisDef)       localTransform;
	PROPERTY(scNode, vec2f)          position;
	PROPERTY(scNode, vec2f)          localPosition;
	PROPERTY(scNode, vec2f)          scale;
	PROPERTY(scNode, vec2f)          localScale;
	PROPERTY(scNode, vec2f)          size;
	PROPERTY(scNode, vec2f)          localSize;
	PROPERTY(scNode, float)          rotation;
	PROPERTY(scNode, float)          localRotation;
	PROPERTY(scNode, float)          shift;
	PROPERTY(scNode, float)          localShift;
	PROPERTY(scNode, float)          depth;
	PROPERTY(scNode, color4)         color;

	
	scNode();
	scNode(const scNode& node);
	virtual ~scNode();

	scNode& operator=(const scNode& node);

	shared(scNode) addNode(const shared(scNode)& node);
	bool removeNode(const shared(scNode)& node);
	bool removeAllNodes();
	shared(scNode) getNode(const string& id) const;

	shared(scComponent) getComponent(UniqueType componentType);
	shared(scComponent) addComponent(const shared(scComponent)& compontent);
	bool removeComponent(const shared(scComponent)& compontent);
	bool removeAllComponents();

	template<typename T>
	shared(T) addNode(const shared(T)& node)
	{
		return shared(T)(addNode( shared(scNode)(node) ));
	}

	template<typename T>
	shared(T) addComponent(const shared(T)& compontent)
	{
		return shared(T)(addComponent( shared(scComponent)(compontent) ));
	}
	
	template<typename T>
	shared(T) getComponent(UniqueType componentType)
	{
		return shared(T)(getComponent( componentType ));
	}

	void setId(const string& id);
	string getId() const;

	void setParent(const shared(scNode)& parent);
	shared(scNode) getParent() const;

	void setTransform(const basisDef& def);
	basisDef getTransform() const;

	void setLocalTransform(const basisDef& def);
	basisDef getLocalTransform() const;

	void setBasis(const basis& basis_);
	basisDef getBasis() const;

	void setLocalBasis(const basis& basis_);
	basisDef getLocalBasis() const;

	void setPosition(const vec2f& position);
	vec2f getPosition() const;

	void setLocalPosition(const vec2f& position);
	vec2f getLocalPosition() const;

	void setScale(const vec2f& scale);
	vec2f getScale() const;

	void setLocalScale(const vec2f& scale);
	vec2f getLocalScale() const;

	void setSize(const vec2f& size);
	vec2f getSize() const;

	void setLocalSize(const vec2f& size);
	vec2f getLocalSize() const;

	void setRotation(const float& angle);
	float getRotation();

	void setLocalRotation(const float& angle);
	float getLocalRotation();

	void setShift(const float& shift);
	float getShift() const;

	void setLocalShift(const float& shift);
	float getLocalShift() const;

	void setEnable(const bool& enable);
	bool isEnable() const;

	void setVisible(const bool& visible);
	bool isVisible() const;

	void setDepth(const float& depth);
	float getDepth() const;

	void setColor(const color4& color);
	color4 getColor() const;

protected:
	void initializeProperties();
	void update(float dt);
	void draw();
};

CLOSE_O2_NAMESPACE

#endif //SCENE_NODE_H
