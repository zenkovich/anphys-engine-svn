#include "mesh_test.h"

#include "mesh_merge.h"

void cMeshTest::generateMainPlaneMesh( const vec3& size, const int segx, const int segy )
{
	createPlaneMesh(mMainMeshVericies, mMainMeshPolygons, size, segx, segy);
}

void cMeshTest::generateSecondaryPlaneMesh( const vec3& size, const int segx, const int segy )
{
	createPlaneMesh(mSecondaryMeshVericies, mSecondaryMeshPolygons, size, segx, segy);
}

void cMeshTest::generateSecondaryTorusMesh( const vec3& size, const int circleSegs, const int circles )
{
	createTorusMesh(mSecondaryMeshVericies, mSecondaryMeshPolygons, size, circleSegs, circles);
}

void cMeshTest::generateSecondarySphereMesh( const vec3& size, const int circleSegs, const int circles )
{
	createSphereMesh(mSecondaryMeshVericies, mSecondaryMeshPolygons, size, circleSegs, circles);
}

void cMeshTest::generateSecondaryCubeMesh( const vec3& size, const int segsx, const int segsy, const int segsz )
{
	createCubeMesh(mSecondaryMeshVericies, mSecondaryMeshPolygons, size, segsx, segsy, segsz);
}

void cMeshTest::randomizeMainMesh( const vec3& range )
{
	randomizeMesh(mMainMeshVericies, mMainMeshPolygons, range);
}

void cMeshTest::randomizeSecondaryMesh( const vec3& range )
{
	randomizeMesh(mSecondaryMeshVericies, mSecondaryMeshPolygons, range);
}

void cMeshTest::processMeshMerge( const vec3& secMeshOffs, const mat3x3& secMeshOrient )
{
	int aVertCount = mMainMeshVericies.size();
	vec3* aVert = new vec3[aVertCount*2];
	memcpy(aVert, &mMainMeshVericies[0], aVertCount*sizeof(vec3));

	int aPolyCount = mMainMeshPolygons.size();
	poly3* aPoly = new poly3[aPolyCount*2];
	memcpy(aPoly, &mMainMeshPolygons[0], aPolyCount*sizeof(poly3));

	int bVertCount = mSecondaryMeshVericies.size();
	vec3* bVert = new vec3[bVertCount];
	int i = 0;
	for (VecArr::iterator it = mSecondaryMeshVericies.begin(); it != mSecondaryMeshVericies.end(); ++it, i++)
	{
		bVert[i] = (*it)*secMeshOrient + secMeshOffs;
	}

	int bPolyCount = mSecondaryMeshPolygons.size();
	poly3* bPoly = new poly3[bPolyCount];
	memcpy(bPoly, &mSecondaryMeshPolygons[0], bPolyCount*sizeof(poly3));

	int resVertCount = 0;
	int resPolyCount = 0;

	mergeMeshes(aVert, aVertCount, aPoly, aPolyCount, bVert, bVertCount, bPoly, bPolyCount, &resVertCount, &resPolyCount);

	mMainMeshVericies.clear();
	mMainMeshPolygons.clear();
	
	mMainMeshVericies.reserve(resVertCount);
	mMainMeshPolygons.reserve(resPolyCount);
	
	for (int i = 0; i < resVertCount; i++)
		mMainMeshVericies.push_back(aVert[i]);
	
	for (int i = 0; i < resPolyCount; i++)
		mMainMeshPolygons.push_back(aPoly[i]);
	
	delete[] aVert;
	delete[] bVert;
	delete[] aPoly;
	delete[] bPoly;
}

void cMeshTest::createPlaneMesh( VecArr& verticies, PolyArr& polygons, const vec3& size, const int segx, const int segy )
{
	verticies.clear(); polygons.clear();
	verticies.reserve((segx + 1)*(segy + 1));
	polygons.reserve(segx*segy*2);

	for (int i = 0; i < segx + 1; i++)
	{
		float xCoef = (float)i/(float)segx;
		float xCoord = size.x*xCoef - size.x*0.5f;

		int ii = i*segy;

		for (int j = 0; j < segy + 1; j++)
		{
			float yCoef = (float)j/(float)segy;
			float yCoord = size.y*yCoef - size.y*0.5f;

			verticies.push_back(vec3(xCoord, 0, yCoord));

			if (i > 0 && j > 0)
			{
				int a = j + ii - 1 - segy;
				int b = j + ii - segy;
				int c = j + ii - 1;
				int d = j + ii;
				
				polygons.push_back(poly3(a, b, d));
				polygons.push_back(poly3(a, d, c));
			}
		}
	}
}

void cMeshTest::createTorusMesh( VecArr& verticies, PolyArr& polygons, const vec3& size, const int circleSegs, const int circles )
{
	verticies.clear(); polygons.clear();
	verticies.reserve((circleSegs + 1)*(circles + 1));
	polygons.reserve(circleSegs*circles*2);

	for (int i = 0; i < circles; i++)
	{
		float xCoef = (float)i/(float)circles;

		float rangle = 3.1415926f*2.0f*xCoef;
		float rsn = sinf(rangle), rcs = cosf(rangle);
		vec3 raxis(rsn, 0, rcs);
		vec3 yaxis(0, size.y/size.x, 0);

		int ii = i*circleSegs;

		for (int j = 0; j < circleSegs; j++)
		{
			float yCoef = (float)j/(float)circleSegs;

			float angle = 3.1415926f*2.0f*yCoef;
			float sn = sinf(angle), cs = cosf(angle);

			float vx = sn*size.x + size.z, vy = cs*size.x;

			verticies.push_back( raxis*vx + yaxis*vy );

			if (i > 0 && j > 0)
			{
				int a = j + ii - 1 - circleSegs;
				int b = j + ii - circleSegs;
				int c = j + ii - 1;
				int d = j + ii;
				
				polygons.push_back(poly3(a, b, d));
				polygons.push_back(poly3(a, d, c));
			}
		}
	}
}

void cMeshTest::createSphereMesh( VecArr& verticies, PolyArr& polygons, const vec3& size, const int circleSegs, const int circles )
{

}

void cMeshTest::createCubeMesh( VecArr& verticies, PolyArr& polygons, const vec3& size, const int segsx, const int segsy, const int segsz )
{

}

void cMeshTest::randomizeMesh( VecArr& verticies, PolyArr& polygons, const vec3& range )
{
	vec3 hr = range*0.5f;
	for (VecArr::iterator it = verticies.begin(); it != verticies.end(); ++it)
	{
		(*it) += vec3( random(-hr.x, hr.x), random(-hr.y, hr.y), random(-hr.y, hr.y) );
	}
}
