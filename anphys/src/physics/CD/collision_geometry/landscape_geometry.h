#ifndef LANDSCAPE_GEOMETRY_H
#define LANDSCAPE_GEOMETRY_H

#include "collision_geometry_part.h"

struct phLandscapeCollisionGeometry:public phCollisionGeometryPart
{
	/*from phCollisionGeometryPart
	{
		mat3x3 mRotate;
		vec3   mOffset;

		vec3 mWorldPosition;
		mat3x3 mWorldOrient;

		phCollisionGeometry* mCollisionGeometry;
		AABB mAABB;

		phCollisionGeometryPart(): mCollisionGeometry(NULL) {}
		phCollisionGeometryPart(phCollisionGeometry* collisionGeometry): mCollisionGeometry(collisionGeometry) {}
		phCollisionGeometryPart(phCollisionGeometry* collisionGeometry, mat3x3 rotate, vec3 offset):
		mCollisionGeometry(collisionGeometry), mRotate(rotate), mOffset(offset) {}
		phCollisionGeometryPart(mat3x3 rotate, vec3 offset):
			mCollisionGeometry(NULL), mRotate(rotate), mOffset(offset) {}

		void preUpdate(float dt) {}
		void update(float dt) {}
		void postUpdate(float dt) {}
		
		phCollision* checkCollision(phCollisionGeometryPart* collisionGeometryPart, phCollision* collision) { return NULL; }
	}; */

	DEFINE_TYPE(phLandscapeCollisionGeometry)

	struct lVertex
	{
		vec3  mPosition;
		float mFrictionCoef;
		float mBounceCoef;

		lVertex():mFrictionCoef(1.0f), mBounceCoef(1.0f) {}
		lVertex(const vec3& pos, float frictionCoef = 1.0f, float bounceCoef = 1.0f):
			mPosition(pos), mFrictionCoef(frictionCoef), mBounceCoef(bounceCoef) {}
	};

	struct lPolygon
	{
		unsigned int a, b, c;
		lVertex*     pa;
		lVertex*     pb;
		lVertex*     pc;
				     
		vec3         nab, nac;
		float        abLength, acLength;

		AABB         aabb;

		lPolygon():a(0), b(0), c(0), pa(NULL), pb(NULL), pc(NULL), abLength(0), acLength(0) {}
		lPolygon(unsigned int ia, unsigned int ib, unsigned int ic, lVertex* verticies);
	};

	lVertex*     mVerticies;
	lPolygon*    mPolygons;
	unsigned int mVerticiesCount;
	unsigned int mPolygonsCount;

	phLandscapeCollisionGeometry();
	phLandscapeCollisionGeometry(phCollisionGeometry* collisionGeometry);

	void preUpdate(float dt);
	void update(float dt);
	void postUpdate(float dt);
	
	phCollision* checkCollision(phCollisionGeometryPart* collisionGeometryPart, phCollision* collision);

	void postInitialize();
};


#endif //LANDSCAPE_GEOMETRY_H