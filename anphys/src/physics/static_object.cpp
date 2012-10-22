#include "static_object.h"

#include "collision_geometry.h"
#include "collision.h"
#include "collision_point.h"

phStaticObject::phStaticObject():phObject()
{
	mCollisionGeometry = new phCollisionGeometry(this);

	mType = static_physics_object;
}

phStaticObject::phStaticObject(phScene* scene):phObject(scene)
{
	mCollisionGeometry = new phCollisionGeometry(this);

	mType = static_physics_object;
}

phStaticObject::phStaticObject(vec3 position, mat3x3 orientation, phCollisionGeometry* collisionGeometry):phObject(),  
							 mPosition(position), mOrient(orientation), mCollisionGeometry(collisionGeometry)
{
	mCollisionGeometry->mObject = this;

	mType = static_physics_object;
}

phStaticObject::~phStaticObject()
{
	safe_release(mCollisionGeometry);
}

void phStaticObject::update(float dt)
{
	mCollisionGeometry->update(dt);
}

void phStaticObject::setPos(vec3 pos)
{
	mPosition = pos;
}

void phStaticObject::setOrient(mat3x3 orient)
{
	mOrient = orient;
}

phCollision* phStaticObject::checkCollision(phObject* object, phCollision* collision)
{
	phCollisionGeometry* collisionGeomB = object->getCollisionGeometry();
	if (!collisionGeomB) return NULL;

	return mCollisionGeometry->checkCollision(collisionGeomB, collision);
}

phCollisionGeometry* phStaticObject::getCollisionGeometry()
{
	return mCollisionGeometry;
}
