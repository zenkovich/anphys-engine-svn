#include "chassis_vehicle_component.h"

#include "vehicle_wheel.h"
#include "vehicle_chassis_spring.h"
#include "vehicle_chassis_gas_shock.h"

#include "vehicle.h"
#include "../rigid_object.h"

#include "util/debug/render_stuff.h"
#include "util/math/mmath.h"

phVehicleChassisComponent::phVehicleChassisComponent( phVehicle* vehicle, const std::string& id ):
	phVehicleComponent(vehicle, id)
{
	mMinPosition = mMaxPosition = 0;
	mWheelAngle = mPosition = 0;
}

phVehicleChassisComponent::~phVehicleChassisComponent()
{
}

void phVehicleChassisComponent::loadParametres( const vec3& localPos, const mat3x3& localAxis, float minPos, 
	                                            float maxPos, float wheelRadius, float wheelmass, 
												float springForce, float gasShockForce, float brakeForce1,
						                        float brakeForce2 )
{
	mLocalPosition = localPos;
	mLocalAxis = localAxis;
	mInitialLocalAxis = localAxis;
	mMinPosition = minPos;
	mMaxPosition = maxPos;
	mPosition = maxPos;
	mWheelAngle = 0;
	mWheelXAngle = 0;

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

	mBrakeForce1 = brakeForce1;
	mBrakeForce2 = brakeForce2;

	mBrakeCoef1 = 0;
	mBrakeCoef2 = 0;

	mInGroundDepth = -1.0f;

	mWheelOnGround = false;
}

void phVehicleChassisComponent::derivedPreSolve( float dt )
{	
	float invDt = 1.0f/dt;
	float biasERP = 0.5f;	
	float Mu = 1.0f;

	vec3 ra = mWheelBottomPoint - mVehicle->mPosition;

	vec3 n1 = mGroundNormal;     
	vec3 w1 = mGroundNormal^ra;      

	float b = n1*n1*mVehicle->mInvMass + w1*(w1*mVehicle->mInvWorldInertia);
	float invB = 1.0f/b;
	
	float gasShock = mGasShockForce*mWheelVelocity;
	float springForce = (mPosition - mMaxPosition)*mSpringForce;	

	float shiftForce = (springForce + gasShock)*dt;

	mWheelVelocity -= shiftForce*mWheelInvMass;

	float contactDepth = mInGroundDepth - 0.01f;
	vec3 shiftImpulseVec;
	float shiftImpulse = 0;

	float biasImpulse = 0;

	if (contactDepth > 0.0f)
	{
		biasImpulse = contactDepth*invDt*biasERP;

		float a = n1*mVehicle->getVelocity() + w1*mVehicle->getAngularVelocity();
		float lambda = -a*(1.3f)*invB;

		shiftImpulse = lambda;
		shiftImpulseVec = mGroundNormal*lambda;
	}
	else
	{
		biasImpulse = 0.0f;

		shiftImpulse = 0;
	}

	shiftImpulse += shiftForce;
	shiftImpulseVec += mGlobalAxis.getYVector()*shiftForce;

	gLog->fout(1, "J = %.3f depth = %.3f spring = %.3f df = %.3f\n", shiftImpulse, contactDepth, springForce,
		mPosition - mMaxPosition);
	
	getRenderStuff().addBlueArrow(mWheelBottomPoint, mWheelBottomPoint + mGlobalAxisY*shiftForce*0.0006f);
				
	vec3 f1n1 = mGlobalAxisX;       
	vec3 f1w1 = mGlobalAxisX^ra;         
				
	float f1b = f1n1*f1n1*mVehicle->mInvMass + f1w1*(f1w1*mVehicle->mInvWorldInertia);
	float invF1b = 1.0f/f1b;
				
	vec3 f2n1 = mGlobalAxisZ;       
	vec3 f2w1 = mGlobalAxisZ^ra;        

	float f2b = f2n1*f2n1*mVehicle->mInvMass + f2w1*(f2w1*mVehicle->mInvWorldInertia);
	float invF2b = 1.0f/f2b;

	float f1a = f1n1*mVehicle->getVelocity() + f1w1*mVehicle->getAngularVelocity();
	float f1lambda = -f1a/f1b;

	float f2a = f2n1*mVehicle->getVelocity() + f2w1*mVehicle->getAngularVelocity() + 
		        mWheelAngVelocity*mWheelRadius*2.0f*3.1415926f;
	float f2lambda = -f2a/f2b;

	if (!mWheelOnGround)
	{
		f1lambda = f2lambda = 0;
	}

	float quadf = f2lambda*f2lambda + f1lambda*f1lambda;
	float maxFriction = shiftImpulse*Mu;
	bool clampedFriction = false;
	if (quadf > maxFriction*maxFriction)
	{
		float invCoef = 1.0f/sqrtf(quadf);
		f1lambda = f1lambda*invCoef*maxFriction;
		f2lambda = f2lambda*invCoef*maxFriction;
		clampedFriction = true;
	}

	if (clampedFriction)
	{
		getRenderStuff().addRedArrow(mWheelBottomPoint, mWheelBottomPoint + f1n1*f1lambda*0.1f);
		getRenderStuff().addRedArrow(mWheelBottomPoint, mWheelBottomPoint + f2n1*f2lambda*0.1f);
	}
	else
	{
		getRenderStuff().addGreenArrow(mWheelBottomPoint, mWheelBottomPoint + f1n1*f1lambda*0.1f);
		getRenderStuff().addBlueArrow(mWheelBottomPoint, mWheelBottomPoint + f2n1*f2lambda*0.1f);
	}

	float wheelTorq = -f2a*mWheelInertia;

	if (!mWheelOnGround)
		wheelTorq = 0;

	wheelTorq = sign(wheelTorq)*fmin(fabs(wheelTorq), fabs(maxFriction));

	mWheelAngVelocity += wheelTorq*mWheelInvInertia/2.0f/3.1415926f/mWheelRadius;

	vec3 fimpulse = mGlobalAxisX*f1lambda + mGlobalAxisZ*f2lambda;
	
	mVehicle->applyImpulse(mWheelBottomPoint, fimpulse + shiftImpulseVec);

	if (mWheelOnGround)
	{
		float biasA = n1*mVehicle->getBiasVelocity() + w1*mVehicle->getBiasAngularVelocity();

		float biasLambda = -(biasA - biasImpulse)*invB;

		if (biasLambda < 0.0f)
			biasLambda = 0.0f;
					
		mVehicle->applyBiasImpulse(mWheelBottomPoint, mGroundNormal*biasLambda);
	}
}

void phVehicleChassisComponent::derivedSolve( float dt )
{	
}

void phVehicleChassisComponent::derivedPostSolve( float dt )
{
	float lastPosition = mPosition;

	mPosition = fclamp(mPosition + mWheelVelocity*dt, mMaxPosition, mMinPosition);
	
	mWheelAngVelocity += mWheelTorque*mWheelInvInertia;
	mWheelTorque = 0;

	mWheelAngVelocity -= sign(mWheelAngVelocity)*
		fmin((mBrakeForce1*mBrakeCoef1 + mBrakeForce2*mBrakeCoef2)*mWheelInvInertia*dt, 
		     fabs(mWheelAngVelocity));

	mWheelXAngle += mWheelAngVelocity*dt*2.0f*3.1415926f;
	
	mLocalAxis = mInitialLocalAxis*rotatedYMatrix(mWheelAngle);

	mGlobalAxis = mLocalAxis*mVehicle->mOrient;
	mGlobalPosition = mLocalPosition*mVehicle->mOrient + mVehicle->mPosition;
	
	mGlobalAxisX = mGlobalAxis.getXVector();
	mGlobalAxisY = mGlobalAxis.getYVector();
	mGlobalAxisZ = mGlobalAxis.getZVector();
		
	mWheelBottomPoint = mGlobalPosition + mGlobalAxis.getYVector()*(mPosition - mWheelRadius);	

	checkTestCollision();

	if (mWheelOnGround)
	{
		float yproj = mGlobalAxisY*mGroundNormal;
		float posOffset = mInGroundDepth/(yproj);

		float newPosition = mPosition + posOffset;
		float minDist = mMinPosition + newPosition;

		mPosition = mMinPosition + fmin(0.0f, minDist);

		mWheelVelocity = (mPosition - lastPosition)/dt;

		if (minDist > 0)
		{
			mInGroundDepth = yproj*minDist;
		}
		else
		{
			mInGroundDepth = -1.0f;
		}
	}

	mWheelBottomPoint = mGlobalPosition + mGlobalAxis.getYVector()*(mPosition - mWheelRadius);	

	getRenderStuff().addRedArrow(mGlobalPosition, mGlobalPosition + mGlobalAxis.getXVector()*0.5f);
	getRenderStuff().addGreenArrow(mGlobalPosition, mGlobalPosition + mGlobalAxis.getYVector()*0.5f);
	getRenderStuff().addBlueArrow(mGlobalPosition, mGlobalPosition + mGlobalAxis.getZVector()*0.5f);
		
	getRenderStuff().addBlueCube(mWheelBottomPoint);
	getRenderStuff().addRedCube(mGroundContact);

	getRenderStuff().addBlueArrow(mWheelBottomPoint, mGroundContact);

	vec3 wheelPos = mGlobalPosition + mGlobalAxis.getYVector()*mPosition;

	int segments = 10;
	for (int i = 0; i < segments; i++)
	{
		float angle = rad((float)i*360.0f/(float)segments) + mWheelXAngle;
		float sn = sinf(angle), cs = cosf(angle);

		vec3 segPoint = wheelPos + mGlobalAxisY*sn*mWheelRadius + mGlobalAxisZ*cs*mWheelRadius;

		getRenderStuff().addBlueArrow(wheelPos, segPoint);
	}
}

void phVehicleChassisComponent::checkTestCollision()
{
	/*if (mWheelBottomPoint.y < 0)
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

	return;*/

	vec3 bottomPoint = mWheelBottomPoint;
	vec3 dir = mGlobalAxis.getYVector()*-1.0f;
	vec3 topPoint = mGlobalPosition;

	lPolygon** polygonsBuffer = mVehicle->mPolygonsBuffer;
	unsigned int polygonsCount = mVehicle->mPolygonsBufferCount;

	vec3 point, normal;
	float depth;

	float minDepth = FLT_MAX;
	mWheelOnGround = false;
	for (unsigned int i = 0; i < polygonsCount; i++)
	{
		if (polygonsBuffer[i]->norm*dir < 0)
		{
			if (polygonsBuffer[i]->isIntersect(bottomPoint, &point, &normal, &depth))
			{
				if (depth < minDepth)
				{
					mWheelOnGround = true;

					mGroundNormal = normal;
					mGroundContact = point;
					mInGroundDepth = depth;
				}
			}
		}
	}

	if (!mWheelOnGround)
	{
		mInGroundDepth = -1.0f;
	}
}
