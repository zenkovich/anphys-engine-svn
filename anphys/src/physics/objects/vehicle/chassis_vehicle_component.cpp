#include "chassis_vehicle_component.h"

#include "vehicle_wheel.h"
#include "vehicle_chassis_spring.h"
#include "vehicle_chassis_gas_shock.h"

#include "vehicle.h"
#include "../rigid_object.h"

phVehicleChassisComponent::phVehicleChassisComponent( phVehicle* vehicle, const std::string& id ):
	phVehicleComponent(vehicle, id)
{
	mWheel = new phVehicleWheel;
	mSpring = new phVehicleChassisSpring;
	mGasShock = new phVehicleChassisGasShock;
}

phVehicleChassisComponent::~phVehicleChassisComponent()
{
	safe_release(mWheel);
	safe_release(mSpring);
	safe_release(mGasShock);
}

void phVehicleChassisComponent::loadParametres( const vec3& localPos, const mat3x3& localAxis, float minPos, 
	                                            float maxPos )
{
	mLocalPosition = localPos;
	mLocalAxis = localAxis;
	mMinPosition = minPos;
	mMaxPosition = maxPos;
	mPosition = maxPos;
	mWheelAngle = 0;
}

void phVehicleChassisComponent::update( float dt )
{
	mat3x3 globalAxis = mVehicle->mPhysicsObject->mOrient*mLocalAxis;
	vec3 globalPos = mLocalPosition*mVehicle->mPhysicsObject->mOrient + mVehicle->mPhysicsObject->mPosition;
}
