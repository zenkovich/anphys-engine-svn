#ifndef NODE_COMPONENT_H
#define NODE_COMPONENT_H

#include "public.h"

#include "util/type_indexation.h"
#include "util/property.h"
#include "util/callback.h"

OPEN_O2_NAMESPACE

class scNode;

class scComponent: public cShareObject
{
	friend class scNode;

protected:
	shared<scNode>    mOwnerNode;
	bool              mEnable;

	DEFINE_TYPE(scComponent);

public:
	PROPERTY(scComponent, bool) enable;

	cCallbackChain onEnalbedEvent;
	cCallbackChain onDisabledEvent;

	scComponent(const shared<scNode>& ownerNode);
	virtual ~scComponent();

	virtual shared<scComponent> clone() const = 0;

	virtual void update(float dt) {}
	virtual void draw() {}

	shared<scNode> node() const;

	void setEnable(const bool& enable);
	bool isEnable() const;

protected:
	void initializeProperties();
};

CLOSE_O2_NAMESPACE

#endif //NODE_COMPONENT_H
