#include "collision_geometry.h"

#include "collision_geometry_part.h"
#include "collision.h"

#include <algorithm>

phCollisionGeometry::phCollisionGeometry():mObject(NULL), mPerformance(1) {}

phCollisionGeometry::phCollisionGeometry(phObject* object):mObject(object), mPerformance(1) {}

phCollisionGeometry::phCollisionGeometry(phObject* object, phCollisionGeometryPart* part):mObject(object), mPerformance(1)
{
	addPart(part);
}

phCollisionGeometry::phCollisionGeometry(phCollisionGeometryPart* part)
{
	addPart(part);
}

phCollisionGeometry::~phCollisionGeometry()
{
	removeAllParts();
}

phCollisionGeometryPart* phCollisionGeometry::addPart(phCollisionGeometryPart* newPart)
{
	mParts.push_back(newPart);
	newPart->mCollisionGeometry = this;
	return newPart;
}

bool phCollisionGeometry::removePart(phCollisionGeometryPart* part)
{
	PartsList::iterator it = std::find(mParts.begin(), mParts.end(), part);
	if (it == mParts.end()) return false;

	safe_release(*it);
	mParts.erase(it);
	return true;
}

void phCollisionGeometry::removeAllParts()
{
	for (PartsList::iterator it = mParts.begin(); it != mParts.end(); it++)
		safe_release(*it);

	mParts.clear();
}

void phCollisionGeometry::preUpdate(float dt)
{
	for (PartsList::iterator it = mParts.begin(); it != mParts.end(); it++)
		(*it)->preUpdate(dt);
}

void phCollisionGeometry::update(float dt)
{
	for (PartsList::iterator it = mParts.begin(); it != mParts.end(); it++)
		(*it)->update(dt);
}

void phCollisionGeometry::postUpdate(float dt)
{
	for (PartsList::iterator it = mParts.begin(); it != mParts.end(); it++)
		(*it)->postUpdate(dt);
}

phCollision* phCollisionGeometry::checkCollision(phCollisionGeometry* collisionGeometry, phCollision* collision)
{
	return collision;
}
