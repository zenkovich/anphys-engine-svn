#ifndef COLLISION_POINT_H
#define COLLISION_POINT_H

#include "../util/utils.h"

struct phCollision;
struct phCollisionGeometryPart;

struct phCollisionPoint
{
	phCollision* mCollision;

	vec3 mPoint;
	vec3 mNormal;

	vec3 mImpulse;
	vec3 mBiasImpulse;

	phCollisionGeometryPart* mPartObjectA;
	phCollisionGeometryPart* mPartObjectB;

	bool mUses;

	phCollisionPoint(): mCollision(NULL), mUses(false) {}
	phCollisionPoint(phCollision* collision, phCollisionGeometryPart* partObjectA, phCollisionGeometryPart* partObjectB): 
		mCollision(collision), mUses(false), mPartObjectA(partObjectA), mPartObjectB(partObjectB) {}
	phCollisionPoint(phCollision* collision, phCollisionGeometryPart* partObjectA, phCollisionGeometryPart* partObjectB, vec3 point, vec3 normal):
		mCollision(collision), mPoint(point), mNormal(normal), mUses(true), mPartObjectA(partObjectA), mPartObjectB(partObjectB) {}
};

#endif //COLLISION_POINT_H