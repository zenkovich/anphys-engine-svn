#include "collision.h"

#include "collision_point.h"

phCollision::phCollision():mObjectA(NULL), mObjectB(NULL), mCollisionData(NULL)
{ 
	mPoints.reserve(COLLISION_RESERVED_POINTS); 
	for (int i = 0; i < COLLISION_RESERVED_POINTS; i++)
		mPoints.push_back(new phCollisionPoint(this, NULL, NULL));

	mCollisionData = new float[COLLISION_DATA_SIZE];
}

phCollision::phCollision(phObject* objectA, phObject* objectB):mObjectA(NULL), mObjectB(NULL), mCollisionData(NULL)
{
	setObjects(objectA, objectB);
	mPoints.reserve(COLLISION_RESERVED_POINTS);

	mCollisionData = new float[COLLISION_DATA_SIZE];
}

phCollision::~phCollision()
{
	for (PointsList::iterator it = mPoints.begin(); it != mPoints.end(); it++)
		safe_release(*it);
	clear();
	safe_release(mCollisionData);
}

void phCollision::setObjects(phObject* objectA, phObject* objectB)
{
	clear();
	safe_release(mCollisionData);
	mObjectA = objectA;
	mObjectB = objectB;
	mCollisionData = new float[COLLISION_DATA_SIZE];
}

phCollisionPoint* phCollision::addPoint()
{
	for (PointsList::iterator it = mPoints.begin(); it != mPoints.end(); it++)
		if (!(*it)->mUses) return (*it);

	phCollisionPoint* newPoint = new phCollisionPoint(this, NULL, NULL);
	mPoints.push_back(newPoint);

	return newPoint;
}

void phCollision::clear()
{
	for (PointsList::iterator it = mPoints.begin(); it != mPoints.end(); it++)
		(*it)->mUses = false;
}