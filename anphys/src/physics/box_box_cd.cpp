#include "box_box_cd.h"

#include "box_collision_geometry.h"
#include "collision.h"
#include "collision_point.h"
#include "cd_stuff.h"

//utils
#include "../util/debug/render_stuff.h"


phCollision* checkCollisionBoxBox( phBoxCollisionGeometry* geomA, phBoxCollisionGeometry* geomB, phCollision* collision )
{
	vec3 geometriesDistanceVec = geomB->mWorldPosition - geomA->mWorldPosition;
	float geometriesDistance = geometriesDistanceVec.len();

	vec3 separationAxis;
	float penetrationDepth = 999999.0f;

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
			float bProjection = absf( bBoundaryVertex*axis );
			float distanceProjection = geometriesDistanceVec*axis;
			float depth = aProjection + bProjection - absf(distanceProjection);

			axis *= sign(distanceProjection);

			//vec3 originDbgPoint = faceGeomA->mWorldPosition

			getRenderStuff().addBlueArrow(faceGeomA->mWorldPosition + axis*(aProjection - depth), faceGeomA->mWorldPosition + axis*aProjection);
			/*getRenderStuff().addBlueArrow(faceGeomA->mWorldPosition, faceGeomA->mWorldPosition + axis*100.0f);
			getRenderStuff().addGreenCube(faceGeomA->mWorldPosition + axis*aProjection);
			getRenderStuff().addRedCube(faceGeomA->mWorldPosition + axis*distanceProjection);
			getRenderStuff().addBlueCube(faceGeomA->mWorldPosition + axis*(distanceProjection - bProjection));
			getRenderStuff().addGreenArrow(faceGeomA->mWorldPosition + axis*distanceProjection, 
				faceGeomA->mWorldPosition + axis*(distanceProjection - bProjection));*/

			if (depth > 0 && depth < penetrationDepth)
			{
				separationAxis = axis;
				penetrationDepth = depth;
			}
			if (depth < 0) return collision;
		}
	}

	getRenderStuff().addGreenArrow(vec3(5), vec3(5) + separationAxis*penetrationDepth);

	return collision;
}
