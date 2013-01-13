#include "box_box_cd.h"

#include "box_collision_geometry.h"
#include "collision.h"
#include "collision_point.h"
#include "cd_stuff.h"
#include "support_geom.h"

//utils
#include "../util/utils.h"
#include "../util/debug/render_stuff.h"


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
	unsigned int edgeAxisA = 0, edgeAxisB = 0;

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

			if (getCDDebugLevel() >= 3)
			{
				getRenderStuff().addBlueArrow(faceGeomA->mWorldPosition + axis*(aProjection - depth), faceGeomA->mWorldPosition + axis*aProjection);
				getRenderStuff().addBlueArrow(faceGeomA->mWorldPosition, faceGeomA->mWorldPosition + axis*100.0f);
				getRenderStuff().addGreenCube(faceGeomA->mWorldPosition + axis*aProjection);
				getRenderStuff().addRedCube(faceGeomA->mWorldPosition + axis*distanceProjection);
				getRenderStuff().addBlueCube(faceGeomA->mWorldPosition + axis*(distanceProjection - bProjection));
				getRenderStuff().addGreenArrow(faceGeomA->mWorldPosition + axis*distanceProjection, 
					faceGeomA->mWorldPosition + axis*(distanceProjection - bProjection));
			}

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

			/*if (depth < -0.001f) 
				getRenderStuff().addBlueArrow(geomA->mWorldPosition + axis*(aProjection - depth), geomA->mWorldPosition + axis*(aProjection));
			else 
				getRenderStuff().addGreenArrow(geomA->mWorldPosition + axis*(aProjection - depth), geomA->mWorldPosition + axis*(aProjection));
			*/
			if (depth < -0.001f) 
				return collision;

			if (depth < 0.001f) continue;

			float axisLength = 1.0f/axis.len();
			depth *= axisLength;
			if (depth < penetrationDepth)
			{
				penetrationDepth = depth;

				float distanceProjectionSign = sign(distanceProjection);
				separationAxis = axis*axisLength*(-distanceProjectionSign);

				aAxisProjection = aProjection*axisLength;
				bAxisProjection = -bProjection*axisLength;

				apt = geomA->mWorldPosition + axis*(aProjection - depth);

				separationAxisId = 12 + 1;
				edgeAxisA = i;
				edgeAxisB = j;
			}
		}
	}
	
	if (getCDDebugLevel() >= 2) *gLog << formatStr("Separation axis id is %i:", separationAxisId);
	
	if (separationAxisId >= 0 && separationAxisId < 12) //face normal is separation axis
	{
		if (getCDDebugLevel() >= 2) *gLog << "Face normal axis\n";

		phBoxCollisionGeometry* currGeomA = geomA;
		phBoxCollisionGeometry* currGeomB = geomB;
		float currbAxisProjection = bAxisProjection;
		vec3 currSeparationAxis(-separationAxis.x, -separationAxis.y, -separationAxis.z);
									  
		static int aIndexes[6][9] = { {5, 9,  14, 13, 17, 18, 21, 22, 25 },
									  {4, 7,  15, 11, 16, 19, 20, 23, 24 },
									  {1, 8,  17, 12, 16, 20, 21, 22, 23 },
									  {3, 6,  14, 10, 15, 18, 19, 24, 25 },
									  {0, 6,  7,  8,  9,  18, 19, 20, 21 },
									  {2, 10, 11, 12, 13, 22, 23, 24, 25 }, };

		int currSeparationAxisId = separationAxisId;
		if (currSeparationAxisId > 5)
		{
			currSeparationAxisId -= 6;
			if (currSeparationAxisId % 2 != 0) currSeparationAxisId -= 1;
			else                               currSeparationAxisId += 1;
			currGeomA = geomB; currGeomB = geomA;
			currbAxisProjection = aAxisProjection;
			currSeparationAxis.x = -currSeparationAxis.x;
			currSeparationAxis.y = -currSeparationAxis.y;
			currSeparationAxis.z = -currSeparationAxis.z;
		}

		int idx = 0;
		for (phCollisionElementsList::iterator it = currGeomA->mSupportGeom.mProbablyIntersectingElements.begin();
			 it != currGeomA->mSupportGeom.mProbablyIntersectingElements.end() && idx < 9; ++it, ++idx)
		{
			*it = currGeomA->mSupportGeom.mElements[aIndexes[currSeparationAxisId][idx]];
		}
		currGeomA->mSupportGeom.mProbablyIntersectingElements[9] = NULL;

		float projB = 0.0f;
		currGeomB->mSupportGeom.projectOnAxis(currSeparationAxis, currGeomB->mWorldPosition, &projB);
	}
	else //edge-edge cross product is separation axis
	{
		if (getCDDebugLevel() >= 2) *gLog << "egde-edge\n";

		vec3 invSeparationAxis(-separationAxis.x, -separationAxis.y, -separationAxis.z);

		static int edgesIndexes[3][4] = { { 6, 8, 12, 10 }, { 7, 11, 13, 9 }, { 14, 15, 16, 17 } };

		phCollisionEdge* edgeA, *edgeB;

		float maxProjectionA = 0.0f, maxProjectionB = 0.0f;
		for (short i = 0; i < 4; i++)
		{
			phCollisionEdge* currEdgeA = 
				static_cast<phCollisionEdge*>(geomA->mSupportGeom.mElements[edgesIndexes[edgeAxisA][i]]);

			phCollisionEdge* currEdgeB = 
				static_cast<phCollisionEdge*>(geomB->mSupportGeom.mElements[edgesIndexes[edgeAxisB][i]]);
			
			float projA = (currEdgeA->mFirstVertex->mVertex - geomA->mWorldPosition)*separationAxis;
			float projB = (currEdgeB->mFirstVertex->mVertex - geomB->mWorldPosition)*invSeparationAxis;
			
			if (projA > maxProjectionA)
			{
				maxProjectionA = projA;
				edgeA = currEdgeA;
			}
			if (projB > maxProjectionB)
			{
				maxProjectionB = projB;
				edgeB = currEdgeB;
			}
		}
			
		edgeA->fillSupportGeomData(geomA->mSupportGeom.mProbablyIntersectingElements, separationAxis);
		edgeB->fillSupportGeomData(geomB->mSupportGeom.mProbablyIntersectingElements, invSeparationAxis);

	/*	edgeA->showDbgGraphics();
		edgeB->showDbgGraphics();*/

		/*float projA = 0.0f, projB = 0.0f;
		geomA->mSupportGeom.projectOnAxis(separationAxis, geomA->mWorldPosition, &projA);
		
		geomB->mSupportGeom.projectOnAxis(separationAxis*(-1.0f), geomB->mWorldPosition, &projB);*/
	}

	if (getCDDebugLevel() >= 2)
	{
		for (phCollisionElementsList::iterator it = geomA->mSupportGeom.mProbablyIntersectingElements.begin();
				 it != geomA->mSupportGeom.mProbablyIntersectingElements.end(); ++it)
		{
			if (*it == NULL) break;
			(*it)->showDbgGraphics();
		}

		for (phCollisionElementsList::iterator it = geomB->mSupportGeom.mProbablyIntersectingElements.begin();
				 it != geomB->mSupportGeom.mProbablyIntersectingElements.end(); ++it)
		{
			if (*it == NULL) break;
			(*it)->showDbgGraphics();
		}
	}

	checkIntersection(geomA->mSupportGeom.mProbablyIntersectingElements, 
		              geomB->mSupportGeom.mProbablyIntersectingElements, collision, separationAxis);

	if (getCDDebugLevel() >= 1)
	{
		for (phCollision::CollisionPointsList::ValuesList::iterator it = collision->mPoints->mValues.begin();
			 it != collision->mPoints->mValues.end(); it++)
		{		
			getRenderStuff().addGreenArrow((*it)->mPoint - (*it)->mNormal*(*it)->mDepth*0.5f, 
										   (*it)->mPoint + (*it)->mNormal*(*it)->mDepth*0.5f);
			getRenderStuff().addGreenCube((*it)->mPoint);

			getRenderStuff().addRedArrow((*it)->mPoint, (*it)->mPoint + (*it)->mNormal);
		}
	}

	return collision;
}
