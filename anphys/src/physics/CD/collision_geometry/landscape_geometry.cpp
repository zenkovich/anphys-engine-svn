#include "landscape_geometry.h"

REGIST_TYPE(phLandscapeCollisionGeometry)
	
phLandscapeCollisionGeometry::lPolygon::lPolygon( unsigned int ia, unsigned int ib, unsigned int ic, lVertex* verticies )
{
	a = ia; b = ib; c = ic;
	pa = &verticies[a]; pb = &verticies[b]; pc = &verticies[c];

	aabb.mMin = pa->mPosition;
	aabb.mMax = pa->mPosition;

	for (unsigned int i = 0; i < 2; i++)
	{
		vec3 p;
		if (i == 0) pb->mPosition;
		if (i == 1) pc->mPosition;

		aabb.mMin.x = fmin(aabb.mMin.x, p.x);
		aabb.mMin.y = fmin(aabb.mMin.y, p.y);
		aabb.mMin.z = fmin(aabb.mMin.z, p.z);

		aabb.mMax.x = fmax(aabb.mMax.x, p.x);
		aabb.mMax.y = fmax(aabb.mMax.y, p.y);
		aabb.mMax.z = fmax(aabb.mMax.z, p.z);
	}

	vec3 ab = pb->mPosition - pa->mPosition;
	vec3 ac = pc->mPosition - pa->mPosition;

	abLength = ab.len();
	acLength = ac.len();

	nab = ab/abLength;
	nac = ac/acLength;
}

phLandscapeCollisionGeometry::phLandscapeCollisionGeometry():phCollisionGeometryPart(), mVerticies(NULL),
	mPolygons(NULL), mVerticiesCount(0), mPolygonsCount(0)
{

}

phLandscapeCollisionGeometry::phLandscapeCollisionGeometry( phCollisionGeometry* collisionGeometry ):
	phCollisionGeometryPart(collisionGeometry), mVerticies(NULL), mPolygons(NULL), mVerticiesCount(0), mPolygonsCount(0)
{

}

void phLandscapeCollisionGeometry::preUpdate( float dt )
{

}

void phLandscapeCollisionGeometry::update( float dt )
{

}

void phLandscapeCollisionGeometry::postUpdate( float dt )
{

}

phCollision* phLandscapeCollisionGeometry::checkCollision( phCollisionGeometryPart* collisionGeometryPart, 
	                                                       phCollision* collision )
{
	return collision;
}

void phLandscapeCollisionGeometry::postInitialize()
{

}
