#ifndef VEHICLE_WHEEL_H
#define VEHICLE_WHEEL_H

#include "util/math/mmath.h"
#include "../rigid_object.h"

struct cFixedIntervalsGraphic;

struct phVehicleWheel:public phRigidObject
{
	float mRadius;
	vec3  mGroundNormal;

	cFixedIntervalsGraphic* mTyreFrictionGraphic;

//functions
	phVehicleWheel();
	~phVehicleWheel();

	void update(float dt);
};

#endif //VEHICLE_WHEEL_H