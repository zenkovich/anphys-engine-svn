#ifndef ENGINE_PHYSICS_STATIC_BODY_OBJECT_COMPONENT_H
#define ENGINE_PHYSICS_STATIC_BODY_OBJECT_COMPONENT_H

#include "engine/scene/object_component.h"
#include "util/utils.h"

struct phStaticObject;
struct grRender3DObject;

struct cPhysicsStaticBodyObjectComponent:public cObjectComponent
{
	phStaticObject*   mStaticPhysicsBody;

	grRender3DObject* mRenderObject;


	cPhysicsStaticBodyObjectComponent();
	cPhysicsStaticBodyObjectComponent(phStaticObject* physicsObject);
	cPhysicsStaticBodyObjectComponent(cObject* ownerObject);
	cPhysicsStaticBodyObjectComponent(cObject* ownerObject, phStaticObject* physicsObject);
	virtual ~cPhysicsStaticBodyObjectComponent();

	void updateDependencies();

	void update(float dt);

	ObjectComponentType::types getType() { return ObjectComponentType::PhysicsStaticBody; }
};

#endif //ENGINE_PHYSICS_STATIC_BODY_OBJECT_COMPONENT_H