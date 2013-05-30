#include "landscape_geometry.h"

REGIST_TYPE(phLandscapeCollisionGeometry)
	
lPolygon::lPolygon( unsigned int ia, unsigned int ib, unsigned int ic, lVertex* verticies )
{
	a = ia; b = ib; c = ic;
	pa = &verticies[a]; pb = &verticies[b]; pc = &verticies[c];

	aabb.mMin = pa->mPosition;
	aabb.mMax = pa->mPosition;

	for (unsigned int i = 0; i < 2; i++)
	{
		vec3 p;
		if (i == 0) p = pb->mPosition;
		if (i == 1) p = pc->mPosition;

		aabb.mMin.x = fmin(aabb.mMin.x, p.x);
		aabb.mMin.y = fmin(aabb.mMin.y, p.y);
		aabb.mMin.z = fmin(aabb.mMin.z, p.z);

		aabb.mMax.x = fmax(aabb.mMax.x, p.x);
		aabb.mMax.y = fmax(aabb.mMax.y, p.y);
		aabb.mMax.z = fmax(aabb.mMax.z, p.z);
	}
	
	vec3 ab = pb->mPosition - pa->mPosition;
	vec3 bc = pc->mPosition - pb->mPosition;
	vec3 ca = pa->mPosition - pc->mPosition;

	norm = (bc^ab).normalize();
	
	nab = norm^ab;
	nbc = norm^bc;
	nca = norm^ca;
}

bool lPolygon::isIntersect( const vec3& bottom, vec3* point, vec3* pnorm, float* depth )
{
	if ((bottom - pa->mPosition)*nab > 0 ||
		(bottom - pb->mPosition)*nbc > 0 ||
		(bottom - pc->mPosition)*nca > 0)
	{
		return false;
	}

	float ndepth = (pa->mPosition - bottom)*norm;

	if (ndepth < 0)
		return false;

	*pnorm = norm;
	*depth = ndepth;
	*point = bottom - norm*ndepth;

	return true;
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
	mPolygonsBufferSize = 100;
	mPolygonsBufferCount = 0;
	mPolygonsBuffer = new lPolygon*[mPolygonsBufferSize];
}

void phLandscapeCollisionGeometry::getPolygons( const AABB& aabb )
{
	mPolygonsBufferCount = 0;
	for (unsigned int i = 0; i < mPolygonsCount; i++)
	{
		if (mPolygons[i].aabb.isIntersect(aabb) && mPolygonsBufferCount < mPolygonsBufferSize)
			mPolygonsBuffer[mPolygonsBufferCount++] = &mPolygons[i];
	}
}

phLandscapeCollisionGeometry::~phLandscapeCollisionGeometry()
{
	safe_release_arr(mVerticies);
	safe_release_arr(mPolygons);
	safe_release_arr(mPolygonsBuffer);
}
