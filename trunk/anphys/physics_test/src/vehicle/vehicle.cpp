#include "vehicle.h"
#include "vehicle_chassis.h"

#include "util/log/log_system.h"

namespace physics
{

Vehicle::Vehicle()
{	
	int maxCollisionPoints = 50;

	for (int i = 0; i < maxCollisionPoints; i++)
		mFreeCollisionPoints.push_back(new CollisionPoint());
	
	mFrontLeftChassis = new VehicleChassis(this);
	mFrontRightChassis = new VehicleChassis(this);
	mRearLeftChassis = new VehicleChassis(this);
	mRearRightChassis = new VehicleChassis(this);

	mPolygonsBuffer = NULL;

	mWheelAnglesCoef = 1;

	mEngineRpm = 0;
	mEngineInertia = 1; 
	mEngineInvInertia = 1;
	mEngineFriction = 1;
	mEngineTorqueValues = NULL;
	mEngineTorqueValuesCount = 0;
	mEngineTorqueGraphicMaxRpm = 10;
	mWheelDriveType = WD_FULL;
	mGearsCoefs = NULL;
	mGearsCount = 0;
	mMainGear = 1;
	mCurrentGear = 1;
	mThrottleCoef = 0;
	mBrakeCoef = 0;
	mHandBrakeCoef = 0;
	mSteerWheelAngle = 0;
	mClutchCoef = 0;
	mEngineTorque = 0;
}

Vehicle::~Vehicle()
{
	for (CollisionPointsList::iterator it = mFreeCollisionPoints.begin(); it != mFreeCollisionPoints.end(); it++)
	{
		delete *it;
	}
	
	delete mFrontLeftChassis;
	delete mFrontRightChassis;
	delete mRearLeftChassis;
	delete mRearRightChassis;

	if (mEngineTorqueValues)
		delete[] mEngineTorqueValues;

	if (mGearsCoefs)
		delete[] mGearsCoefs;
}

void Vehicle::update( float dt )
{	
	mThrottleCoef  = fclamp(mThrottleCoef, 0.0f, 1.0f);
	mBrakeCoef     = fclamp(mBrakeCoef, 0.0f, 1.0f);
	mHandBrakeCoef = fclamp(mHandBrakeCoef, 0.0f, 1.0f);
	mClutchCoef    = fclamp(mClutchCoef, 0.0f, 1.0f);

	if (mCurrentGear < 0)
		mCurrentGear = 0;

	if (mCurrentGear > mGearsCount - 1)
		mCurrentGear = mGearsCount - 1;

	mVelocity += mForce + vec3(0, -9.8f*dt, 0);
	mAngularVelocity += mTorque;

	mForce = mTorque = vec3(0);
	
	mFrontLeftChassis->mBrakeCoef1  = mBrakeCoef;
	mFrontRightChassis->mBrakeCoef1 = mBrakeCoef;
	mRearLeftChassis->mBrakeCoef1   = mBrakeCoef;
	mRearRightChassis->mBrakeCoef1  = mBrakeCoef;
	
	mFrontLeftChassis->mBrakeCoef2  = mHandBrakeCoef;
	mFrontRightChassis->mBrakeCoef2 = mHandBrakeCoef;
	mRearLeftChassis->mBrakeCoef2   = mHandBrakeCoef;
	mRearRightChassis->mBrakeCoef2  = mHandBrakeCoef;
	
	mFrontLeftChassis->mWheelAngle  = mSteerWheelAngle*mWheelAnglesCoef;
	mFrontRightChassis->mWheelAngle = mSteerWheelAngle*mWheelAnglesCoef;
	
	mFrontLeftChassis->derivedPreSolve(dt);
	mFrontRightChassis->derivedPreSolve(dt);
	mRearLeftChassis->derivedPreSolve(dt);
	mRearRightChassis->derivedPreSolve(dt);

	solveCollisions(dt);
	updateEngine(dt);

	mFrontLeftChassis->derivedSolve(dt);
	mFrontRightChassis->derivedSolve(dt);
	mRearLeftChassis->derivedSolve(dt);
	mRearRightChassis->derivedSolve(dt);
	solveEngineWheelDrive();

	mPosition += (mVelocity + mBiasVelocity)*dt;

	vec3 rotateVec = (mAngularVelocity + mBiasAngularVelocity)*dt;
	float rotateVecLen = rotateVec.len();
	if (rotateVecLen > 0.0000001f)
		mOrient = rotateMatrixAroundVec(mOrient, rotateVec/rotateVecLen, rotateVecLen);

	mBiasVelocity = mBiasAngularVelocity = vec3(0);

	mInvWorldInertia = (mOrient)*mInvInertia*(mOrient.transpose());
	mInvWorldInertia = mInvInertia;
	
	mFrontLeftChassis->derivedPostSolve(dt);
	mFrontRightChassis->derivedPostSolve(dt);
	mRearLeftChassis->derivedPostSolve(dt);
	mRearRightChassis->derivedPostSolve(dt);

	updateCollisionGeometry();
	checkCollisions();
}

void Vehicle::updateEngine( float dt )
{
	mEngineRpm += mEngineTorque;
	mEngineTorque = 0;

	if (mEngineRpm < 200)
		mEngineRpm = 200;

	if (mEngineRpm < mEngineIdleRpm && mThrottleCoef < 0.2f)
		mThrottleCoef = 0.2f;

	float engineTorque = getEngineTorqueFromGraphic()*mThrottleCoef - mEngineFriction*mEngineRpm*(1 - mThrottleCoef);
	mEngineRpm += engineTorque*mEngineInvInertia*0.5f*60.0f*dt ;

	//gLog->fout(1, "Engine torque = %.3f N*m\n", engineTorque);

	mResDriveCoef = mGearsCoefs[mCurrentGear]*mMainGear;	

	float wheelsTorque = -engineTorque*0.5f*mResDriveCoef/(float)mDriveChassisCount*mClutchCoef*0.001f;

	float fastestWheelSpeed = 0;
	for (int i = 0; i < mDriveChassisCount; i++)
	{
		mDriveChassisList[i]->mWheelAngVelocity += wheelsTorque*mDriveChassisList[i]->mWheelInvInertia;

		if (fastestWheelSpeed < fabs(mDriveChassisList[i]->mWheelAngVelocity))
			fastestWheelSpeed = mDriveChassisList[i]->mWheelAngVelocity;
	}

	fastestWheelSpeed = mDriveChassisList[0]->mWheelAngVelocity;

	if (mCurrentGear != 1 && mClutchCoef > 0.11f)
	{
		mEngineRpm = -fastestWheelSpeed*60.0f*mResDriveCoef;
		//gLog->fout(1, "Engine rpm = %.3f (%.3f)\n", mEngineRpm, fastestWheelSpeed);
	}
}

float Vehicle::getEngineTorqueFromGraphic()
{
	float range = mEngineTorqueGraphicMaxRpm;
	int idx = (int)((mEngineRpm)/range*(float)mEngineTorqueValuesCount);
	idx = imax(imin(idx, mEngineTorqueValuesCount - 2), 0);

	float segmentLength = range/(float)mEngineTorqueValuesCount;

	float scoef = (mEngineRpm - (float)idx*segmentLength)/segmentLength;
	
	float value1 = mEngineTorqueValues[idx];
	float value2 = mEngineTorqueValues[idx + 1];

	return value1 + (value2 - value1)*scoef;
}

void Vehicle::solveEngineWheelDrive(  )
{		
}

void Vehicle::setPolygonsBuffer( lPolygon** buffer, lVertex* vertexBuffer, unsigned int count )
{
	mPolygonsBuffer = buffer;
	mVertexBuffer = vertexBuffer;
	mPolygonsBufferCount = count;
}

void Vehicle::updateCollisionGeometry()
{
	for (PointsList::iterator it = mCollisionGeometryPoints.begin(); it != mCollisionGeometryPoints.end(); ++it)
	{
		CollisionGeometryVertex* vertex = &(*it);
		vertex->mGlobalPos = vertex->mLocalPos*mOrient + mPosition;
	}
}

void Vehicle::checkCollisions()
{
	mActiveCollisionPoints.clear();

	for (PointsList::iterator it = mCollisionGeometryPoints.begin(); it != mCollisionGeometryPoints.end(); ++it)
	{
		CollisionGeometryVertex* vertex = &(*it);

		CollisionPoint* cpoint = vertex->mCollisionPoint;
		bool fromFreeList = false;

		if (!cpoint)
		{
			cpoint = mFreeCollisionPoints[0];
			cpoint->reset();
			fromFreeList = true;
		}

		bool isColliding = false;
		float minDepth = 999.0f;

		for (int i = 0; i < (int)mPolygonsBufferCount; i++)
		{
			lPolygon* polygon = mPolygonsBuffer[i];

			if (polygon->isIntersect(vertex->mGlobalPos, mVertexBuffer, &cpoint->mPoint, &cpoint->mNormal, &cpoint->mDepth))
			{
				if (minDepth > cpoint->mDepth)
				{
					minDepth = cpoint->mDepth;
					isColliding = true;
				}
			}
		}

		if (isColliding)
		{
			mActiveCollisionPoints.push_back(cpoint);
			vertex->mCollisionPoint = cpoint;
		}
		else
		{
			vertex->mCollisionPoint = NULL;
		}

		if (fromFreeList && isColliding)
		{
			mFreeCollisionPoints.erase(mFreeCollisionPoints.begin());
		}
		if (!fromFreeList && !isColliding)
		{
			mFreeCollisionPoints.push_back(cpoint);
		}
	}
}

void Vehicle::solveCollisions( float dt )
{
	float E = 0.3f, Mu = 0.5f, biasERP = 0.8f;
	int iterations = 10;
	float invDt = 1.0f/dt;

	for (CollisionPointsList::iterator jt = mActiveCollisionPoints.begin(); jt != mActiveCollisionPoints.end(); ++jt)
	{
		CollisionPoint* collisionPoint = *jt;

		float contactDepth = collisionPoint->mDepth - 0.05f;
		if (contactDepth > 0.0f)
		{
			collisionPoint->mBiasImpulse = contactDepth*invDt*biasERP;
		}
		else collisionPoint->mBiasImpulse = 0.0f;

		collisionPoint->t1 = collisionPoint->mNormal^vec3(0, 0, 1);
		if (collisionPoint->t1*collisionPoint->t1 < 0.0001f)
			collisionPoint->t1 = collisionPoint->mNormal^vec3(0, 1, 0);

		collisionPoint->t2 = collisionPoint->mNormal^collisionPoint->t1;
			
		vec3 imp = collisionPoint->mNormal*collisionPoint->J + collisionPoint->t1*collisionPoint->Jf1 + 
				                                                collisionPoint->t2*collisionPoint->Jf2;
			
		applyImpulse(collisionPoint->mPoint, imp);

		vec3 ra = collisionPoint->mPoint - mPosition;
			
		vec3 n1 = collisionPoint->mNormal;            collisionPoint->n1 = n1;
		vec3 w1 = collisionPoint->mNormal^ra;         collisionPoint->w1 = w1;

		float b = n1*n1*mInvMass + w1*(w1*mInvWorldInertia);
		collisionPoint->B = 1.0f/b;

				
		vec3 f1n1 = collisionPoint->t1;            collisionPoint->f1n1 = f1n1;
		vec3 f1w1 = collisionPoint->t1^ra;         collisionPoint->f1w1 = f1w1;
				
		float f1b = f1n1*f1n1*mInvMass + f1w1*(f1w1*mInvWorldInertia);
		collisionPoint->Bf1 = 1.0f/f1b;
				
		vec3 f2n1 = collisionPoint->t2;            collisionPoint->f2n1 = f2n1;
		vec3 f2w1 = collisionPoint->t2^ra;         collisionPoint->f2w1 = f2w1;

		float f2b = f2n1*f2n1*mInvMass + f2w1*(f2w1*mInvWorldInertia);
		collisionPoint->Bf2 = 1.0f/f2b;
	}

	for (int i = 0; i < iterations; i++)
	{
		for (CollisionPointsList::iterator jt = mActiveCollisionPoints.begin(); jt != mActiveCollisionPoints.end(); ++jt)
		{
			CollisionPoint* collisionPoint = *jt;
				
			vec3 ra = collisionPoint->mPoint - mPosition;
			
			vec3 n1 = collisionPoint->n1;
			vec3 w1 = collisionPoint->w1;

			float a = n1*mVelocity + w1*mAngularVelocity;
			float b =collisionPoint->B;

			float lambda = -a*(E + 1.0f)*b;

			collisionPoint->J += lambda;
			if(collisionPoint->J < 0.0f)
			{
				lambda += (0.0f - collisionPoint->J);
				collisionPoint->J = 0.0f;
			}

			applyImpulse(collisionPoint->mPoint, collisionPoint->mNormal*lambda);
				
			vec3 f1n1 = collisionPoint->f1n1;
			vec3 f1w1 = collisionPoint->f1w1;
				
			float f1a = f1n1*mVelocity + f1w1*mAngularVelocity;
			float f1b = collisionPoint->Bf1;

			float f1lambda = -f1a*f1b;
				
			vec3 f2n1 = collisionPoint->f2n1;
			vec3 f2w1 = collisionPoint->f2w1;
				
			float f2a = f2n1*mVelocity + f2w1*mAngularVelocity;
			float f2b = collisionPoint->Bf2;

			float f2lambda = -f2a*f2b;
				
			float f1accumulated = f1lambda + collisionPoint->Jf1;
			float f2accumulated = f2lambda + collisionPoint->Jf2;
			float maxfriction = collisionPoint->J*Mu;

			if (f1accumulated*f1accumulated + f2accumulated*f2accumulated > maxfriction*maxfriction)
			{
				float invLen = 1.0f/sqrtf(f1lambda*f1lambda + f2lambda*f2lambda);
				f1lambda = f1lambda*invLen*maxfriction - collisionPoint->Jf1;
				f2lambda = f2lambda*invLen*maxfriction - collisionPoint->Jf2;
			}
				
			collisionPoint->Jf1 += f1lambda;
			collisionPoint->Jf2 += f2lambda;

			vec3 Jf = collisionPoint->t1*f1lambda + collisionPoint->t2*f2lambda;

			applyImpulse(collisionPoint->mPoint, Jf);

			
			float biasA = n1*mBiasVelocity + w1*mBiasAngularVelocity;

			float biasLambda = -(biasA - collisionPoint->mBiasImpulse)*b;

			if (biasLambda > 0.0f)					
				applyBiasImpulse(collisionPoint->mPoint, collisionPoint->mNormal*biasLambda);
		}
	}
}

void Vehicle::applyImpulse(vec3& point, vec3& impulse)
{
	mVelocity += impulse*mInvMass;
	mAngularVelocity += (impulse^(point - mPosition))*mInvWorldInertia;
}

void Vehicle::applyBiasImpulse( vec3& point, vec3& impulse )
{
	mBiasVelocity += impulse*mInvMass;
	mBiasAngularVelocity += (impulse^(point - mPosition))*mInvWorldInertia;
}

void Vehicle::getPosition( float* positionVec )
{
	vmask(positionVec, mPosition);
}

void Vehicle::getOrientation( float* orientMatrix )
{
	mmask(orientMatrix, mOrient);
}

void Vehicle::setEngineParams( float* graphicValues, int valuesCount, float maxRpm, float idleRpm, float inertia, float friction )
{
	mEngineTorqueValuesCount = valuesCount;

	mEngineTorqueValues = new float[valuesCount];
	for (int i = 0; i < mEngineTorqueValuesCount; i++)
		mEngineTorqueValues[i] = graphicValues[i];

	mEngineTorqueGraphicMaxRpm = maxRpm;
	mEngineInertia = inertia;
	mEngineInvInertia = 1.0f/inertia;
	mEngineIdleRpm = idleRpm;
	mEngineFriction = friction;
}

void Vehicle::setGearBoxParametres( float* gears, int gearsCount, float mainGear, WheelDriveType driveType )
{
	mGearsCount = gearsCount;

	mGearsCoefs = new float[gearsCount];
	for (int i = 0; i < gearsCount; i++)
		mGearsCoefs[i] = gears[i];

	mMainGear = mainGear;
	mWheelDriveType = driveType;	

	if (mWheelDriveType == WD_FULL)
	{
		mDriveChassisList[0] = mFrontLeftChassis;
		mDriveChassisList[1] = mFrontRightChassis;
		mDriveChassisList[2] = mRearLeftChassis;
		mDriveChassisList[3] = mRearRightChassis;
		mDriveChassisCount = 4;
	}
	else if (mWheelDriveType == WD_FWD)
	{
		mDriveChassisList[0] = mFrontLeftChassis;
		mDriveChassisList[1] = mFrontRightChassis;
		mDriveChassisCount = 2;
	}
	else 
	{
		mDriveChassisList[0] = mRearLeftChassis;
		mDriveChassisList[1] = mRearRightChassis;
		mDriveChassisCount = 2;
	}
}

}