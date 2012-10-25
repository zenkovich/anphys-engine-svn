#include "physics_rigid_body_object_component.h"

//engine
#include "object.h"

//render
#include "../../render/render_3d_object.h"

//physics
#include "../../physics/rigid_object.h"
#include "../../physics/physics_scene.h"

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
	if (mOwnerObject) mRenderObject = (grRender3DObject*)mOwnerObject->getComponentByType(ObjectComponentType::Render3DObject);
}

void cPhysicsRigidBodyObjectComponent::update(float dt)
{
	if (mRenderObject && mRigidPhysicsBody)
	{
		mRenderObject->mPosition = mRigidPhysicsBody->mPosition;
		mRenderObject->mOrient   = mRigidPhysicsBody->mOrient;
	}
}
