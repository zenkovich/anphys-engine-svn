#include "collision.h"

#include "collision_point.h"

phCollision::phCollision():mObjectA(NULL), mObjectB(NULL), mCollisionData(NULL), mIndex(0), mTempIndex(0)
{ 
	mPoints = new cArray<phCollisionPoint>(nCollisionReservedPoints);

	mCollisionData = new char[nCollisionDataSize];
}

phCollision::phCollision(phObject* objectA, phObject* objectB):mObjectA(NULL), mObjectB(NULL), 
	mCollisionData(NULL), mIndex(0), mTempIndex(0)
{
	setObjects(objectA, objectB);
	mPoints = new cArray<phCollisionPoint>(nCollisionReservedPoints);

	mCollisionData = new char[nCollisionDataSize];
}

phCollision::~phCollision()
{
	safe_release(mPoints)
	safe_release(mCollisionData);
}

void phCollision::setObjects(phObject* objectA, phObject* objectB)
{
	clear();
	mObjectA = objectA;
	mObjectB = objectB;
}

phCollisionPoint* phCollision::addPoint()
{
	phCollisionPoint* newPoint = mPoints->push_back();
	newPoint->mCollision = this;
	newPoint->reset();

	return newPoint;
}

void phCollision::clear()
{
	mPoints->clear();
	memset(mCollisionData, 0, nCollisionDataSize);
}

void phCollision::freeUnusedPoints()
{
	for (CollisionPointsList::ValuesList::iterator it = mPoints->mValues.begin(); it != mPoints->mValues.end();)
	{
		if ((*it)->mIndex != mTempIndex)
		{
			mPoints->mFreeValues.push_back(*it);
			it = mPoints->mValues.erase(it);
			continue;
		}

		++it;
	}
}
