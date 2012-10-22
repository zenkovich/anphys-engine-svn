#include "box_collision_geometry.h"

#include "collision.h"
#include "physics_object.h"
#include "collision_geometry.h"
#include "box_box_cd.h"

phBoxCollisionGeometry::phBoxCollisionGeometry():phCollisionGeometryPart() {}

phBoxCollisionGeometry::phBoxCollisionGeometry(phCollisionGeometry* collisionGeometry):phCollisionGeometryPart(collisionGeometry) {}

phBoxCollisionGeometry::phBoxCollisionGeometry(phCollisionGeometry* collisionGeometry, mat3x3 rotate, 
											   vec3 offset, vec3 size):
		phCollisionGeometryPart(collisionGeometry, rotate, offset), mSize(size) {}

phBoxCollisionGeometry::phBoxCollisionGeometry(mat3x3 rotate, vec3 offset, vec3 size):
		phCollisionGeometryPart(rotate, offset), mSize(size) {}

void phBoxCollisionGeometry::preUpdate(float dt)
{
	if (mCollisionGeometry)
		if (mCollisionGeometry->mObject)
		{
			vec3 objPos = mCollisionGeometry->mObject->getPos();
			mat3x3 objOrient = mCollisionGeometry->mObject->getOrient();

			mWorldPosition = objPos + mOffset*objOrient;
			mWorldOrient = objOrient*mRotate;
		}

	vec3 axisX = vec3(mSize.x, 0, 0)*mWorldOrient;
	vec3 axisY = vec3(0, mSize.y, 0)*mWorldOrient;
	vec3 axisZ = vec3(0, 0, mSize.z)*mWorldOrient;

	vec3 hx = axisX*0.5f;
	vec3 hy = axisY*0.5f;
	vec3 hz = axisZ*0.5f;

	mSizeVectors[0][0] = axisX.x; mSizeVectors[0][1] = axisX.y; mSizeVectors[0][2] = axisX.z;
	mSizeVectors[1][0] = axisY.x; mSizeVectors[1][1] = axisY.y; mSizeVectors[1][2] = axisY.z;
	mSizeVectors[2][0] = axisZ.x; mSizeVectors[2][1] = axisZ.y; mSizeVectors[2][2] = axisZ.z;

	mPoints[0] = hx*-1.0f + hy - hz;
	mPoints[1] = hx       + hy - hz;
	mPoints[2] = hx       - hy - hz;
	mPoints[3] = hx*-1.0f - hy - hz;
	mPoints[4] = hx       + hy + hz;
	mPoints[5] = hx*-1.0f + hy + hz;
	mPoints[6] = hx*-1.0f - hy + hz;
	mPoints[7] = hx       - hy + hz;

	mAABB.computeFromPoints(mPoints, 8);
}

void phBoxCollisionGeometry::update(float dt) {}

void phBoxCollisionGeometry::postUpdate(float dt) {}

phCollision* phBoxCollisionGeometry::checkCollision(phCollisionGeometryPart* collisionGeometryPart, phCollision* collision)
{
	return checkCollisionBoxBox(this, static_cast<phBoxCollisionGeometry*>(collisionGeometryPart), collision);
}