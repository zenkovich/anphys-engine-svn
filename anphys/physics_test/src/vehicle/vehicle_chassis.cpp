#include "vehicle_chassis.h"

#include "vehicle.h"

namespace physics
{

void VehicleChassis::loadParametres( const vec3& localPos, const mat3x3& localAxis, float minPos, float maxPos, float wheelRadius, float wheelmass, float springForce, float gasShockForce, float brakeForce1, float brakeForce2 /*= 0.0f*/ )
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

	mMinPosition = mMaxPosition = 0;
	mWheelAngle = mPosition = 0;

	mWheelOnGround = false;
}

void VehicleChassis::loadParametres( float* localPosVec, float* localAxisMatrix, float minPos, float maxPos, 
	                                 float wheelRadius, float wheelmass, float springForce, float gasShockForce, 
									 float brakeForce1, float brakeForce2 /*= 0.0f*/ )
{
	loadParametres(vmask(localPosVec), mmask(localAxisMatrix), minPos, maxPos, wheelRadius, wheelmass, springForce, 
		gasShockForce, brakeForce1, brakeForce2);
}

void VehicleChassis::derivedPreSolve( float dt )
{
	if (!mWheelOnGround)
		return;

	float invDt = 1.0f/dt;
	float biasERP = 0.5f;	

	vec3 ra = mCollisionPoint.mPoint - mVehicle->mPosition;

	vec3 n1 = mCollisionPoint.mNormal;            mCollisionPoint.n1 = n1;
	vec3 w1 = mCollisionPoint.mNormal^ra;         mCollisionPoint.w1 = w1;

	float b = n1*n1*mVehicle->mInvMass + w1*(w1*mVehicle->mInvWorldInertia);
	mCollisionPoint.B = 1.0f/b;
	
	float gasShock = mGasShockForce*mWheelVelocity;
	float springForce = (mPosition - mMaxPosition)*mSpringForce;	
	float shiftForce = (springForce + gasShock)*dt;

	mWheelVelocity -= shiftForce*mWheelInvMass;

	float contactDepth = mCollisionPoint.mDepth - 0.01f;
	vec3 imp1;
	if (contactDepth > 0.0f)
	{
		mCollisionPoint.mBiasImpulse = contactDepth*invDt*biasERP;

		float a = n1*mVehicle->mVelocity + w1*mVehicle->mAngularVelocity;
		float lambda = -a*(1.3f)*mCollisionPoint.B;

		mCollisionPoint.J = lambda;
		imp1 = mCollisionPoint.mNormal*mCollisionPoint.J;
	}
	else
	{
		mCollisionPoint.mBiasImpulse = 0.0f;

		mCollisionPoint.J = 0;
	}
	mCollisionPoint.J += shiftForce;
	imp1 += mGlobalAxis.getYVector()*shiftForce;

	vec3 imp = imp1 + mCollisionPoint.t1*mCollisionPoint.Jf1 + 
			   mCollisionPoint.t2*mCollisionPoint.Jf2;
			
	mVehicle->applyImpulse(mCollisionPoint.mPoint, imp);
	

	mCollisionPoint.t1 = mGlobalAxis.getZVector()^mCollisionPoint.mNormal;
	mCollisionPoint.t2 = mCollisionPoint.mNormal^mGlobalAxis.getXVector();
				
	vec3 f1n1 = mCollisionPoint.t1;            mCollisionPoint.f1n1 = f1n1;
	vec3 f1w1 = mCollisionPoint.t1^ra;         mCollisionPoint.f1w1 = f1w1;
				
	float f1b = f1n1*f1n1*mVehicle->mInvMass + f1w1*(f1w1*mVehicle->mInvWorldInertia);
	mCollisionPoint.Bf1 = 1.0f/f1b;
				
	vec3 f2n1 = mCollisionPoint.t2;            mCollisionPoint.f2n1 = f2n1;
	vec3 f2w1 = mCollisionPoint.t2^ra;         mCollisionPoint.f2w1 = f2w1;

	float f2b = f2n1*f2n1*mVehicle->mInvMass + f2w1*(f2w1*mVehicle->mInvWorldInertia);
	mCollisionPoint.Bf2 = 1.0f/f2b;
}

void VehicleChassis::derivedSolve( float dt )
{
	if (!mWheelOnGround)
		return;

	float E = 0.5f, Mu = 1.1f, biasERP = 0.8f;

	vec3 ra = mCollisionPoint.mPoint - mVehicle->mPosition;
			
	vec3 n1 = mCollisionPoint.n1;
	vec3 w1 = mCollisionPoint.w1;

	float b = mCollisionPoint.B;
				
	vec3 f1n1 = mCollisionPoint.f1n1;
	vec3 f1w1 = mCollisionPoint.f1w1;
				
	float f1a = f1n1*mVehicle->mVelocity + f1w1*mVehicle->mAngularVelocity;
	float f1b = mCollisionPoint.Bf1;

	float f1lambda = -f1a*f1b;
				
	vec3 f2n1 = mCollisionPoint.f2n1;
	vec3 f2w1 = mCollisionPoint.f2w1;
				
	float f2a = f2n1*mVehicle->mVelocity + f2w1*mVehicle->mAngularVelocity + 
		        mWheelAngVelocity*mWheelRadius*2.0f*3.1415926f;
	float f2b = mCollisionPoint.Bf2;

	float f2lambda = -f2a*f2b;

	float fl = f2lambda*f2lambda + f1lambda*f1lambda;
	float maxFriction = mCollisionPoint.J*Mu;
	bool clampedFriction = false;
	if (fl > maxFriction*maxFriction)
	{
		float invCoef = 1.0f/sqrtf(fl);
		f1lambda = f1lambda*invCoef*maxFriction;
		f2lambda = f2lambda*invCoef*maxFriction;
		clampedFriction = true;
	}

	float wheelTorq = -f2a*mWheelInertia;
	wheelTorq = sign(wheelTorq)*fmin(fabs(wheelTorq), fabs(mCollisionPoint.J*Mu));

	mWheelAngVelocity += wheelTorq*mWheelInvInertia/2.0f/3.1415926f/mWheelRadius;
				
	/*float f1accumulated = f1lambda + mCollisionPoint.Jf1;
	float f2accumulated = f2lambda + mCollisionPoint.Jf2;
	float maxfriction = mCollisionPoint.J*Mu;

	if (f1accumulated*f1accumulated + f2accumulated*f2accumulated > maxfriction*maxfriction)
	{
		float invLen = 1.0f/sqrtf(f1lambda*f1lambda + f2lambda*f2lambda);
		f1lambda = f1lambda*invLen*maxfriction - mCollisionPoint.Jf1;
		f2lambda = f2lambda*invLen*maxfriction - mCollisionPoint.Jf2;
	}
	
	getRenderStuff().addRedArrow(mWheelBottomPoint, mWheelBottomPoint + f1n1*f1lambda);
	getRenderStuff().addGreenArrow(mWheelBottomPoint, mWheelBottomPoint + f2n1*f2lambda);
				
	mCollisionPoint.Jf1 += f1lambda;
	mCollisionPoint.Jf2 += f2lambda;*/

	vec3 Jf = mCollisionPoint.t1*f1lambda + mCollisionPoint.t2*f2lambda;

	mVehicle->applyImpulse(mCollisionPoint.mPoint, Jf);

	float biasA = n1*mVehicle->mBiasVelocity + w1*mVehicle->mBiasAngularVelocity;

	float biasLambda = -(biasA - mCollisionPoint.mBiasImpulse)*b;
					
	mVehicle->applyBiasImpulse(mCollisionPoint.mPoint, mCollisionPoint.mNormal*biasLambda);
}

void VehicleChassis::derivedPostSolve( float dt )
{
	float lastPosition = mPosition;

	//gLog->fout(1, "ang vel = %.3f\n", mWheelAngVelocity);

	mPosition += mWheelVelocity*dt;

	if (mPosition > mMinPosition)
		mPosition = mMinPosition;

	if (mPosition < mMaxPosition)
		mPosition = mMaxPosition;

	mPosition = mMaxPosition;
	
	mWheelAngVelocity += mWheelTorque*mWheelInvInertia;
	mWheelTorque = 0;

	mWheelAngVelocity -= sign(mWheelAngVelocity)*fmin((mBrakeForce1*mBrakeCoef1 + mBrakeForce2*mBrakeCoef2)*mWheelInvInertia*dt, 
		fabs(mWheelAngVelocity));

	mWheelXAngle += mWheelAngVelocity*dt*2.0f*3.1415926f;

	mLocalAxis = mInitialLocalAxis*rotatedYMatrix(mWheelAngle);

	mGlobalAxis = mLocalAxis*mVehicle->mOrient;
	mGlobalPosition = mLocalPosition*mVehicle->mOrient + mVehicle->mPosition;
	
	vec3 xvec = mGlobalAxis.getXVector();
	vec3 yvec = mGlobalAxis.getYVector();
	vec3 zvec = mGlobalAxis.getZVector();

	vec3 lastWheelBottomPos = mWheelBottomPoint;
	mWheelBottomPoint = mGlobalPosition + mGlobalAxis.getYVector()*(mPosition - mWheelRadius);	

	checkCollision();

	if (mWheelOnGround)
	{
		float yproj = yvec*mCollisionPoint.mNormal;
		float posOffset = mCollisionPoint.mDepth/(yproj);

		float newPosition = mPosition + posOffset;
		float minDist = mMinPosition + newPosition;

		mPosition = mMinPosition + fmin(0.0f, minDist);

		mWheelVelocity = (mPosition - lastPosition)/dt;

		if (minDist > 0)
		{
			mCollisionPoint.mDepth = yproj*minDist;
		}
		else
		{
			mCollisionPoint.mDepth = -1.0f;
			mCollisionPoint.reset();
		}
	}

	mWheelBottomPoint = mGlobalPosition + mGlobalAxis.getYVector()*(mPosition - mWheelRadius);	

	vec3 wheelPos = mGlobalPosition + mGlobalAxis.getYVector()*mPosition;

	int segments = 10;
	for (int i = 0; i < segments; i++)
	{
		float angle = rad((float)i*360.0f/(float)segments) + mWheelXAngle;
		float sn = sinf(angle), cs = cosf(angle);

		vec3 segPoint = wheelPos + yvec*sn*mWheelRadius + zvec*cs*mWheelRadius;
	}
}

void VehicleChassis::checkCollision()
{
	vec3 bottomPoint = mWheelBottomPoint;
	vec3 dir = mGlobalAxis.getYVector()*-1.0f;
	vec3 topPoint = mGlobalPosition;
	float leng = (bottomPoint - topPoint).len();

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
			lPolygon* poly = polygonsBuffer[i];
			if (IntersectLinePolygon(poly->pa->mPosition, poly->pb->mPosition, poly->pc->mPosition,
				poly->norm, bottomPoint, topPoint, &point, &depth))
			{
				depth = (bottomPoint - poly->pa->mPosition)*poly->norm;
				depth = -depth;

				if (depth < minDepth)
				{
					mWheelOnGround = true;

					mCollisionPoint.mNormal = poly->norm;
					mCollisionPoint.mPoint = point;
					mCollisionPoint.mDepth = depth;
				}
			}
		}
	}

	if (!mWheelOnGround)
	{
		mCollisionPoint.reset();
	}
}

void VehicleChassis::getPosition( float* positionVec )
{
	vmask(positionVec, mGlobalPosition + mGlobalAxis.getYVector()*mPosition);
}

void VehicleChassis::getOrientation( float* orientMatrix )
{
	mmask(orientMatrix, rotatedXMatrix(mWheelXAngle)*mGlobalAxis);
}

}