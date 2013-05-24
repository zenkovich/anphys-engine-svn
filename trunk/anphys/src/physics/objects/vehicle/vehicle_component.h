#ifndef VEHICLE_COMPONENT_H
#define VEHICLE_COMPONENT_H

#include <string>
#include <vector>

struct phVehicle;

struct phVehicleComponent
{
	typedef std::vector<phVehicleComponent*> ComponentsList;

	std::string         mId;

	ComponentsList      mChildComponents;
	phVehicleComponent* mParent;
	phVehicle*          mVehicle;

//functions
	phVehicleComponent(phVehicle* vehicle, const std::string& id, phVehicleComponent* parent = NULL);
	virtual ~phVehicleComponent();

	virtual void preSolve(float dt);
	virtual void solve(float dt);
	virtual void postSolve(float dt);
	
	virtual void derivedPreSolve(float dt) {}
	virtual void derivedSolve(float dt) {}
	virtual void derivedPostSolve(float dt) {}

	void addChild(phVehicleComponent* component);
	void removeChild(phVehicleComponent* component);
	void removeAllChilds();
};

#endif //VEHICLE_COMPONENT_H