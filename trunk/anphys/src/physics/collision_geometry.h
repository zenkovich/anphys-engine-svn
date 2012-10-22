#ifndef COLLISION_GEOMETRY_H
#define COLLISION_GEOMETRY_H

#include <vector>
#include "../util/utils.h"

#define MAX_STORED_COLLISIONS 10

struct phObject;
struct phCollisionGeometryPart;
struct phCollision;

struct phCollisionGeometry
{
	typedef std::vector<phCollisionGeometryPart*> PartsList;

	phObject* mObject;
	PartsList mParts;

	phCollision* mOldCollisions[MAX_STORED_COLLISIONS];

	AABB mAABB;

	float mPerformance;

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
};

#endif //COLLISION_GEOMETRY_H