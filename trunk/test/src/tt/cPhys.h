#pragma once
#include "game.h"

#define ANPHYS_ON

#ifdef ANPHYS_ON
#include "vehicle.h"
#include "vehicle_chassis.h"
#endif

class cCar{
protected:

	float mMaxDeltaTime;
	float mAccumDT;

public:
	physics::Vehicle* Vehicle;

	cCar(vec3& Position, quat& Rotation);

	vec3 getPosition();
	void getRotation(float* arr);

	void setSteerWheelAngle(float angle);
	void setThrottleCoef(float coef);
	void setBrakesCoef(float coef);
	void setHandBrakeCoef(float coef);

	void gearUp();
	void gearDown();

	vec3 getWheelPos(int i);
	void getWheelOrient(float* matr, int idx);

	void reset(vec3& pos);

	void setMaxDeltaTime(float maxDt) { mMaxDeltaTime = maxDt; }

	void update(float dt, cEngineLandscapeVertex** vBuffer, cEngineLandscapePoly** pBuffer, int PolyCount);
};