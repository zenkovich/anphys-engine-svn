#include "object.h"

#include <algorithm>
#include "../../util/utils.h"

//engine
#include "object_component.h"
#include "physics_rigid_body_object_component.h"
#include "render3d_object_component.h"

//render
#include "../../render/render_3d_object.h"

cObject::cObject()
{
}

cObject::~cObject()
{
	removeAllComponents();
}

void cObject::update(float dt)
{
	for (ObjectComponentsList::iterator it = mComponents.begin(); it != mComponents.end(); it++)
		(*it)->update(dt);
}

void cObject::draw()
{
	for (ObjectComponentsList::iterator it = mComponents.begin(); it != mComponents.end(); it++)
		(*it)->draw();
}

cObjectComponent* cObject::addComponent(cObjectComponent* component)
{
	mComponents.push_back(component);
	component->mOwnerObject = this;
	component->updateDependencies();
	return component;
}

bool cObject::removeComponent(cObjectComponent* component)
{
	ObjectComponentsList::iterator fnd = std::find(mComponents.begin(), mComponents.end(), component);
	if (fnd == mComponents.end()) return false;

	safe_release(component);
	mComponents.erase(fnd);

	return true;
}

void cObject::removeAllComponents()
{
	for (ObjectComponentsList::iterator it = mComponents.begin(); it != mComponents.end(); it++)
		safe_release(*it);
}

void cObject::updateComponentsDependencies()
{	
	for (ObjectComponentsList::iterator it = mComponents.begin(); it != mComponents.end(); it++)
		(*it)->updateDependencies();
}

cObjectComponent* cObject::getComponentByType( ObjectComponentType::types componentType )
{
	for (ObjectComponentsList::iterator it = mComponents.begin(); it != mComponents.end(); it++)
		if ((*it)->getType() == componentType) return *it;

	return NULL;
}

phRigidObject* cObject::getPhysicsRigidBody()
{
	cPhysicsRigidBodyObjectComponent* physicsComponent = 
		static_cast<cPhysicsRigidBodyObjectComponent*>(getComponentByType(ObjectComponentType::PhysicsRigidBody));
	if (physicsComponent) return physicsComponent->mRigidPhysicsBody;

	return NULL;
}

vec3& cObject::position()
{
	cRender3DObjectComponent* renderObjectComponent = 
		static_cast<cRender3DObjectComponent*>(getComponentByType(ObjectComponentType::Render3DObject));

	return renderObjectComponent->mRender3DObject->mPosition;
}

mat3x3& cObject::orientation()
{	
	cRender3DObjectComponent* renderObjectComponent = 
		static_cast<cRender3DObjectComponent*>(getComponentByType(ObjectComponentType::Render3DObject));

	return renderObjectComponent->mRender3DObject->mOrient;
}
