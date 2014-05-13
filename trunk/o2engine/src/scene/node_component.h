#ifndef NODE_COMPONENT_H
#define NODE_COMPONENT_H

#include "public.h"

#include "util/type_indexation.h"
#include "util/property.h"

OPEN_O2_NAMESPACE

class scNode;

class scComponent
{
protected:
	string         mId;
	shared(scNode) mOwnerNode;

	DEFINE_TYPE(scComponent);

public:
	PROPERTY(scComponent, string) id;

	scComponent(const shared(scNode)& ownerNode);
	virtual ~scComponent();

	virtual shared(scComponent) clone() const = 0;

	virtual void update(float dt) {}
	virtual void draw() {}

	void setId(const string& id);
	string getId() const;

protected:
	void initializeProperties();
};

CLOSE_O2_NAMESPACE

#endif //NODE_COMPONENT_H
