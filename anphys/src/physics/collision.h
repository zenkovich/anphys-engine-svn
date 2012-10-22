#ifndef COLLISION_H
#define COLLISION_H

#include "../util/utils.h"

#include <vector>

#define COLLISION_DATA_SIZE 1024
#define COLLISION_RESERVED_POINTS 10

struct phObject;
struct phCollisionPoint;

struct phCollision
{
	typedef std::vector<phCollisionPoint*> PointsList;

	PointsList mPoints;
	float* mCollisionData;

	phObject* mObjectA;
	phObject* mObjectB;

	phCollision();
	phCollision(phObject* objectA, phObject* objectB);
	~phCollision();

	void setObjects(phObject* objectA, phObject* objectB);

	phCollisionPoint* addPoint();

	void clear();
};


#endif //COLLISION_H