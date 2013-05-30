#ifndef VEHICLE_H
#define VEHICLE_H

#include <vector>

#include "../rigid_object.h"

struct phVehicleComponent;
struct phCollision;
struct lPolygon;

struct phVehicle:public phRigidObject
{
	typedef std::vector<phVehicleComponent*> ComponentsList;

	ComponentsList mComponents;

	lPolygon**   mPolygonsBuffer;
	unsigned int mPolygonsBufferCount;

//functions
	phVehicle();
	~phVehicle();

	void addComponent(phVehicleComponent* component);
	void removeComponent(phVehicleComponent* component);
	void removeAllComponents();

	phVehicleComponent* getComponent(const std::string& id);

	void preSolve(float dt);
	void postSolve(float dt);

	void setPolygonsBuffer(lPolygon** buffer, unsigned int count)
	{
		mPolygonsBuffer = buffer;
		mPolygonsBufferCount = count;
	}
};

#endif //VEHICLE_H