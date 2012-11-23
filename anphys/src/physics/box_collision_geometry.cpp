#include "box_collision_geometry.h"

#include "collision.h"
#include "physics_object.h"
#include "collision_geometry.h"
#include "box_box_cd.h"

//utils
#include "../util/debug/render_stuff.h"

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

	mHalfSize = mSize*0.5f;

	vec3 axisX = vec3(mHalfSize.x, 0, 0)*mWorldOrient;
	vec3 axisY = vec3(0, mHalfSize.y, 0)*mWorldOrient;
	vec3 axisZ = vec3(0, 0, mHalfSize.z)*mWorldOrient;

	mSizeVectors[0][0] = axisX.x; mSizeVectors[0][1] = axisX.y; mSizeVectors[0][2] = axisX.z;
	mSizeVectors[1][0] = axisY.x; mSizeVectors[1][1] = axisY.y; mSizeVectors[1][2] = axisY.z;
	mSizeVectors[2][0] = axisZ.x; mSizeVectors[2][1] = axisZ.y; mSizeVectors[2][2] = axisZ.z;

	mPoints[0] = axisX*-1.0f + axisY - axisZ + mWorldPosition;
	mPoints[1] = axisX       + axisY - axisZ + mWorldPosition;
	mPoints[2] = axisX       - axisY - axisZ + mWorldPosition;
	mPoints[3] = axisX*-1.0f - axisY - axisZ + mWorldPosition;
	mPoints[4] = axisX*-1.0f - axisY + axisZ + mWorldPosition;
	mPoints[5] = axisX       - axisY + axisZ + mWorldPosition;
	mPoints[6] = axisX       + axisY + axisZ + mWorldPosition;
	mPoints[7] = axisX*-1.0f + axisY + axisZ + mWorldPosition;

	/*for (int i = 0; i < 8; i++)
		getRenderStuff().addRedCube(mPoints[i]);

	getRenderStuff().addGreenCube(mWorldPosition);
	getRenderStuff().addRedArrow(mWorldPosition, mWorldPosition + axisX);
	getRenderStuff().addGreenArrow(mWorldPosition, mWorldPosition + axisY);
	getRenderStuff().addBlueArrow(mWorldPosition, mWorldPosition + axisZ);*/

	mAABB.computeFromPoints(mPoints, 8);
}

void phBoxCollisionGeometry::update(float dt) {}

void phBoxCollisionGeometry::postUpdate(float dt) {}

phCollision* phBoxCollisionGeometry::checkCollision(phCollisionGeometryPart* collisionGeometryPart, phCollision* collision)
{
	return checkCollisionBoxBox(this, static_cast<phBoxCollisionGeometry*>(collisionGeometryPart), collision);
}