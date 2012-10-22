#ifndef ENGINE_OBJECT_COMPONENT_H
#define ENGINE_OBJECT_COMPONENT_H

struct cObject;

struct cObjectComponent
{
	cObject* mOwnerObject;

	cObjectComponent():mOwnerObject(0) {}
	cObjectComponent(cObject* ownerObject):mOwnerObject(ownerObject) {}
	virtual ~cObjectComponent() {}

	virtual void draw() {}
	virtual void update(float dt) {}

	virtual void updateDependencies() {}
};

#endif