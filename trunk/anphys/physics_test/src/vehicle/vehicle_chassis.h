#ifndef VEHICLE_CHASSIS_H
#define VEHICLE_CHASSIS_H

#include "stuff.h"

namespace physics
{

struct phCollisionPoint;
struct Vehicle;

struct VehicleChassis
{	
	vec3             mLocalPosition;
	mat3x3           mLocalAxis;
	mat3x3           mInitialLocalAxis;
					 
	float            mMaxPosition;
	float            mMinPosition;
					 
	float            mWheelAngle;
	float            mWheelXAngle;
	float            mPosition;
	bool             mWheelOnGround;
					 
	float            mWheelRadius;
	float            mWheelVelocity;
	float            mWheelAngVelocity;
	float            mWheelTorque;
					 
	float            mWheelMass;
	float            mWheelInvMass;
	float            mWheelInertia;
	float            mWheelInvInertia;
					 
	float            mSpringForce;
	float            mGasShockForce;
					 
	mat3x3           mGlobalAxis;
	vec3             mGlobalPosition;
	vec3             mWheelBottomPoint;
					 
	float            mBrakeForce1;
	float            mBrakeForce2;
	float            mBrakeCoef1;
	float            mBrakeCoef2;

	phCollisionPoint mCollisionPoint;
	Vehicle*         mVehicle;

//functions
	void loadParametres(const vec3& localPos, const mat3x3& localAxis, float minPos, float maxPos, 
		                float wheelRadius, float wheelmass, float springForce, float gasShockForce, float brakeForce1,
						float brakeForce2 = 0.0f);
	
	void derivedPreSolve(float dt);
	void derivedSolve(float dt);
	void derivedPostSolve(float dt);

	void checkCollision();
};

}

#endif //VEHICLE_CHASSIS_H