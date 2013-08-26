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
	vertexTexNorm* aVert = new vertexTexNorm[aVertCount*2];
	memcpy(aVert, &mMainMeshVericies[0], aVertCount*sizeof(vertexTexNorm));

	int aPolyCount = mMainMeshPolygons.size();
	poly3* aPoly = new poly3[aPolyCount*2];
	memcpy(aPoly, &mMainMeshPolygons[0], aPolyCount*sizeof(poly3));

	int bVertCount = mSecondaryMeshVericies.size();
	vertexTexNorm* bVert = new vertexTexNorm[bVertCount];
	int i = 0;
	vec3 bb, bb2;
	for (VecArr::iterator it = mSecondaryMeshVericies.begin(); it != mSecondaryMeshVericies.end(); ++it, i++)
	{
		bb.x = it->x; bb.y = it->y; bb.z = it->z;
		bb2 = bb*secMeshOrient + secMeshOffs;
		bVert[i].x = bb2.x; bVert[i].y = bb2.y; bVert[i].z = bb2.z;
		bVert[i].nx = it->nx; bVert[i].ny = it->ny; bVert[i].nz = it->nz;
		bVert[i].tu = it->tu; bVert[i].tv = it->tv;
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

		int ii = i*(segy + 1);

		for (int j = 0; j < segy + 1; j++)
		{
			float yCoef = (float)j/(float)segy;
			float yCoord = size.y*yCoef - size.y*0.5f;

			verticies.push_back(vertexTexNorm(xCoord, 0, yCoord, 0, 1, 0, xCoef, yCoef));

			if (i > 0 && j > 0)
			{
				int a = j + ii - 2 - segy;
				int b = j + ii - segy - 1;
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
	verticies.reserve(circleSegs*circles);
	polygons.reserve(circleSegs*circles*2);

	int maxVertId = circleSegs*circles;

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

			vec3 pt = raxis*vx + yaxis*vy;

			verticies.push_back( vertexTexNorm(pt.x, pt.y, pt.z, 0, 1, 0, xCoef, yCoef) );

			int a = (ii + j)%maxVertId;
			int b = (ii + j + circleSegs)%maxVertId;
			int d = (ii + j + 1 + circleSegs)%maxVertId;
			int c = (ii + j + 1)%maxVertId;
				
			polygons.push_back(poly3(d, b, a));
			polygons.push_back(poly3(c, d, a));
		}
	}
}

void cMeshTest::createSphereMesh( VecArr& verticies, PolyArr& polygons, const vec3& size, const int circleSegs, const int circles )
{
	verticies.clear(); polygons.clear();
	verticies.reserve(circleSegs*circles);
	polygons.reserve(circleSegs*circles*2);

	int maxVertId = circleSegs*circles;

	for (int i = 0; i < circles; i++)
	{
		float xCoef = (float)i/(float)circles;

		float rc = fabs(xCoef*2.0f - 1);
		float radius = rc*rc;

		vec3 xaxis(radius, 0, 0), zaxis(0, 0, radius);
		float ypos = xCoef*size.y;

		int ii = i*circleSegs;

		for (int j = 0; j < circleSegs; j++)
		{
			float yCoef = (float)j/(float)circleSegs;

			float angle = 3.1415926f*2.0f*yCoef;
			float sn = sinf(angle), cs = cosf(angle);
			
			vec3 pt = xaxis*sn + zaxis*cs;
			pt.x *= size.x;
			pt.z *= size.x;
			pt.y = ypos;

			verticies.push_back( vertexTexNorm(pt.x, pt.y, pt.z, 0, 1, 0, xCoef, yCoef) );

			int a = (ii + j)%maxVertId;
			int b = (ii + j + circleSegs)%maxVertId;
			int d = (ii + j + 1 + circleSegs)%maxVertId;
			int c = (ii + j + 1)%maxVertId;
				
			polygons.push_back(poly3(a, b, d));
			polygons.push_back(poly3(a, d, c));
		}
	}
}

void cMeshTest::randomizeMesh( VecArr& verticies, PolyArr& polygons, const vec3& range )
{
	vec3 hr = range*0.5f;
	for (VecArr::iterator it = verticies.begin(); it != verticies.end(); ++it)
	{
		(*it).x += random(-hr.x, hr.x);
		(*it).y += random(-hr.y, hr.y);
		(*it).z += random(-hr.z, hr.z);
	}
}

void cMeshTest::fillMainMeshData( std::vector<grRender3DObjectMesh*>& renderMeshes, const char* materialName )
{
	typedef std::vector<grRender3DObjectMesh*> MeshVec;

	int polyIdx = 0;

	int* buf = new int[mMainMeshVericies.size() + 32];

	for (MeshVec::iterator it = renderMeshes.begin(); it != renderMeshes.end(); ++it)
	{
		(*it)->clear();

		if (polyIdx < mMainMeshPolygons.size())
		{
			polyIdx = fillMainMeshPart(polyIdx, *it, materialName, buf);
		}
	}

	while (polyIdx < mMainMeshPolygons.size())
	{
		grRender3DObjectMesh* newMesh = new grRender3DObjectMesh(renderMeshes[0]->mRenderObjectsManager, 32000, 32000);
		renderMeshes[0]->mRenderObjectsManager->createObject(newMesh);

		polyIdx = fillMainMeshPart(polyIdx, newMesh, materialName, buf);

		renderMeshes.push_back(newMesh);
	}

	delete[] buf;
}

void cMeshTest::fillSecondaryMeshData( grRender3DObjectMesh* renderMesh, const char* materialName )
{
	renderMesh->clear();

	renderMesh->addPart(&mSecondaryMeshVericies[0], mSecondaryMeshVericies.size(), 
		                (int*)&mSecondaryMeshPolygons[0], mSecondaryMeshPolygons.size(), 
		                renderMesh->mRenderObjectsManager->mRender->mSurfaceMaterials->getSurfaceMaterial(materialName));
}

int cMeshTest::fillMainMeshPart( int polyStartIdx, grRender3DObjectMesh* mesh, const char* materialName, int* vertexIdxBuf )
{
	int maxVerticies = mesh->mMeshData->mAviableVertexCount, maxPoly = mesh->mMeshData->mMaxPolygonsCount;

	memset(vertexIdxBuf, -1, sizeof(int)*mMainMeshVericies.size() + 32);
	int lastVertexId = 0;

	poly3* polygons = new poly3[maxPoly];
	vertexTexNorm* verticies = new vertexTexNorm[maxVerticies];

	int i = 0, polyCount = 0;
	for (PolyArr::iterator it = mMainMeshPolygons.begin() + polyStartIdx; it != mMainMeshPolygons.end() && i < maxPoly; ++it, i++)
	{
		poly3* poly = &polygons[i];
		poly3* srcPoly = &(*it);

		if (vertexIdxBuf[srcPoly->a] < 0) 
		{ 
			vertexIdxBuf[srcPoly->a] = lastVertexId; 
			verticies[lastVertexId++] = mMainMeshVericies[srcPoly->a]; 
			if (lastVertexId > 31990)
				break;
		}

		if (vertexIdxBuf[srcPoly->b] < 0) 
		{ 
			vertexIdxBuf[srcPoly->b] = lastVertexId; 
			verticies[lastVertexId++] = mMainMeshVericies[srcPoly->b]; 
			if (lastVertexId > 31990)
				break;
		}

		if (vertexIdxBuf[srcPoly->c] < 0) 
		{ 
			vertexIdxBuf[srcPoly->c] = lastVertexId; 
			verticies[lastVertexId++] = mMainMeshVericies[srcPoly->c]; 
			if (lastVertexId > 31990)
				break;
		}
		
		poly->a = vertexIdxBuf[srcPoly->a];
		poly->b = vertexIdxBuf[srcPoly->b];
		poly->c = vertexIdxBuf[srcPoly->c];
		polyCount++;
	}

	mesh->addPart(verticies, lastVertexId, (int*)polygons, polyCount, 
		          mesh->mRenderObjectsManager->mRender->mSurfaceMaterials->getSurfaceMaterial(materialName));

	delete[] verticies;
	delete[] polygons;

	return polyStartIdx + polyCount;
}
