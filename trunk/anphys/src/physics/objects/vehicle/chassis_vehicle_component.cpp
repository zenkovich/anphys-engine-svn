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
	mInitialLocalAxis = localAxis;
	mMinPosition = minPos;
	mMaxPosition = maxPos;
	mPosition = maxPos;
	mWheelAngle = 0;
}

void phVehicleChassisComponent::update( float dt )
{
	mLocalAxis = mInitialLocalAxis*rotatedYMatrix(mWheelAngle);

	mat3x3 globalAxis = mVehicle->mPhysicsObject->mOrient*mLocalAxis;
	vec3 globalAxisPos = mLocalPosition*mVehicle->mPhysicsObject->mOrient + mVehicle->mPhysicsObject->mPosition;

	vec3 globalWheelPos = globalAxisPos + globalAxis.getYVector()*fmax(mPosition, mMinPosition);

	float springForce = mSpring->getForce(mMaxPosition - mPosition);
	
	float wheelVelocityByAxis = mWheel->mVelocity*globalAxis.getYVector();

	float gasShockForce = mGasShock->getForce(wheelVelocityByAxis);

	vec3 springShockImpulse = globalAxis.getYVector()*(springForce + gasShockForce)*dt;

	vec3 wheelYImpulse = springShockImpulse;

	vec3 globalWheelBottomPos = globalWheelPos + globalAxis.getYVector()*mWheel->mRadius;
	vec3 r = globalWheelBottomPos - mVehicle->mPhysicsObject->mPosition;

	if (mPosition < mMinPosition)
	{
		mPosition = mMinPosition;

		wheelYImpulse = 
			mVehicle->mPhysicsObject->mVelocity*globalAxis.getYVector()*mVehicle->mPhysicsObject->mMass + 
			mVehicle->mPhysicsObject->mAngularVelocity*
			((r^globalAxis.getYVector())*mVehicle->mPhysicsObject->mWorldInertia);

		vec3 groundPenetration = mWheel->mGroundNormal*
			(globalAxis.getYVector()*(mMinPosition - mPosition)*mWheel->mGroundNormal);

		mVehicle->mPhysicsObject->applyBiasImpulse(globalWheelBottomPos, groundPenetration/dt);
	}

	mVehicle->mPhysicsObject->applyImpulse(globalWheelBottomPos, wheelYImpulse);

	float yForce = mWheel->mGroundNormal*wheelYImpulse*dt;

	vec3 wheelVelocity = mVehicle->mPhysicsObject->mVelocity + r^mVehicle->mPhysicsObject->mAngularVelocity;
	float xWheelVelocity = wheelVelocity*globalAxis.getXVector();
	float zWheelvelocity = wheelVelocity*globalAxis.getZVector();

	float xForce = xWheelVelocity*yForce;
	float zForce = zWheelvelocity*yForce;

	vec3 wheelImpulse = globalAxis.getXVector()*xForce + globalAxis.getZVector()*zForce;

	mVehicle->mPhysicsObject->applyImpulse(globalWheelBottomPos, wheelImpulse*dt);
}
