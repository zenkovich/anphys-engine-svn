#include "physics_rigid_body_object_component.h"

//engine
#include "../object.h"
#include "render3d_object_component.h"

//render
#include "render/render_objects/3d/render_3d_object.h"

//physics
#include "physics/objects/rigid_object.h"
#include "physics/scenes/physics_scene.h"

REGIST_TYPE(cPhysicsRigidBodyObjectComponent)

cPhysicsRigidBodyObjectComponent::cPhysicsRigidBodyObjectComponent():
	mRigidPhysicsBody(NULL), mRenderObject(NULL), cObjectComponent()
{
}

cPhysicsRigidBodyObjectComponent::cPhysicsRigidBodyObjectComponent(phRigidObject* physicsObject):
	cObjectComponent(), mRigidPhysicsBody(physicsObject), mRenderObject(NULL)
{
}

cPhysicsRigidBodyObjectComponent::cPhysicsRigidBodyObjectComponent(cObject* ownerObject):
	cObjectComponent(ownerObject), mRenderObject(NULL), mRigidPhysicsBody(NULL)
{
}

cPhysicsRigidBodyObjectComponent::cPhysicsRigidBodyObjectComponent(cObject* ownerObject, phRigidObject* physicsObject):
	cObjectComponent(ownerObject), mRigidPhysicsBody(physicsObject), mRenderObject(NULL)
{
}

cPhysicsRigidBodyObjectComponent::~cPhysicsRigidBodyObjectComponent()
{
	if (mRigidPhysicsBody)
	{
		mRigidPhysicsBody->mScene->removeObject(mRigidPhysicsBody);
		mRigidPhysicsBody = NULL;
	}
}

void cPhysicsRigidBodyObjectComponent::updateDependencies()
{
	if (mOwnerObject) 
		mRenderObject = mOwnerObject->getComponent<cRender3DObjectComponent>()->mRender3DObject;
}

void cPhysicsRigidBodyObjectComponent::update(float dt)
{
	if (mRenderObject && mRigidPhysicsBody)
	{
		mRenderObject->mPosition = mRigidPhysicsBody->mPosition;
		mRenderObject->mOrient   = mRigidPhysicsBody->mOrient;
	}
}
