#ifndef ENGINE_PHYSICS_RIGID_BODY_OBJECT_COMPONENT_H
#define ENGINE_PHYSICS_RIGID_BODY_OBJECT_COMPONENT_H

#include "../object_component.h"
#include "util/utils.h"

struct phRigidObject;
struct grRender3DObject;

struct cPhysicsRigidBodyObjectComponent:public cObjectComponent
{
	phRigidObject*    mRigidPhysicsBody;

	grRender3DObject* mRenderObject;


	cPhysicsRigidBodyObjectComponent();
	cPhysicsRigidBodyObjectComponent(phRigidObject* physicsObject);
	cPhysicsRigidBodyObjectComponent(cObject* ownerObject);
	cPhysicsRigidBodyObjectComponent(cObject* ownerObject, phRigidObject* physicsObject);
	virtual ~cPhysicsRigidBodyObjectComponent();

	void updateDependencies();

	void update(float dt);

	ObjectComponentType::types getType() { return ObjectComponentType::PhysicsRigidBody; }
};

#endif //ENGINE_PHYSICS_RIGID_BODY_OBJECT_COMPONENT_H