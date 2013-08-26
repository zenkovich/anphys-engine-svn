#include "mesh_merge.h"

#include <vector>

#include "util/debug/render_stuff.h"

struct tpolygon;
struct tvertex;
struct tedge;

typedef std::vector<tpolygon*> ppolygonsVec;
typedef std::vector<tvertex*> pvertexVec;
typedef std::vector<tedge*> pedgesVec;

struct tvertex
{
	vec3 point;
	vec3 normal;
	vec2 tex;

	tvertex() {}
};

struct tpolygon
{
	int a, b, c;
	tvertex *pa, *pb, *pc;
	tedge *ea, *eb, *ec; 

	AABB aabb;

	tpolygon() {}
};

struct tedge
{
	int a, b;
	vertex *va, *vb;
	polygon *pa, *pb;

	tedge() {}
};

struct tmesh
{
	struct cluster
	{
		ppolygonsVec plg;
		AABB aabb;
	};
	typedef std::vector<cluster> clusterVec;

	tvertex* vrt;
	tpolygon* plg;
	clusterVec clusters;
	float clusterSize;
	int vcount, pcount;

	void init(vertexTexNorm* svrt, int vrtCount, int maxVrtCount, poly3* splg, int plgCount, int maxPlgCount)
	{
		vrt = new tvertex[maxVrtCount];
		plg = new tpolygon[maxPlgCount];
		vcount = vrtCount; pcount = plgCount;

		vec3 pmin(svrt[0].x, svrt[0].y, svrt[0].z);
		vec3 pmax = pmin;

		for (int i = 0; i < vrtCount; i++)
		{
			memcpy(&vrt[i], &svrt[i], sizeof(tvertex));
			pmin.x = fmin(svrt[i].x, pmin.x); pmin.y = fmin(svrt[i].y, pmin.y); pmin.z = fmin(svrt[i].z, pmin.z);
			pmax.x = fmax(svrt[i].x, pmax.x); pmax.y = fmax(svrt[i].y, pmax.y); pmax.z = fmax(svrt[i].z, pmax.z);
		}

		int clusterVrtCount = 200;
		int clustersCount = sqrt((float)(vrtCount/clusterVrtCount)) + 1; 
		
		float xClusterSize = (pmax.x - pmin.x)/(float)clustersCount;
		float zClusterSize = (pmax.z - pmin.z)/(float)clustersCount;
		for (int i = 0; i < clustersCount; i++)
		{
			for (int j = 0; j < clustersCount; j++)
			{
				clusters.push_back(cluster());
				clusters.back().aabb = AABB(vec3(pmin.x + (float)i*xClusterSize, pmin.y,       pmin.z + (float)j*zClusterSize),
					                        vec3(pmin.x + (float)(i + 1)*xClusterSize, pmax.y, pmin.z + (float)(j + 1)*zClusterSize));
			}
		}

		for (int i = 0; i < plgCount; i++)
		{
			tpolygon* tplg = &plg[i];
			poly3* tsplg = &splg[i];

			tplg->a = tsplg->a; tplg->b = tsplg->b; tplg->c = tsplg->c;
			tplg->pa = &vrt[tplg->a]; tplg->pb = &vrt[tplg->b]; tplg->pc = &vrt[tplg->c];
			vec3 aabbPts[] = { tplg->pa->point, tplg->pb->point, tplg->pc->point };
			tplg->aabb.computeFromPoints(aabbPts, 3);

			for (clusterVec::iterator it = clusters.begin(); it != clusters.end(); ++it)
			{
				if (it->aabb.isIntersect(tplg->aabb))
				{
					it->plg.push_back(tplg);
				}
			}
		}
	}
};

typedef std::vector<tpolygon> polygonsVec;
typedef std::vector<tvertex> vertexVec;
typedef std::vector<tedge> edgesVec;

void mergeMeshes( vertexTexNorm* aVerticies, int aVerticiesCount, poly3* aPolygons, int aPolygonsCount, 
	              vertexTexNorm* bVerticies, int bVerticiesCount, poly3* bPolygons, int bPolygonsCount, 
				  int* resVerticiesCount, int* resPolygonsCount )
{
	getRenderStuff().reset();

	tmesh amesh;
	amesh.init(aVerticies, aVerticiesCount, aVerticiesCount + bVerticiesCount*2,
		       aPolygons, aPolygonsCount, aPolygonsCount + bPolygonsCount*2);
	
	tmesh bmesh;
	bmesh.init(bVerticies, bVerticiesCount, bVerticiesCount, bPolygons, bPolygonsCount, bPolygonsCount);

	for (tmesh::clusterVec::iterator it = amesh.clusters.begin(); it != amesh.clusters.end(); ++it)
	{
		tmesh::cluster* acluster = &(*it);

		for (tmesh::clusterVec::iterator jt = bmesh.clusters.begin(); jt != bmesh.clusters.end(); ++jt)
		{
			tmesh::cluster* bcluster = &(*jt);

			if (!acluster->aabb.isIntersect(bcluster->aabb))
				continue;

			for (ppolygonsVec::iterator kt = acluster->plg.begin(); kt != acluster->plg.end(); ++kt)
			{
				tpolygon* apoly = *kt;
				if (!apoly->aabb.isIntersect(bcluster->aabb))
					continue;

				for (ppolygonsVec::iterator lt = bcluster->plg.begin(); lt != bcluster->plg.end(); ++lt)
				{
					tpolygon* bpoly = *lt;

					if (apoly->aabb.isIntersect(bpoly->aabb))
					{
						getRenderStuff().addBlueArrow(apoly->pa->point, apoly->pb->point);
						getRenderStuff().addBlueArrow(apoly->pb->point, apoly->pc->point);
						getRenderStuff().addBlueArrow(apoly->pc->point, apoly->pa->point);
						
						getRenderStuff().addRedArrow(bpoly->pa->point, bpoly->pb->point);
						getRenderStuff().addRedArrow(bpoly->pb->point, bpoly->pc->point);
						getRenderStuff().addRedArrow(bpoly->pc->point, bpoly->pa->point);
					}
				}
			}
		}
	}

	*resVerticiesCount = aVerticiesCount;
	*resPolygonsCount = aPolygonsCount;
}
