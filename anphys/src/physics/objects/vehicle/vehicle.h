#ifndef VEHICLE_H
#define VEHICLE_H

#include <vector>

struct phRigidObject;
struct phVehicleComponent;
struct phScene;

struct phVehicle
{
	typedef std::vector<phVehicleComponent*> ComponentsList;

	phScene*       mPhysicsScene;
	phRigidObject* mPhysicsObject;

	ComponentsList mComponents;

//functions
	phVehicle(phScene* scene);
	~phVehicle();

	void addComponent(phVehicleComponent* component);
	void removeComponent(phVehicleComponent* component);
	void removeAllComponents();

	void update(float dt);
};

#endif //VEHICLE_H