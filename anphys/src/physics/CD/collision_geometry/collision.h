#ifndef COLLISION_H
#define COLLISION_H

#include <vector>
#include "util/utils.h"

struct phObject;
struct phCollisionPoint;

struct phCollision
{
//const values
	enum { nCollisionDataSize = 1024, nCollisionReservedPoints = 10 };

//types
	typedef cArray<phCollisionPoint> CollisionPointsList;

//parametres
	CollisionPointsList* mPoints;
	char*                mCollisionData;

	phObject*            mObjectA;
	phObject*            mObjectB;

	unsigned int         mIndex;
	unsigned int         mTempIndex;

//functions
	phCollision();
	phCollision(phObject* objectA, phObject* objectB);
	~phCollision();

	void setObjects(phObject* objectA, phObject* objectB);

	phCollisionPoint* addPoint();

	void clear();

	void freeUnusedPoints();
};


#endif //COLLISION_H