#include "vehicle_component.h"

#include <algorithm>

#include "vehicle.h"
#include "util/memory/mem_utils.h"

phVehicleComponent::phVehicleComponent( phVehicle* vehicle, const std::string& id, 
	                                    phVehicleComponent* parent /*= NULL*/ )
{
	mId  = id;
	mVehicle = vehicle;
	if (parent)
		parent->addChild(this);
}

phVehicleComponent::~phVehicleComponent()
{
	removeAllChilds();
}

void phVehicleComponent::addChild( phVehicleComponent* component )
{
	mChildComponents.push_back(component);
	component->mParent = this;
}

void phVehicleComponent::removeChild( phVehicleComponent* component )
{
	ComponentsList::iterator fnd = std::find(mChildComponents.begin(), mChildComponents.end(), component);
	
	if (fnd == mChildComponents.end())
		return;

	mChildComponents.erase(fnd);
	safe_release(component);
}

void phVehicleComponent::removeAllChilds()
{
	for (ComponentsList::iterator it = mChildComponents.begin(); it != mChildComponents.end(); ++it)
		safe_release(*it);

	mChildComponents.clear();
}

void phVehicleComponent::preSolve( float dt )
{
	derivedPreSolve(dt);

	for (ComponentsList::iterator it = mChildComponents.begin(); it != mChildComponents.end(); ++it)
		(*it)->preSolve(dt);
}

void phVehicleComponent::solve( float dt )
{
	derivedSolve(dt);

	for (ComponentsList::iterator it = mChildComponents.begin(); it != mChildComponents.end(); ++it)
		(*it)->solve(dt);
}

void phVehicleComponent::postSolve( float dt )
{
	derivedPostSolve(dt);

	for (ComponentsList::iterator it = mChildComponents.begin(); it != mChildComponents.end(); ++it)
		(*it)->postSolve(dt);
}
