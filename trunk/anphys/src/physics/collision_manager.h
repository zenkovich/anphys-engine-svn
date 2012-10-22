#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <vector>

#define RESERVED_COLLISIONS 1000

struct phCollision;
struct phScene;
struct phObject;

struct phCollisionManager
{
	typedef std::vector<phCollision*> CollisionsList;

	phScene* mScene;

	CollisionsList mCollisions;

	phCollisionManager();
	phCollisionManager(phScene* scene);
	~phCollisionManager();

	void checkCollisions();

	phCollision* isCollide(phObject* objA, phObject* objB);
};

#endif //COLLISION_MANAGER_H