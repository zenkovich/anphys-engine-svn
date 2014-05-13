#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include "public.h"

#include "node_component.h"
#include "util/property.h"
#include "util/math/basis.h"

OPEN_O2_NAMESPACE

class scNode
{
	typedef vector< shared(scComponent) > ComponentsVector;

protected:
	string           mId;
	basis            mTransform;
	basisDef         mTransformDef;
	shared(scNode)   mParent;
	ComponentsVector mComponents;

public:
	PROPERTY(scNode, string)         id;
	PROPERTY(scNode, shared(scNode)) parent;
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

	
	scNode();
	scNode(const scNode& node);
	virtual ~scNode();

	virtual shared(scNode) clone() const;

	scNode& operator=(const scNode& node);

	virtual void update(float dt);
	virtual void draw();

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

	void setRotation(float angle);
	float getRotation();

	void setLocalRotation(float angle);
	float getLocalRotation();

	void setShift(float shift);
	float getShift() const;

	void setLocalShift(float shift);
	float getLocalShift() const;

protected:
	void initializeProperties();
	virtual void localUpdate(float dt);
	virtual void localDraw();
};

CLOSE_O2_NAMESPACE

#endif //SCENE_NODE_H
