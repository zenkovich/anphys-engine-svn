#ifndef VEHICLE_H
#define VEHICLE_H

#include <vector>

#include "stuff.h"

namespace physics
{

struct VehicleChassis;

struct Vehicle
{
	typedef std::vector<phCollisionGeometryVertex> PointsList;
	typedef std::vector<phCollisionPoint*> CollisionPointsList;

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
	
	VehicleChassis*     mFrontLeftChassis;
	VehicleChassis*     mFrontRightChassis;
	VehicleChassis*     mRearLeftChassis;
	VehicleChassis*     mRearRightChassis;

	PointsList          mCollisionGeometryPoints;
	CollisionPointsList mActiveCollisionPoints;
	CollisionPointsList mFreeCollisionPoints;

	lPolygon**          mPolygonsBuffer;
	unsigned int        mPolygonsBufferCount;

//functions
	Vehicle();
	~Vehicle();

	void update(float dt);
	void applyImpulse(vec3& point, vec3& impulse);
	void applyBiasImpulse( vec3& point, vec3& impulse );

	void setPolygonsBuffer(lPolygon** buffer, unsigned int count);

protected:
	void updateCollisionGeometry();
	void checkCollisions();
	void solveCollisions(float dt);
};

}

#endif //VEHICLE_H