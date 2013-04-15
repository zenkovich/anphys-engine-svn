#include "vehicle.h"

#include <algorithm>

#include "vehicle_component.h"
#include "physics/objects/rigid_object.h"
#include "physics/scenes/physics_scene.h"


phVehicle::phVehicle(phScene* scene):mPhysicsScene(scene)
{
	mPhysicsObject = new phRigidObject(scene);
	scene->addObject(mPhysicsObject);
}

phVehicle::~phVehicle()
{
	mPhysicsScene->removeObject(mPhysicsObject);
	removeAllComponents();
}

void phVehicle::addComponent( phVehicleComponent* component )
{
	mComponents.push_back(component);
	component->mVehicle = this;
}

void phVehicle::removeComponent( phVehicleComponent* component )
{
	ComponentsList::iterator fnd = std::find(mComponents.begin(), mComponents.end(), component);
	if (fnd == mComponents.end())
		return;

	safe_release(component);
	mComponents.erase(fnd);
}

void phVehicle::removeAllComponents()
{
	for (ComponentsList::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		safe_release(*it);

	mComponents.clear();
}

void phVehicle::update( float dt )
{
	for (ComponentsList::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		(*it)->update(dt);
}
