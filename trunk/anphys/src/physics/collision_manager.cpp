#include "collision_manager.h"

#include "collision.h"

phCollisionManager::phCollisionManager():mScene(NULL)
{
	mCollisions = new cArray<phCollision*>(nReservedCollisions);
}

phCollisionManager::phCollisionManager( phScene* scene ):mScene(scene)
{	
	mCollisions = new cArray<phCollision*>(nReservedCollisions);
}

phCollisionManager::~phCollisionManager()
{	
	safe_release(mCollisions);
}

void phCollisionManager::checkCollisions()
{

}

phCollision* phCollisionManager::isCollide( phObject* objA, phObject* objB )
{
	return NULL;
}
