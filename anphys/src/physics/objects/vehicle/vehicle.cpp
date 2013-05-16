#include "vehicle.h"

#include <algorithm>

#include "vehicle_component.h"
#include "physics/objects/rigid_object.h"
#include "physics/scenes/physics_scene.h"


phVehicle::phVehicle():phRigidObject()
{
}

phVehicle::~phVehicle()
{
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

void phVehicle::preSolve( float dt )
{
	phRigidObject::preSolve(dt);

	for (ComponentsList::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		(*it)->update(dt);
}

phVehicleComponent* phVehicle::getComponent( const std::string& id )
{
	for (ComponentsList::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		if ((*it)->mId == id)
			return *it;

	return NULL;
}
