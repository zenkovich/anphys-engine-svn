#ifndef BOX_COLLISION_GEOMETRY_H
#define BOX_COLLISION_GEOMETRY_H

#include "collision_geometry_part.h"

struct phBoxCollisionGeometry:public phCollisionGeometryPart
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

	vec3   mSize;
	vec3   mHalfSize;
	mat3x3 mSizeVectors;

	vec3   mPoints[8];
	//            ^ Y
	//            |       Z
	//            |      /
	//         5-------4/  
	//        /|      /| 
	//       0-------1 |
	//       | |  .  | |   ----> X
	//       | 6- - -|-7
	//       |/      |/
	//       3-------2 
	//
	//

	phBoxCollisionGeometry();
	phBoxCollisionGeometry(phCollisionGeometry* collisionGeometry);
	phBoxCollisionGeometry(phCollisionGeometry* collisionGeometry, mat3x3 rotate, vec3 offset, vec3 size);
	phBoxCollisionGeometry(mat3x3 rotate, vec3 offset, vec3 size);

	void preUpdate(float dt);
	void update(float dt);
	void postUpdate(float dt);
	
	phCollision* checkCollision(phCollisionGeometryPart* collisionGeometryPart, phCollision* collision);
};


#endif //BOX_COLLISION_GEOMETRY_H