#include "vehicle.h"
#include "vehicle_chassis.h"

namespace physics
{

Vehicle::Vehicle()
{	
	mDebugging = false;

	int maxCollisionPoints = 50;

	for (int i = 0; i < maxCollisionPoints; i++)
		mFreeCollisionPoints.push_back(new CollisionPoint());
	
	mFrontLeftChassis = new VehicleChassis(this);
	mFrontRightChassis = new VehicleChassis(this);
	mRearLeftChassis = new VehicleChassis(this);
	mRearRightChassis = new VehicleChassis(this);
	
	mRearRightChassis->mInvertedSide = true;
	mFrontRightChassis->mInvertedSide = true;

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
	mLastChangeGearTime = 0;
	mAeroCoef1 = mAeroCoef3 = mAeroCoef2 = 0;

	mTime = 0;
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
	mTime += dt;

	mThrottleCoef  = fclamp(mThrottleCoef, 0.0f, 1.0f);
	mBrakeCoef     = fclamp(mBrakeCoef, 0.0f, 1.0f);
	mHandBrakeCoef = fclamp(mHandBrakeCoef, 0.0f, 1.0f);
	mClutchCoef    = fclamp(mThrottleCoef, 0.0f, 1.0f);

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

	updateAerodynamics(dt);

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

	mLastChangeGearTime += dt;

	/*if (mDebugging)
	{
		if (mCurrentGear > 0 && mEngineRpm > 6100 && mLastChangeGearTime > 0.5f)
		{
			mCurrentGear += 1;
			mLastChangeGearTime = 0;
			printf("gear up %i\n", mCurrentGear - 1);
		}

		if (mCurrentGear > 2 && mEngineRpm < 3000 && mLastChangeGearTime > 0.5f)
		{
			mCurrentGear -= 1;
			mLastChangeGearTime = 0;
			printf("gear down %i\n", mCurrentGear - 1);
		}
	}*/
}

void Vehicle::updateEngine( float dt )
{
	mEngineRpm += mEngineTorque;
	mEngineTorque = 0;

	if (mEngineRpm < 200)
		mEngineRpm = 200;

	if (mThrottleCoef < 0.25f)
	{
		if (mEngineRpm < mEngineIdleRpm)
			mThrottleCoef = 0.2f;
		else 
			mThrottleCoef = 0;
	}

	float engineTorque = getEngineTorqueFromGraphic()*mThrottleCoef - mEngineFriction*mEngineRpm*(1 - mThrottleCoef);
	float radianPlus = engineTorque*dt*mEngineInvInertia;
	mEngineRpm += radianPlus/2.0f/3.1415926f*60.0f;
	//mEngineRpm += engineTorque*mEngineInvInertia*0.5f*60.0f*dt ;

	//gLog->fout(1, "Engine torque = %.3f N*m\n", engineTorque);

	mResDriveCoef = mGearsCoefs[mCurrentGear]*mMainGear;	

	if (fabs(mResDriveCoef) > 0.0001f)
	{
		float wheelsTorque = -engineTorque*mResDriveCoef/(float)mDriveChassisCount*mClutchCoef*dt;

		float fastestWheelSpeed = 0;
		for (int i = 0; i < mDriveChassisCount; i++)
		{
			float wheelRadPlus = wheelsTorque/(mDriveChassisList[i]->mWheelInertia + mEngineInertia*fabs(mResDriveCoef));

			if (fastestWheelSpeed < fabs(mDriveChassisList[i]->mWheelAngVelocity))
				fastestWheelSpeed = mDriveChassisList[i]->mWheelAngVelocity;

			mDriveChassisList[i]->mWheelAngVelocity += wheelRadPlus/2.0f/3.1415926f;
		}

		//fastestWheelSpeed = mDriveChassisList[0]->mWheelAngVelocity;

		if (mCurrentGear != 1 && mClutchCoef > 0.11f)
		{
			mEngineRpm = -fastestWheelSpeed*60.0f*mResDriveCoef;
			//gLog->fout(1, "Engine rpm = %.3f (%.3f)\n", mEngineRpm, fastestWheelSpeed);
		}

		if (mDebugging)
			printf("vel = %.3f/%.3f rpm = %.1f/%.1f gear %i t1 %.2f t2 %.2f\n", -fastestWheelSpeed*2.0f*3.1415926*mDriveChassisList[0]->mWheelRadius*3.6f, mVelocity.len()*3.6f, mEngineRpm, fastestWheelSpeed, mCurrentGear, engineTorque, wheelsTorque/dt);
	}
	else
		if (mDebugging) printf("RPM = %.3f\n", mEngineRpm);
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
	return;

	if (fabs(mResDriveCoef) > 0.0001f)
	{
		float fastestWheelSpeed = 0;
		float wheelsInertiaSumm = 0;
		for (int i = 0; i < mDriveChassisCount; i++)
		{
			if (fastestWheelSpeed < fabs(mDriveChassisList[i]->mWheelAngVelocity))
				fastestWheelSpeed = -mDriveChassisList[i]->mWheelAngVelocity;

			wheelsInertiaSumm += mDriveChassisList[i]->mWheelInvInertia;
		}

		/*fastestWheelSpeed = -mDriveChassisList[0]->mWheelAngVelocity;*/
		wheelsInertiaSumm = mDriveChassisList[0]->mWheelInvInertia;

		float engineRps = mEngineRpm/60.0f;

		/*engineRps = 1;
		fastestWheelSpeed = 2;
		mResDriveCoef = 5;
		/*wheelsInertiaSumm = 4;
		mEngineInvInertia = 3;*/

		float c = 1.0f/60.0f/mResDriveCoef;
		float c2 = 1.0f;

		float vp1 = engineRps*mResDriveCoef;
		float vp2 = fastestWheelSpeed;

		//float t = (engineRps*mResDriveCoef - fastestWheelSpeed)/(1.0f/wheelsInertiaSumm - mResDriveCoef/mEngineInvInertia);
		float a = (fastestWheelSpeed*c2 - mEngineRpm*c);
		float b = (c*c/mEngineInvInertia - c2*c2/wheelsInertiaSumm);
		float t = a/b*mClutchCoef;

		mEngineRpm += t/mEngineInvInertia*c;
		engineRps += t/mEngineInvInertia*mResDriveCoef;

		/*mDriveChassisList[0]->mWheelAngVelocity -= t/wheelsInertiaSumm;
		fastestWheelSpeed += t/wheelsInertiaSumm;*/
		for (int i = 0; i < mDriveChassisCount; i++)
		{
			mDriveChassisList[i]->mWheelAngVelocity -= t/wheelsInertiaSumm*c2;

			if (fastestWheelSpeed < fabs(mDriveChassisList[i]->mWheelAngVelocity))
				fastestWheelSpeed = -mDriveChassisList[i]->mWheelAngVelocity;
		}
		
		//engineRps = mEngineRpm/60.0f;
		float vp11 = engineRps*mResDriveCoef;
		float vp21 = fastestWheelSpeed;

		printf("eng %.3f wheel %.3f, %.3f %.3f\n", mEngineRpm*c, mDriveChassisList[0]->mWheelAngVelocity, vp11, vp21);

		//fastestWheelSpeed = mDriveChassisList[0]->mWheelAngVelocity;

		/*if (mCurrentGear != 1 && mClutchCoef > 0.11f)
		{
			mEngineRpm = -fastestWheelSpeed*60.0f*mResDriveCoef;
			//gLog->fout(1, "Engine rpm = %.3f (%.3f)\n", mEngineRpm, fastestWheelSpeed);
		}*/
	}
}

void Vehicle::setPolygonsBuffer( lPolygon** buffer, lVertex** vertexBuffer, unsigned int count )
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

		if (mDebugging)
			pushDbgPoint(vertex->mGlobalPos, 1, 0, 1, 1);

		CollisionPoint* cpoint = vertex->mCollisionPoint;
		bool fromFreeList = false;

		if (!cpoint)
		{
			cpoint = mFreeCollisionPoints[0];
			cpoint->reset();
			fromFreeList = true;
		}

		bool isColliding = false;
		float minDepth = 9999;

		vec3 np = vertex->mGlobalPos, na = vertex->mGlobalPos, nb = vertex->mGlobalPos, nc = vertex->mGlobalPos;

		for (int i = 0; i < (int)mPolygonsBufferCount; i++)
		{
			lPolygon* polygon = mPolygonsBuffer[i];

			vec3 pt, norm;
			float depth;

			if (polygon->isIntersect(vertex->mGlobalPos, mVertexBuffer, &pt, &norm, &depth))
			{
				if (fabs(minDepth) > fabs(depth))
				{
					minDepth = depth;
					if (depth < 0 || depth > 1.3f)
					{
						isColliding = false;
					}
					else
					{
						isColliding = true;
						cpoint->mPoint = pt;
						cpoint->mNormal = norm;
						cpoint->mDepth = depth;
					}
					np = pt;
					na = vmask((float*)&mVertexBuffer[polygon->a]->mPosition);
					nb = vmask((float*)&mVertexBuffer[polygon->b]->mPosition);
					nc = vmask((float*)&mVertexBuffer[polygon->c]->mPosition);
				}
			}
		}
		
		if (mDebugging)
		{
			if (isColliding)
			{
				pushDbgLine(vertex->mGlobalPos, np, 1, 0, 0, 1);
			
				pushDbgLine(na, nb, 1, 0, 0, 1);
				pushDbgLine(nb, nc, 1, 0, 0, 1);
				pushDbgLine(nc, na, 1, 0, 0, 1);
			}
			else
			{
				pushDbgLine(vertex->mGlobalPos, np, 0, 0, 1, 1);
			
				pushDbgLine(na, nb, 0, 1, 0, 1);
				pushDbgLine(nb, nc, 0, 1, 0, 1);
				pushDbgLine(nc, na, 0, 1, 0, 1);
			}

			//isColliding = false;

			pushDbgLine(na, nb, 0, 1, 0, 1);
			pushDbgLine(nb, nc, 0, 1, 0, 1);
			pushDbgLine(nc, na, 0, 1, 0, 1);
			pushDbgLine(np, na, 0.9f, 1, 0.9f, 0.2f);
			pushDbgLine(np, nb, 0.9f, 1, 0.9f, 0.2f);
			pushDbgLine(np, nc, 0.9f, 1, 0.9f, 0.2f);
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
	float E = 0.0f, Mu = 0.3f, biasERP = 0.8f;
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

void Vehicle::pushDbgLine( const vec3& p1, const vec3& p2, float cr, float cg, float cb, float ca )
{
	mDbgLines.push_back(DbgLine(p1, p2, cr, cg, cb, ca));
}

void Vehicle::pushDbgPoint( const vec3& p1, float cr, float cg, float cb, float ca )
{
	vec3 ofsx(0.1f, 0, 0);
	vec3 ofsy(0, 0.1f, 0);
	vec3 ofsz(0, 0, 0.1f);

	pushDbgLine(p1 - ofsx, p1 + ofsx, cr, cg, cb, ca);
	pushDbgLine(p1 - ofsy, p1 + ofsy, cr, cg, cb, ca);
	pushDbgLine(p1 - ofsz, p1 + ofsz, cr, cg, cb, ca);
}

float Vehicle::getLandscapeFrtCoef( unsigned char id )
{
	return mLandscapeFrtCoefs[id];
}

void Vehicle::setupLanscapeFrtCoefs( float* coefs, int count /*= 256*/ )
{
	for (int i = 0; i < count; i++)
		mLandscapeFrtCoefs[i] = coefs[i];
}

float Vehicle::getLandscapeDepthCoef( unsigned char id )
{
	return mLandscapeDepthCoefs[id];
}

void Vehicle::setupLanscapeDepthCoefs( float* coefs, int count /*= 256*/ )
{
	for (int i = 0; i < count; i++)
		mLandscapeDepthCoefs[i] = coefs[i];
}

void Vehicle::setupAeroCoefs( float aeroFriction, const vec3& aeroPoint1, float aeroCoef1, const vec3& aeroPoint2, float aeroCoef2, const vec3& aeroPoint3, float aeroCoef3 )
{
	mAeroFriction = aeroFriction;
	mAeroCoef1 = aeroCoef1;
	mAeroCoef2 = aeroCoef2;
	mAeroCoef3 = aeroCoef3;
	mAeroPoint1 = aeroPoint1;
	mAeroPoint2 = aeroPoint2;
	mAeroPoint3 = aeroPoint3;
}

void Vehicle::updateAerodynamics( float dt )
{
	//return;

	vec3 zvec = mOrient.getZVector();
	vec3 yvec = mOrient.getYVector();
	vec3 xvec = mOrient.getXVector();
	
	float zvproj = mVelocity*zvec;
	float xvproj = mVelocity*xvec;

	/*printf("%.2 %.2f %.2f %.2f %.2f\n", xvproj*xvproj*mAeroFriction*2.0f*dt, zvproj*zvproj*mAeroFriction*dt,
		zvproj*zvproj*mAeroCoef1*dt, zvproj*zvproj*mAeroCoef2*dt, zvproj*zvproj*mAeroCoef3*dt);*/

	vec3 frImp = xvec*(xvproj*xvproj*mAeroFriction*2.0f*dt) + zvec*(zvproj*zvproj*mAeroFriction*dt);
	mVelocity -= frImp;
	
	vec3 p1 = mPosition + mAeroPoint1*mOrient;
	vec3 p2 = mPosition + mAeroPoint2*mOrient;
	vec3 p3 = mPosition + mAeroPoint3*mOrient;
	
	applyImpulse(p1, yvec*(-zvproj*zvproj*mAeroCoef1*dt));
	applyImpulse(p2, yvec*(-zvproj*zvproj*mAeroCoef2*dt));
	applyImpulse(p3, yvec*(-zvproj*zvproj*mAeroCoef3*dt));
	
	pushDbgLine(mPosition, mPosition - frImp/dt, 1, 0, 0, 1);
	pushDbgLine(p1, p1 + yvec*(-zvproj*zvproj*mAeroCoef1*dt), 0, 1, 0, 1);
	pushDbgLine(p2, p2 + yvec*(-zvproj*zvproj*mAeroCoef2*dt), 0, 0, 1, 1);
	pushDbgLine(p3, p3 + yvec*(-zvproj*zvproj*mAeroCoef3*dt), 1, 1, 0, 1);
	pushDbgPoint(p1, 0, 1, 0, 1);
	pushDbgPoint(p2, 0, 0, 1, 1);
	pushDbgPoint(p3, 1, 1, 0, 1);
}

}