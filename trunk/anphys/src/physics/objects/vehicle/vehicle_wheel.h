#ifndef VEHICLE_WHEEL_H
#define VEHICLE_WHEEL_H

#include "util/math/mmath.h"

struct cFixedIntervalsGraphic;

struct phVehicleWheel
{
	vec3  mGlobalPosition;
	mat3x3 mGlobalOrient;

	float mRadius;
	float mAngle;
	float mRPM;
	float mTorque;

	cFixedIntervalsGraphic* mTyreFrictionGraphic;

//functions
	phVehicleWheel();
	~phVehicleWheel();

	void update(float dt);
};

#endif //VEHICLE_WHEEL_H