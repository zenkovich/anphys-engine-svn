#include "physics_static_body_object_component.h"

//engine
#include "../object.h"
#include "render3d_object_component.h"

//render
#include "render/render_objects/3d/render_3d_object.h"

//physics
#include "physics/objects/static_object.h"
#include "physics/scenes/physics_scene.h"

cPhysicsStaticBodyObjectComponent::cPhysicsStaticBodyObjectComponent():
	mStaticPhysicsBody(NULL), mRenderObject(NULL), cObjectComponent()
{
}

cPhysicsStaticBodyObjectComponent::cPhysicsStaticBodyObjectComponent(phStaticObject* physicsObject):
	cObjectComponent(), mStaticPhysicsBody(physicsObject), mRenderObject(NULL)
{
}

cPhysicsStaticBodyObjectComponent::cPhysicsStaticBodyObjectComponent(cObject* ownerObject):
	cObjectComponent(ownerObject), mRenderObject(NULL), mStaticPhysicsBody(NULL)
{
}

cPhysicsStaticBodyObjectComponent::cPhysicsStaticBodyObjectComponent(cObject* ownerObject, phStaticObject* physicsObject):
	cObjectComponent(ownerObject), mStaticPhysicsBody(physicsObject), mRenderObject(NULL)
{
}

cPhysicsStaticBodyObjectComponent::~cPhysicsStaticBodyObjectComponent()
{
	if (mStaticPhysicsBody)
	{
		mStaticPhysicsBody->mScene->removeObject(mStaticPhysicsBody);
		mStaticPhysicsBody = NULL;
	}
}

void cPhysicsStaticBodyObjectComponent::updateDependencies()
{
	if (mOwnerObject) 
		mRenderObject = ((cRender3DObjectComponent*)mOwnerObject->getComponentByType(ObjectComponentType::Render3DObject))->mRender3DObject;
}

void cPhysicsStaticBodyObjectComponent::update(float dt)
{
	if (mRenderObject && mStaticPhysicsBody)
	{
		mRenderObject->mPosition = mStaticPhysicsBody->mPosition;
		mRenderObject->mOrient   = mStaticPhysicsBody->mOrient;
	}
}
