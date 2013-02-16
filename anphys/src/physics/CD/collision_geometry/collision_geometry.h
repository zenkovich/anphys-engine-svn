#ifndef COLLISION_GEOMETRY_H
#define COLLISION_GEOMETRY_H

#include <vector>
#include "../util/utils.h"

struct phObject;
struct phCollisionGeometryPart;
struct phCollision;

struct phCollisionGeometry
{
	enum { nMaxStoredCollisions = 10 };
	typedef std::vector<phCollisionGeometryPart*> PartsList;
	typedef std::vector<phCollision*> CollisionsList;

	phObject* mObject;
	PartsList mParts;

	CollisionsList mStoredCollisions;

	AABB      mAABB;

	float     mPerformance;

	phCollisionGeometry();
	phCollisionGeometry(phObject* object);
	phCollisionGeometry(phObject* object, phCollisionGeometryPart* part);
	phCollisionGeometry(phCollisionGeometryPart* part);
	~phCollisionGeometry();

	phCollisionGeometryPart* addPart(phCollisionGeometryPart* newPart);
	bool removePart(phCollisionGeometryPart* part);
	void removeAllParts();

	void preUpdate(float dt);
	void update(float dt);
	void postUpdate(float dt);

	phCollision* checkCollision(phCollisionGeometry* collisionGeometry, phCollision* collision);

	void storeCollision(phCollision* collision);
	void eraseStoredCollision(phCollision* collision);
};

#endif //COLLISION_GEOMETRY_H