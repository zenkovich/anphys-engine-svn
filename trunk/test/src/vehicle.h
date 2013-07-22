#ifndef VEHICLE_H
#define VEHICLE_H

#include <vector>

#include "stuff.h"

namespace physics
{

struct VehicleChassis;

struct Vehicle
{
	typedef std::vector<CollisionGeometryVertex> PointsList;
	typedef std::vector<CollisionPoint*> CollisionPointsList;

//rigid object parametres
	mat3x3              mOrient;
	vec3                mPosition;
	vec3                mVelocity;
	vec3                mAngularVelocity;
	float               mMass;
	float               mInvMass;
	mat3x3              mInertia;
	mat3x3              mInvInertia;
	mat3x3              mWorldInertia;
	mat3x3              mInvWorldInertia;
	vec3                mForce;
	vec3                mTorque;
	vec3                mBiasVelocity;
	vec3                mBiasAngularVelocity;
	
//vehicle chassis
	float               mWheelAnglesCoef;
	VehicleChassis*     mFrontLeftChassis;
	VehicleChassis*     mFrontRightChassis;
	VehicleChassis*     mRearLeftChassis;
	VehicleChassis*     mRearRightChassis;

//collision geometry points
	PointsList          mCollisionGeometryPoints;
	CollisionPointsList mActiveCollisionPoints;
	CollisionPointsList mFreeCollisionPoints;

//engine
	float               mEngineRpm;
	float               mEngineIdleRpm;
	float               mEngineInertia;
	float               mEngineInvInertia;
	float               mEngineFriction;
	float               mEngineTorque;

	float*              mEngineTorqueValues;
	int                 mEngineTorqueValuesCount;
	float               mEngineTorqueGraphicMaxRpm;
	VehicleChassis*     mDriveChassisList[4];
	int                 mDriveChassisCount;

//gearbox
	enum WheelDriveType { WD_FULL = 0, WD_FWD, WD_REAR };
	WheelDriveType      mWheelDriveType;
	float*              mGearsCoefs;
	int                 mGearsCount;
	float               mMainGear;
	int                 mCurrentGear;
	float               mResDriveCoef;

//controls
	float               mThrottleCoef;
	float               mBrakeCoef;
	float               mHandBrakeCoef;
	float               mSteerWheelAngle;
	float               mClutchCoef;

//pointer to landscape polygons buffer
	lPolygon**          mPolygonsBuffer;
	lVertex**           mVertexBuffer;
	unsigned int        mPolygonsBufferCount;


//functions
	Vehicle();
	~Vehicle();

	void update(float dt);
	void applyImpulse(vec3& point, vec3& impulse);
	void applyBiasImpulse( vec3& point, vec3& impulse );

	void setPolygonsBuffer(lPolygon** buffer, lVertex** vertexBuffer, unsigned int count);	

	void getPosition(float* positionVec);
	void getOrientation(float* orientMatrix);

	void setEngineParams(float* graphicValues, int valuesCount, float maxRpm, float idleRpm, float inertia, float friction);
	void setGearBoxParametres(float* gears, int gearsCount, float mainGear, WheelDriveType driveType);
	
protected:
	void updateCollisionGeometry();
	void checkCollisions();
	void solveCollisions(float dt);

	void updateEngine(float dt);

	void solveEngineWheelDrive(  );

	float getEngineTorqueFromGraphic();
};

}

#endif //VEHICLE_H