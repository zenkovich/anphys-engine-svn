#ifndef VEHICLE_H
#define VEHICLE_H

#include <vector>

#include "../rigid_object.h"

struct phVehicleComponent;

struct phVehicle:public phRigidObject
{
	typedef std::vector<phVehicleComponent*> ComponentsList;

	ComponentsList mComponents;

//functions
	phVehicle();
	~phVehicle();

	void addComponent(phVehicleComponent* component);
	void removeComponent(phVehicleComponent* component);
	void removeAllComponents();

	phVehicleComponent* getComponent(const std::string& id);

	void preSolve(float dt);
};

#endif //VEHICLE_H