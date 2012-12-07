#include "box_collision_geometry.h"

#include "collision.h"
#include "physics_object.h"
#include "collision_geometry.h"
#include "box_box_cd.h"

//utils
#include "../util/debug/render_stuff.h"

phBoxCollisionGeometry::phBoxCollisionGeometry():
	phCollisionGeometryPart()
{
	initCollisionData();
}

phBoxCollisionGeometry::phBoxCollisionGeometry(phCollisionGeometry* collisionGeometry):
	phCollisionGeometryPart(collisionGeometry)
{
	initCollisionData();
}

phBoxCollisionGeometry::phBoxCollisionGeometry(phCollisionGeometry* collisionGeometry, mat3x3 rotate, 
											   vec3 offset, vec3 size):
		phCollisionGeometryPart(collisionGeometry, rotate, offset), mSize(size)
{
	initCollisionData();
}

phBoxCollisionGeometry::phBoxCollisionGeometry(mat3x3 rotate, vec3 offset, vec3 size):
		phCollisionGeometryPart(rotate, offset), mSize(size)
{
	initCollisionData();
}

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

	mVerticies[0]->mVertex = axisX*-1.0f + axisY - axisZ + mWorldPosition;
	mVerticies[1]->mVertex = axisX       + axisY - axisZ + mWorldPosition;
	mVerticies[2]->mVertex = axisX       - axisY - axisZ + mWorldPosition;
	mVerticies[3]->mVertex = axisX*-1.0f - axisY - axisZ + mWorldPosition;
	mVerticies[4]->mVertex = axisX*-1.0f - axisY + axisZ + mWorldPosition;
	mVerticies[5]->mVertex = axisX       - axisY + axisZ + mWorldPosition;
	mVerticies[6]->mVertex = axisX       + axisY + axisZ + mWorldPosition;
	mVerticies[7]->mVertex = axisX*-1.0f + axisY + axisZ + mWorldPosition;

	mSupportGeom.calculateParametres();

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

void phBoxCollisionGeometry::initCollisionData()
{
	static int edgesIndexes[12][2] = { { 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 },
	                            { 7, 6 }, { 6, 5 }, { 5, 4 }, { 4, 7 },
	                            { 0, 7 }, { 1, 6 }, { 2, 5 }, { 3, 4 } };

	static int polygons[6][4] = { { 0, 1, 2, 3 }, 
		                          { 2, 11, 6, 10 }, 
		                          { 4, 5, 6, 7 }, 
		                          { 0, 8, 4, 9 }, 
		                          { 1, 9, 5, 10 }, 
		                          { 3, 8, 7, 11 } };

	for (int i = 0; i < 8; i++)
	{
		mVerticies[i] = new phCollisionVertex;
		mSupportGeom.mElements.push_back(mVerticies[i]);
	}

	phCollisionEdge* edges[12];

	for (int i = 0; i < 12; i++)
	{
		edges[i] = new phCollisionEdge(mVerticies[edgesIndexes[i][0]], mVerticies[edgesIndexes[i][1]]);		
		mSupportGeom.mElements.push_back(edges[i]);
	}

	for (int i = 0; i < 6; i++)
	{
		phCollisionPolygon* newCollisionPolygon = 
			new phCollisionPolygon(edges[polygons[i][0]], edges[polygons[i][1]], edges[polygons[i][2]],
			                       edges[polygons[i][3]]);
		mSupportGeom.mElements.push_back(newCollisionPolygon);
	}

	mSupportGeom.initProjectionBuffers();
}
