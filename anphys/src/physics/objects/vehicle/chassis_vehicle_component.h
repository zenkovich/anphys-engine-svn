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

	float                     mMaxPosition;
	float                     mMinPosition;

	float                     mWheelAngle;
	float                     mWheelYAngle;
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

	phCollisionPoint*         mCollisionPoint;

//functions
	phVehicleChassisComponent(phVehicle* vehicle, const std::string& id);
	~phVehicleChassisComponent();

	void loadParametres(const vec3& localPos, const mat3x3& localAxis, float minPos, float maxPos, 
		                float wheelRadius, float wheelmass, float springForce, float gasShockForce);
	
	void derivedPreSolve(float dt);
	void derivedSolve(float dt);
	void derivedPostSolve(float dt);

	void checkTestCollision();
};

#endif //CHASSIS_VEHICLE_COMPONENT_H