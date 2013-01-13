#ifndef COLLISION_GEOMETRY_PART_H
#define COLLISION_GEOMETRY_PART_H

#include "../util/utils.h"

#define getCDDebugLevel() 2

struct phCollisionGeometry;
struct phCollision;
struct phCollisionPoint;

struct phCollisionGeometryPart
{
	mat3x3 mRotate;
	vec3   mOffset;

	vec3   mWorldPosition;
	mat3x3 mWorldOrient;

	phCollisionGeometry* mCollisionGeometry;
	AABB   mAABB;

	phCollisionGeometryPart(): mCollisionGeometry(NULL) {}
	phCollisionGeometryPart(phCollisionGeometry* collisionGeometry): mCollisionGeometry(collisionGeometry) {}
	phCollisionGeometryPart(phCollisionGeometry* collisionGeometry, mat3x3 rotate, vec3 offset):
		mCollisionGeometry(collisionGeometry), mRotate(rotate), mOffset(offset) {}
	phCollisionGeometryPart(mat3x3 rotate, vec3 offset):
		mCollisionGeometry(NULL), mRotate(rotate), mOffset(offset) {}

	virtual void preUpdate(float dt) {}
	virtual void update(float dt) {}
	virtual void postUpdate(float dt) {}
	
	virtual phCollision* checkCollision(phCollisionGeometryPart* collisionGeometryPart, phCollision* collision) { return NULL; }
};

#endif //COLLISION_GEOMETRY_PART_H