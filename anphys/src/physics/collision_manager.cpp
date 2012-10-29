#include "collision_manager.h"

#include "collision.h"

phCollisionManager::phCollisionManager():mScene(NULL)
{
	for (unsigned int i = 0; i < nReservedCollisions; i++)
		mCollisions.push_back(new phCollision);
}

phCollisionManager::phCollisionManager( phScene* scene ):mScene(scene)
{	
	for (unsigned int i = 0; i < nReservedCollisions; i++)
		mCollisions.push_back(new phCollision);
}

phCollisionManager::~phCollisionManager()
{	
	for (CollisionsList::iterator it = mCollisions.begin(); it != mCollisions.end(); it++)
		safe_release(*it);

	mCollisions.clear();
}

void phCollisionManager::checkCollisions()
{

}

phCollision* phCollisionManager::isCollide( phObject* objA, phObject* objB )
{
	return NULL;
}
