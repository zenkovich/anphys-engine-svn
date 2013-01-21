#ifndef COLLISION_POINT_H
#define COLLISION_POINT_H

#include "../util/utils.h"

struct phCollision;
struct phCollisionGeometryPart;

struct phCollisionPoint
{
//parametres
	phCollision* mCollision;

	vec3 mPoint;
	vec3 mNormal;
	float mDepth;

	vec3 mImpulse;
	float mBiasImpulse;

	float Kn, Kf1, Kf2, J, Jf1, Jf2, B;

	vec3 t1, t2;

	phCollisionGeometryPart* mPartObjectA;
	phCollisionGeometryPart* mPartObjectB;
	
	unsigned int mIndex;

	bool mSolved;

//functions
	phCollisionPoint(): mCollision(NULL), mIndex(0), Kn(0), Kf1(0), Kf2(0), J(0), Jf1(0), Jf2(0), B(0) {}

	phCollisionPoint(phCollision* collision, phCollisionGeometryPart* partObjectA, 
		             phCollisionGeometryPart* partObjectB):mCollision(collision), mPartObjectA(partObjectA), 
		mPartObjectB(partObjectB), mIndex(0), Kn(0), Kf1(0), Kf2(0), J(0), Jf1(0), Jf2(0), B(0) {}

	phCollisionPoint(phCollision* collision, phCollisionGeometryPart* partObjectA, phCollisionGeometryPart* 
		             partObjectB, vec3 point, vec3 normal):mCollision(collision), mPoint(point), 
		mNormal(normal), mPartObjectA(partObjectA), mPartObjectB(partObjectB), mIndex(0), Kn(0), Kf1(0), 
		Kf2(0), J(0), Jf1(0), Jf2(0), B(0) {}

	void reset() { Kn = Kf1 = Kf2 = J = Jf1 = Jf2 = B = 0; t1 = t2 = vec3(0); }
};

#endif //COLLISION_POINT_H