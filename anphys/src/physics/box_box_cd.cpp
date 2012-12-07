#include "box_box_cd.h"

#include "box_collision_geometry.h"
#include "collision.h"
#include "collision_point.h"
#include "cd_stuff.h"
#include "support_geom.h"

//utils
#include "../util/utils.h"
#include "../util/debug/render_stuff.h"


vec3 *supportA = new vec3[4], *supportB = new vec3[4];
phCollision* checkCollisionBoxBox( phBoxCollisionGeometry* geomA, phBoxCollisionGeometry* geomB, phCollision* collision )
{
	vec3 geometriesDistanceVec = geomB->mWorldPosition - geomA->mWorldPosition;
	float geometriesDistance = geometriesDistanceVec.len();

	mat3x3 axisProjections, absAxisProjections;
	for (unsigned int i = 0; i < 3; i++)
	{
		for (unsigned int j = 0; j < 3; j++)
		{
			float dotProduct = geomA->mWorldOrient[i][0]*geomB->mWorldOrient[j][0] + 
				               geomA->mWorldOrient[i][1]*geomB->mWorldOrient[j][1] + 
							   geomA->mWorldOrient[i][2]*geomB->mWorldOrient[j][2];
			axisProjections[i][j] = dotProduct;
			absAxisProjections[i][j] = fabs(dotProduct);
		}
	}

	vec3 dotAPosDiff(geometriesDistanceVec.x*geomA->mWorldOrient[0][0] + 
		             geometriesDistanceVec.y*geomA->mWorldOrient[0][1] + 
					 geometriesDistanceVec.z*geomA->mWorldOrient[0][2],
					 
					 geometriesDistanceVec.x*geomA->mWorldOrient[1][0] + 
		             geometriesDistanceVec.y*geomA->mWorldOrient[1][1] + 
					 geometriesDistanceVec.z*geomA->mWorldOrient[1][2],
	
	                 geometriesDistanceVec.x*geomA->mWorldOrient[2][0] + 
		             geometriesDistanceVec.y*geomA->mWorldOrient[2][1] + 
					 geometriesDistanceVec.z*geomA->mWorldOrient[2][2]);

	vec3 separationAxis;
	float penetrationDepth = 999999.0f;
	float aAxisProjection, bAxisProjection;
	unsigned int separationAxisId = 0;

	vec3 apt;

	//checking faces axes
	phBoxCollisionGeometry* faceAxisGeometries[2][2] = { { geomA, geomB }, { geomB, geomA } };
	for (unsigned int i = 0; i < 2; i++)
	{
		phBoxCollisionGeometry* faceGeomA = faceAxisGeometries[i][0];
		phBoxCollisionGeometry* faceGeomB = faceAxisGeometries[i][1];

		vec3 bBoundaryVertex = faceGeomB->mPoints[4] - faceGeomB->mWorldPosition;
		for (int j = 0; j < 3; j++)
		{
			vec3 axis(faceGeomA->mWorldOrient[j][0], faceGeomA->mWorldOrient[j][1], faceGeomA->mWorldOrient[j][2]);

			float aProjection = faceGeomA->mHalfSize.get(j);
			float bProjection = (i == 0) ? 
				(faceGeomB->mHalfSize.x*absAxisProjections[j][0] + faceGeomB->mHalfSize.y*absAxisProjections[j][1] + faceGeomB->mHalfSize.z*absAxisProjections[j][2]) :
			    (faceGeomB->mHalfSize.x*absAxisProjections[0][j] + faceGeomB->mHalfSize.y*absAxisProjections[1][j] + faceGeomB->mHalfSize.z*absAxisProjections[2][j]);

			float distanceProjection = geometriesDistanceVec*axis;
			float depth = aProjection + bProjection - absf(distanceProjection);


			//vec3 originDbgPoint = faceGeomA->mWorldPosition

			//getRenderStuff().addBlueArrow(faceGeomA->mWorldPosition + axis*(aProjection - depth), faceGeomA->mWorldPosition + axis*aProjection);
			/*getRenderStuff().addBlueArrow(faceGeomA->mWorldPosition, faceGeomA->mWorldPosition + axis*100.0f);
			getRenderStuff().addGreenCube(faceGeomA->mWorldPosition + axis*aProjection);
			getRenderStuff().addRedCube(faceGeomA->mWorldPosition + axis*distanceProjection);
			getRenderStuff().addBlueCube(faceGeomA->mWorldPosition + axis*(distanceProjection - bProjection));
			getRenderStuff().addGreenArrow(faceGeomA->mWorldPosition + axis*distanceProjection, 
				faceGeomA->mWorldPosition + axis*(distanceProjection - bProjection));*/

			if (depth > 0 && depth < penetrationDepth)
			{
				float distanceProjectionSign = sign(distanceProjection);
				axis *= distanceProjectionSign;

				separationAxis = axis;
				penetrationDepth = depth;

				aAxisProjection = aProjection;
				bAxisProjection = -bProjection;

				apt = faceGeomA->mWorldPosition + axis*(aProjection - depth);

				separationAxisId = j*2 + ( (distanceProjectionSign > 0) ? 1:0 ) + i*6;
			}
			if (depth < 0) return collision;
		}
	}

	int indexes[3][2] = { {1, 2} , {2, 0} , {0, 1} };

	
	for (unsigned int i = 0; i < 3; i++)
	{
		for (unsigned int j = 0; j < 3; j++)
		{
			vec3 axis = vec3(geomA->mWorldOrient[i][0], geomA->mWorldOrient[i][1], geomA->mWorldOrient[i][2])^
				        vec3(geomB->mWorldOrient[j][0], geomB->mWorldOrient[j][1], geomB->mWorldOrient[j][2]);

			float distanceProjection = axisProjections[indexes[i][0]][j]*dotAPosDiff.get(indexes[i][1]) -
				                       axisProjections[indexes[i][1]][j]*dotAPosDiff.get(indexes[i][0]);

			float aProjection = geomA->mHalfSize.get(indexes[i][0])*absAxisProjections[indexes[i][1]][j] + 
				                geomA->mHalfSize.get(indexes[i][1])*absAxisProjections[indexes[i][0]][j];

			float bProjection = geomB->mHalfSize.get(indexes[j][0])*absAxisProjections[i][indexes[j][1]] +
				                geomB->mHalfSize.get(indexes[j][1])*absAxisProjections[i][indexes[j][0]];
			
			float depth = aProjection + bProjection - absf(distanceProjection);

			if (depth < -0.001f) 
				getRenderStuff().addBlueArrow(geomA->mWorldPosition + axis*(aProjection - depth), geomA->mWorldPosition + axis*(aProjection));
			else 
				getRenderStuff().addGreenArrow(geomA->mWorldPosition + axis*(aProjection - depth), geomA->mWorldPosition + axis*(aProjection));
			
			if (depth < -0.001f) 
				return collision;

			if (depth < 0.001f) continue;

			float axisLength = axis.len();
			depth /= axisLength;
			if (depth < penetrationDepth)
			{
				penetrationDepth = depth;

				float distanceProjectionSign = sign(distanceProjection);
				separationAxis = axis/axisLength*(-distanceProjectionSign);

				aAxisProjection = aProjection/axisLength;
				bAxisProjection = -bProjection/axisLength;

				apt = geomA->mWorldPosition + axis*(aProjection - depth);

				separationAxisId = 12 + (i*3 + j)*2 + ( (distanceProjectionSign > 0) ? 1:0 );
			}
		}
	}

	int supportACount = 0, supportBCount = 0;

	float d = 0.1f;

	*gLog << formatStr("Separation axis id is %i:\n", separationAxisId);

	float dst = 9999999.0f;
	dst = geomA->mSupportGeom.projectOnAxis(separationAxis, geomA->mWorldPosition);
	dst = geomB->mSupportGeom.projectOnAxis(separationAxis*(-1.0f), geomB->mWorldPosition);
	geomA->mSupportGeom.copyTempProjections();
	geomB->mSupportGeom.copyTempProjections();

	checkIntersection(&geomA->mSupportGeom, aAxisProjection, &geomB->mSupportGeom, bAxisProjection, collision);
	
	/*if (separationAxisId >= 0 && separationAxisId < 12) //face normal is separation axis
	{
		*gLog << "Face normal axis\n";

		phBoxCollisionGeometry* currGeomA = geomA;
		phBoxCollisionGeometry* currGeomB = geomB;
		vec3* currSupportA = supportA;
		vec3* currSupportB = supportB;
		int*  currSupportACount = &supportACount;
		int*  currSupportBCount = &supportBCount;
		float currbAxisProjection = bAxisProjection;

		int currSeparationAxisId = separationAxisId;
		if (currSeparationAxisId > 5)
		{
			currSeparationAxisId -= 6;
			currGeomA = geomB; currGeomB = geomA;
			currSupportA = supportB; currSupportB = supportA;
			currSupportACount = &supportBCount; currSupportBCount = &supportACount;
			currbAxisProjection = aAxisProjection;
		}

		static int aIndexes[6][4] = { { 0, 3, 4, 7 },    //x positive
		                              { 1, 6, 5, 2 },    //x negative
		                              { 2, 3, 4, 5 },    //y positive
		                              { 0, 7, 6, 1 },    //y negative
		                              { 0, 1, 2, 3 },    //z positive
		                              { 6, 7, 4, 5 } };  //z negative

		*currSupportACount = 4;
		currSupportA[0] = currGeomA->mPoints[aIndexes[currSeparationAxisId][0]];
		currSupportA[1] = currGeomA->mPoints[aIndexes[currSeparationAxisId][1]];
		currSupportA[2] = currGeomA->mPoints[aIndexes[currSeparationAxisId][2]];
		currSupportA[3] = currGeomA->mPoints[aIndexes[currSeparationAxisId][3]];

		findNearPoints(currGeomB->mPoints, 8, currSupportB, *currSupportBCount, separationAxis, currGeomB->mWorldPosition,
				       currbAxisProjection - d, currbAxisProjection + d);
	}
	else //edge-edge cross product is separation axis
	{
		*gLog << "egde-edge\n";

		findNearPoints(geomA->mPoints, 8, supportA, supportACount, separationAxis, geomA->mWorldPosition,
			aAxisProjection - d, aAxisProjection + d);
		
		findNearPoints(geomB->mPoints, 8, supportB, supportBCount, separationAxis, geomB->mWorldPosition,
				bAxisProjection - d, bAxisProjection + d);
	}*/

	for (int i = 0; i < supportACount; i++)
	{
		getRenderStuff().addBlueCube(supportA[i]);
		if (supportACount > 2)
		{
			getRenderStuff().addBlueArrow(supportA[i], supportA[(i + 1)%supportACount]);
		}
	}

	for (int i = 0; i < supportBCount; i++)
	{
		getRenderStuff().addBlueCube(supportB[i]);
		if (supportBCount > 2)
		{
			getRenderStuff().addBlueArrow(supportB[i], supportB[(i + 1)%supportBCount]);
		}
	}

	

	phCollisionPoint* pt = collision->addPoint();
	pt->mNormal = separationAxis*penetrationDepth;
	getRenderStuff().addGreenArrow(apt, apt + separationAxis*penetrationDepth);

	return collision;
}
