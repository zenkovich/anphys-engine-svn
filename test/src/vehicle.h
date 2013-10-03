#ifndef VEHICLE_H
#define VEHICLE_H

#include <vector>

#include "stuff.h"

namespace physics
{

struct VehicleChassis;

struct Vehicle
{
	struct DbgLine
	{
		vec3 p1, p2;
		float cr, cg, cb, ca;

		DbgLine(const vec3& cp1, const vec3& cp2, float r, float g, float b, float a):
			p1(cp1), p2(cp2), cr(r), cg(g), cb(b), ca(a) {}
	};

	typedef std::vector<CollisionGeometryVertex> PointsList;
	typedef std::vector<CollisionPoint*> CollisionPointsList;
	typedef std::vector<DbgLine> DbgLinesList;

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
	
	vec3                mAeroPoint1;
	vec3                mAeroPoint2;
	vec3                mAeroPoint3;
	float               mAeroFriction;
	float               mAeroCoef1;
	float               mAeroCoef2;
	float               mAeroCoef3;

//pointer to landscape polygons buffer
	lPolygon**          mPolygonsBuffer;
	lVertex**           mVertexBuffer;
	unsigned int        mPolygonsBufferCount;

	DbgLinesList        mDbgLines;
	float mTime;

	float               mLastChangeGearTime;
	
	float               mLandscapeFrtCoefs[256];
	float               mLandscapeDepthCoefs[256];

	bool                mDebugging;

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
	
	void pushDbgLine(const vec3& p1, const vec3& p2, float cr, float cg, float cb, float ca);
	void pushDbgPoint(const vec3& p1, float cr, float cg, float cb, float ca);
	
	float getLandscapeFrtCoef(unsigned char id);
	void setupLanscapeFrtCoefs(float* coefs, int count = 256);

	float getLandscapeDepthCoef(unsigned char id);
	void setupLanscapeDepthCoefs(float* coefs, int count = 256);

	void setupAeroCoefs(float aeroFriction, const vec3& aeroPoint1, float aeroCoef1,
		                                    const vec3& aeroPoint2, float aeroCoef2,
		                                    const vec3& aeroPoint3, float aeroCoef3);
	
protected:
	void updateCollisionGeometry();
	void checkCollisions();
	void solveCollisions(float dt);

	void updateEngine(float dt);
	void updateAerodynamics(float dt);

	void solveEngineWheelDrive(  );

	float getEngineTorqueFromGraphic();
};

}

#endif //VEHICLE_H