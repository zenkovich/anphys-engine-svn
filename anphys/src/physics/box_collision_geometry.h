#ifndef BOX_COLLISION_GEOMETRY_H
#define BOX_COLLISION_GEOMETRY_H

#include "collision_geometry_part.h"

#include "cd_stuff.h"

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

	phCollisionSupportGeom mSupportGeom;
	phCollisionVertex*     mVerticies[6];
	
	//                                                                                          
	//                                                                                          
	//                                                                                          
	//                       (7)---------------------|4|--------------------(6)                 
	//                       /|                                             /|                  
	//                      /                                              / |                  
	//                     /  |               /                           /  |                  
	//                   |8|               -[3]-       |                |9|  |                  
	//                   /    |             /        -[2]-              /   |5|                 
	//                  /    |7|                       |               /     |                  
	//                 /      |                                       /      |                  
	//               (0)----------------------|0|------------------(1)       |               |/   
	//      |/        |       |                                      |  ------------------- [4]
	//     [5]-----------    (4)-- -- -- -- -- -- -- |6|-- -- -- -- -|- -- -(5)             /|  
	//     /|         |       /                                      |       /                  
	//                |                                              |      /                   
	//                |     /                |                      |1|    /                    
	//               |3|  |11|             -[0]-          /          |   |10|                   
	//                |   /                  |         -[1]-         |   /                      
	//                |                                 /            |  /                     
	//                | /                                            | /                        
	//                |                                              |/
	//               (3)---------------------|2|--------------------(2)                        
	//                                                                                          
                                                                                  

	phBoxCollisionGeometry();
	phBoxCollisionGeometry(phCollisionGeometry* collisionGeometry);
	phBoxCollisionGeometry(phCollisionGeometry* collisionGeometry, mat3x3 rotate, vec3 offset, vec3 size);
	phBoxCollisionGeometry(mat3x3 rotate, vec3 offset, vec3 size);

	void preUpdate(float dt);
	void update(float dt);
	void postUpdate(float dt);
	
	phCollision* checkCollision(phCollisionGeometryPart* collisionGeometryPart, phCollision* collision);

	void initCollisionData();
};


#endif //BOX_COLLISION_GEOMETRY_H