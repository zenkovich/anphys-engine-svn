#include "collision_manager.h"

#include "collision_geometry/collision.h"
#include "collision_geometry/collision_geometry.h"
#include "../scenes/physics_scene.h"
#include "../objects/physics_object.h"

phCollisionManager::phCollisionManager():mScene(NULL), mIndex(0)
{
	mCollisions = new CollisionsList(nReservedCollisions);
}

phCollisionManager::phCollisionManager( phScene* scene ):mScene(scene), mIndex(0)
{	
	mCollisions = new CollisionsList(nReservedCollisions);
}

phCollisionManager::~phCollisionManager()
{	
	safe_release(mCollisions);
}

void phCollisionManager::checkCollisions()
{
	unsigned int newIndex = mIndex + 1;
	if (newIndex > 999999) newIndex = 0;

	for (phScene::ObjectsList::iterator it = mScene->mObjects.begin(); it != mScene->mObjects.end(); ++it)
	{
		for (phScene::ObjectsList::iterator jt = it + 1; jt != mScene->mObjects.end(); ++jt)
		{
			phObject* objectA = *it;
			phObject* objectB = *jt;
			
			phCollisionGeometry* geometryA = objectA->getCollisionGeometry();
			phCollisionGeometry* geometryB = objectB->getCollisionGeometry();

			if (!geometryA->mAABB.isIntersect(geometryB->mAABB)) continue;

			phCollision* collision = NULL;
			bool isStoredCollision = false;
			for (phCollisionGeometry::CollisionsList::iterator it = geometryA->mStoredCollisions.begin();
				 it != geometryA->mStoredCollisions.end();it++)
			{
				phCollision* storedCollision = *it;
				if ((storedCollision->mObjectB == objectA &&
					 storedCollision->mObjectA == objectB) ||
					(storedCollision->mObjectA == objectA &&
					 storedCollision->mObjectB == objectB))
				{
					collision = storedCollision;
					isStoredCollision = true;
					break;
				}
			}

			if (!collision)
			{
				collision = mCollisions->getFreeValue();
				collision->setObjects(objectA, objectB);
			}

			collision->mTempIndex = newIndex;
			geometryA->checkCollision(geometryB, collision);

			if (!(collision->mIndex != newIndex) && !isStoredCollision)
			{
				mCollisions->acceptValue(collision);
				geometryA->mStoredCollisions.push_back(collision);
				geometryB->mStoredCollisions.push_back(collision);
			}
				
			collision->freeUnusedPoints();
		}
	}

	for (CollisionsList::ValuesList::iterator it = mCollisions->mValues.begin(); it != mCollisions->mValues.end();)
	{
		if ((*it)->mIndex != newIndex)
		{
			phCollision* collision = *it;
			collision->mObjectA->getCollisionGeometry()->eraseStoredCollision(collision);
			collision->mObjectB->getCollisionGeometry()->eraseStoredCollision(collision);

			mCollisions->mFreeValues.push_back(*it);
			it = mCollisions->mValues.erase(it);
			continue;
		}

		it++;
	}

	mIndex = newIndex;
}

phCollision* phCollisionManager::isCollide( phObject* objA, phObject* objB )
{
	for (CollisionsList::ValuesList::iterator it = mCollisions->mValues.begin(); it != mCollisions->mValues.end(); it++)
	{
		if (((*it)->mObjectA == objA && (*it)->mObjectB == objB) ||
			((*it)->mObjectA == objB && (*it)->mObjectB == objA)) return *it;
	}

	return NULL;
}
