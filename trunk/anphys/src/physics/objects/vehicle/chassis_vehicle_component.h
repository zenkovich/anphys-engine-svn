#ifndef CHASSIS_VEHICLE_COMPONENT_H
#define CHASSIS_VEHICLE_COMPONENT_H

#include "util/math/mmath.h"

#include "vehicle_component.h"

struct phCollisionPoint;

struct phVehicleWheel;
struct phVehicleChassisSpring;
struct phVehicleChassisGasShock;

struct phVehicleChassisComponent:public phVehicleComponent
{
	vec3                      mLocalPosition;
	mat3x3                    mLocalAxis;
	mat3x3                    mInitialLocalAxis;
	
	vec3                      mGlobalAxisX;
	vec3                      mGlobalAxisY;
	vec3                      mGlobalAxisZ;

	float                     mInGroundDepth;
	vec3                      mGroundNormal;
	vec3                      mGroundContact;

	float                     mMaxPosition;
	float                     mMinPosition;

	float                     mWheelAngle;
	float                     mWheelXAngle;
	float                     mPosition;
	bool                      mWheelOnGround;

	float                     mWheelRadius;
	float                     mWheelVelocity;
	float                     mWheelAngVelocity;
	float                     mWheelTorque;

	float                     mWheelMass;
	float                     mWheelInvMass;
	float                     mWheelInertia;
	float                     mWheelInvInertia;

	float                     mSpringForce;
	float                     mGasShockForce;

	mat3x3                    mGlobalAxis;
	vec3                      mGlobalPosition;
	vec3                      mWheelBottomPoint;
	
	float                     mBrakeForce1;
	float                     mBrakeForce2;
	float                     mBrakeCoef1;
	float                     mBrakeCoef2;

//functions
	phVehicleChassisComponent(phVehicle* vehicle, const std::string& id);
	~phVehicleChassisComponent();

	void loadParametres(const vec3& localPos, const mat3x3& localAxis, float minPos, float maxPos, 
		                float wheelRadius, float wheelmass, float springForce, float gasShockForce, float brakeForce1,
						float brakeForce2 = 0.0f);
	
	void derivedPreSolve(float dt);
	void derivedSolve(float dt);
	void derivedPostSolve(float dt);

	void checkTestCollision();
};

#endif //CHASSIS_VEHICLE_COMPONENT_H