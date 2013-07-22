#pragma once
#include "game.h"

#define ANPHYS_ON

#ifdef ANPHYS_ON
#include "vehicle.h"
#include "vehicle_chassis.h"
#endif

class cCar{
protected:
	physics::Vehicle* Vehicle;

public:
	cCar(vec3& Position, quat& Rotation);

	vec3 getPosition();
	void getRotation(float* arr);
	void update(float dt, cEngineLandscapeVertex** vBuffer, cEngineLandscapePoly** pBuffer, int PolyCount);
};