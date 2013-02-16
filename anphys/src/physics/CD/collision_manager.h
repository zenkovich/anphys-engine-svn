#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <vector>
#include "../util/other/precreated_vector.h"

struct phCollision;
struct phScene;
struct phObject;

struct phCollisionManager
{
	enum { nReservedCollisions = 1024 };
	typedef cArray<phCollision> CollisionsList;

	phScene*        mScene;
	CollisionsList* mCollisions;
	unsigned int    mIndex;


	phCollisionManager();
	phCollisionManager(phScene* scene);
	~phCollisionManager();

	void checkCollisions();

	phCollision* isCollide(phObject* objA, phObject* objB);
};

#endif //COLLISION_MANAGER_H