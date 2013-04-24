#ifndef CHASSIS_VEHICLE_COMPONENT_H
#define CHASSIS_VEHICLE_COMPONENT_H

#include "util/math/mmath.h"

#include "vehicle_component.h"

struct phVehicleWheel;
struct phVehicleChassisSpring;
struct phVehicleChassisGasShock;

struct phVehicleChassisComponent:public phVehicleComponent
{
	phVehicleWheel*           mWheel;
	phVehicleChassisSpring*   mSpring;
	phVehicleChassisGasShock* mGasShock;

	vec3                      mLocalPosition;
	mat3x3                    mLocalAxis;
	mat3x3                    mInitialLocalAxis;
	float                     mMaxPosition;
	float                     mMinPosition;
	float                     mWheelAngle;
	float                     mPosition;

//functions
	phVehicleChassisComponent(phVehicle* vehicle, const std::string& id);
	~phVehicleChassisComponent();

	void loadParametres(const vec3& localPos, const mat3x3& localAxis, float minPos, float maxPos);

	void update(float dt);
};

#endif //CHASSIS_VEHICLE_COMPONENT_H