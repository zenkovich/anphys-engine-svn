#ifndef VEHICLE_WHEEL_H
#define VEHICLE_WHEEL_H

#include "util/math/mmath.h"
#include "../rigid_object.h"

struct cFixedIntervalsGraphic;
struct phCollision;

struct phVehicleWheel:public phRigidObject
{
	float mRadius;
	vec3  mGroundNormal;
	bool  mOnGround;

	cFixedIntervalsGraphic* mTyreFrictionGraphic;

//functions
	phVehicleWheel():mRadius(0.2f), mOnGround(false) {}
	~phVehicleWheel() {}

	void update(float dt) {}
};

#endif //VEHICLE_WHEEL_H