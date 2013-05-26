#include "chassis_vehicle_component.h"

#include "vehicle_wheel.h"
#include "vehicle_chassis_spring.h"
#include "vehicle_chassis_gas_shock.h"

#include "vehicle.h"
#include "../rigid_object.h"

#include "util/debug/render_stuff.h"

phVehicleChassisComponent::phVehicleChassisComponent( phVehicle* vehicle, const std::string& id ):
	phVehicleComponent(vehicle, id)
{
	mMinPosition = mMaxPosition = 0;
	mWheelAngle = mPosition = 0;

	mCollisionPoint = new phCollisionPoint();
}

phVehicleChassisComponent::~phVehicleChassisComponent()
{
	safe_release(mCollisionPoint);
}

void phVehicleChassisComponent::loadParametres( const vec3& localPos, const mat3x3& localAxis, float minPos, 
	                                            float maxPos, float wheelRadius, float wheelmass, 
												float springForce, float gasShockForce )
{
	mLocalPosition = localPos;
	mLocalAxis = localAxis;
	mInitialLocalAxis = localAxis;
	mMinPosition = minPos;
	mMaxPosition = maxPos;
	mPosition = maxPos;
	mWheelAngle = 0;
	mWheelYAngle = 0;

	mSpringForce = springForce;
	mGasShockForce = gasShockForce;

	mWheelVelocity = 0;
	mWheelAngVelocity = 0;
	mWheelRadius = wheelRadius;
	mWheelTorque = 0;

	mWheelMass = wheelmass;
	mWheelInvMass = 1.0f/mWheelMass;

	mWheelInertia = 0.5f*mWheelMass*mWheelRadius*mWheelRadius;
	mWheelInvInertia = 1.0f/mWheelInertia;

	mWheelOnGround = false;
}

void phVehicleChassisComponent::derivedPreSolve( float dt )
{	
	if (!mWheelOnGround)
		return;

	float invDt = 1.0f/dt;

	float biasERP = 0.5f;

	float gasShock = mGasShockForce*mWheelVelocity;
	float springForce = ((mPosition - mMaxPosition)*mSpringForce + gasShock)*dt;	

	mWheelVelocity -= springForce*mWheelInvMass;

	float contactDepth = mCollisionPoint->mDepth - 0.01f;
	if (contactDepth > 0.0f)
	{
		mCollisionPoint->mBiasImpulse = contactDepth*invDt*biasERP;
	}
	else
	{
		mCollisionPoint->mBiasImpulse = 0.0f;

		mCollisionPoint->J = 0;
	}

	mCollisionPoint->J += springForce;
	
	getRenderStuff().addBlueArrow(mWheelBottomPoint, mWheelBottomPoint + mCollisionPoint->mNormal*mCollisionPoint->J);

	mCollisionPoint->t1 = mGlobalAxis.getXVector();
	mCollisionPoint->t2 = mGlobalAxis.getZVector();
			
	vec3 imp = mCollisionPoint->mNormal*mCollisionPoint->J + mCollisionPoint->t1*mCollisionPoint->Jf1 + 
															 mCollisionPoint->t2*mCollisionPoint->Jf2;
			
	mVehicle->applyImpulse(mCollisionPoint->mPoint, imp);

	vec3 ra = mCollisionPoint->mPoint - mVehicle->mPosition;
			
	vec3 n1 = mCollisionPoint->mNormal;            mCollisionPoint->n1 = n1;
	vec3 w1 = mCollisionPoint->mNormal^ra;         mCollisionPoint->w1 = w1;

	float b = n1*n1*mVehicle->mInvMass + w1*(w1*mVehicle->mInvWorldInertia);
	mCollisionPoint->B = 1.0f/b;

				
	vec3 f1n1 = mCollisionPoint->t1;            mCollisionPoint->f1n1 = f1n1;
	vec3 f1w1 = mCollisionPoint->t1^ra;         mCollisionPoint->f1w1 = f1w1;
				
	float f1b = f1n1*f1n1*mVehicle->mInvMass + f1w1*(f1w1*mVehicle->mInvWorldInertia);
	mCollisionPoint->Bf1 = 1.0f/f1b;
				
	vec3 f2n1 = mCollisionPoint->t2;            mCollisionPoint->f2n1 = f2n1;
	vec3 f2w1 = mCollisionPoint->t2^ra;         mCollisionPoint->f2w1 = f2w1;

	float f2b = f2n1*f2n1*mVehicle->mInvMass + f2w1*(f2w1*mVehicle->mInvWorldInertia);
	mCollisionPoint->Bf2 = 1.0f/f2b;
}

void phVehicleChassisComponent::derivedSolve( float dt )
{
	if (!mWheelOnGround)
		return;

	float E = 0.5f, Mu = 1.0f, biasERP = 0.8f;

	vec3 ra = mCollisionPoint->mPoint - mVehicle->getPos();
			
	vec3 n1 = mCollisionPoint->n1;
	vec3 w1 = mCollisionPoint->w1;

	float a = n1*mVehicle->getVelocity() + w1*mVehicle->getAngularVelocity();
	float b = mCollisionPoint->B;

	float lambda = -a*(E + 1.0f)*b;
	

	/*float gasShock = mGasShockForce*mWheelVelocity;
	float springForce = ((mPosition - mMaxPosition)*mSpringForce + gasShock*1000000.0f)*dt;
	
	getRenderStuff().addBlueArrow(mWheelBottomPoint, mWheelBottomPoint + mCollisionPoint->mNormal*mWheelVelocity);
	getRenderStuff().addBlueArrow(mWheelBottomPoint, mWheelBottomPoint + mCollisionPoint->mNormal*gasShock);*/

	if (mCollisionPoint->mDepth < 0)
	{
		lambda = 0;
	}
	else
	{
		mCollisionPoint->J += lambda;
		if(mCollisionPoint->J < 0.0f)
		{
			lambda += (0.0f - mCollisionPoint->J);
			mCollisionPoint->J = 0.0f;
		}
		//mCollisionPoint->J = lambda;

		mVehicle->applyImpulse(mCollisionPoint->mPoint, mCollisionPoint->mNormal*lambda);
	}
				
	vec3 f1n1 = mCollisionPoint->f1n1;
	vec3 f1w1 = mCollisionPoint->f1w1;
				
	float f1a = f1n1*mVehicle->getVelocity() + f1w1*mVehicle->getAngularVelocity();
	float f1b = mCollisionPoint->Bf1;

	float f1lambda = -f1a*f1b;
				
	vec3 f2n1 = mCollisionPoint->f2n1;
	vec3 f2w1 = mCollisionPoint->f2w1;
				
	float f2a = f2n1*mVehicle->getVelocity() + f2w1*mVehicle->getAngularVelocity();
	float f2b = mCollisionPoint->Bf2;

	float f2lambda = -f2a*f2b;
				
	float f1accumulated = f1lambda + mCollisionPoint->Jf1;
	float f2accumulated = f2lambda + mCollisionPoint->Jf2;
	float maxfriction = mCollisionPoint->J*Mu;

	if (f1accumulated*f1accumulated + f2accumulated*f2accumulated > maxfriction*maxfriction)
	{
		float invLen = 1.0f/sqrtf(f1lambda*f1lambda + f2lambda*f2lambda);
		f1lambda = f1lambda*invLen*maxfriction - mCollisionPoint->Jf1;
		f2lambda = f2lambda*invLen*maxfriction - mCollisionPoint->Jf2;
	}
				
	mCollisionPoint->Jf1 += f1lambda;
	mCollisionPoint->Jf2 += f2lambda;

	vec3 Jf = mCollisionPoint->t1*f1lambda + mCollisionPoint->t2*f2lambda;

	mVehicle->applyImpulse(mCollisionPoint->mPoint, Jf);

	float biasA = n1*mVehicle->getBiasVelocity() + w1*mVehicle->getBiasAngularVelocity();

	float biasLambda = -(biasA - mCollisionPoint->mBiasImpulse)*b;
					
	mVehicle->applyBiasImpulse(mCollisionPoint->mPoint, mCollisionPoint->mNormal*biasLambda);
}

void phVehicleChassisComponent::derivedPostSolve( float dt )
{
	float lastPosition = mPosition;

	mPosition += mWheelVelocity*dt;

	if (mPosition > mMinPosition)
		mPosition = mMinPosition;

	if (mPosition < mMaxPosition)
		mPosition = mMaxPosition;

	//mPosition = mMaxPosition;

	mLocalAxis = mInitialLocalAxis*rotatedYMatrix(mWheelAngle);

	mGlobalAxis = mVehicle->mOrient*mLocalAxis;
	mGlobalPosition = mLocalPosition*mVehicle->mOrient + mVehicle->mPosition;
	
	vec3 xvec = mGlobalAxis.getXVector();
	vec3 yvec = mGlobalAxis.getYVector();
	vec3 zvec = mGlobalAxis.getZVector();

	//mPosition = mMaxPosition;
	
	mWheelBottomPoint = mGlobalPosition + mGlobalAxis.getYVector()*(mPosition - mWheelRadius);	

	checkTestCollision();

	if (mWheelOnGround)
	{
		float yproj = yvec*mCollisionPoint->mNormal;
		float posOffset = mCollisionPoint->mDepth/(yproj);

		float newPosition = mPosition + posOffset;
		float minDist = mMinPosition + newPosition;

		mPosition = mMinPosition + fmin(0.0f, minDist);

		mWheelVelocity = (mPosition - lastPosition)/dt;

		gLog->fout(1, "pos %.3f dist %.3f ofs %.3f\n", mPosition, minDist, posOffset);

		if (minDist > 0)
		{
			mCollisionPoint->mDepth = yproj*minDist;
		}
		else
		{
			mCollisionPoint->mDepth = -1.0f;
			mCollisionPoint->reset();
		}
	}

	mWheelBottomPoint = mGlobalPosition + mGlobalAxis.getYVector()*(mPosition - mWheelRadius);	

	getRenderStuff().addRedArrow(mGlobalPosition, mGlobalPosition + mGlobalAxis.getXVector()*0.5f);
	getRenderStuff().addGreenArrow(mGlobalPosition, mGlobalPosition + mGlobalAxis.getYVector()*0.5f);
	getRenderStuff().addBlueArrow(mGlobalPosition, mGlobalPosition + mGlobalAxis.getZVector()*0.5f);
		
	getRenderStuff().addBlueCube(mWheelBottomPoint);

	vec3 wheelPos = mGlobalPosition + mGlobalAxis.getYVector()*mPosition;

	int segments = 10;
	for (int i = 0; i < segments; i++)
	{
		float angle = rad((float)i*360.0f/(float)segments + mWheelYAngle);
		float sn = sinf(angle), cs = cosf(angle);

		vec3 segPoint = wheelPos + yvec*sn*mWheelRadius + zvec*cs*mWheelRadius;

		getRenderStuff().addRedArrow(wheelPos, segPoint);
	}
}

void phVehicleChassisComponent::checkTestCollision()
{
	if (mWheelBottomPoint.y < 0)
	{
		mWheelOnGround = true;

		mCollisionPoint->mNormal = vec3(0, 1, 0);
		mCollisionPoint->mPoint = vec3(mWheelBottomPoint.x, 0, mWheelBottomPoint.z);
		mCollisionPoint->mDepth = -mWheelBottomPoint.y;

		getRenderStuff().addBlueArrow(mWheelBottomPoint, mWheelBottomPoint + mCollisionPoint->mNormal*mCollisionPoint->mDepth);
	}
	else
	{
		mWheelOnGround = false;
		mCollisionPoint->reset();
	}
}
