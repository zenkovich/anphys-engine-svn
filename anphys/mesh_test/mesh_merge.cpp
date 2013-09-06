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
	vec3 n;

	AABB aabb;

	ppolygonsVec cplg;

	tpolygon() {}
};

struct tedge
{
	int a, b;
	tvertex *va, *vb;
	tpolygon *pa, *pb;

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
	tedge* edg;
	clusterVec clusters;
	float clusterSize;
	int vcount, pcount, ecount, eMaxCount;

	~tmesh()
	{
		delete[] vrt;
		delete[] plg;
		delete[] edg;
	}

	void init(vertexTexNorm* svrt, int vrtCount, int maxVrtCount, poly3* splg, int plgCount, int maxPlgCount)
	{
		vrt = new tvertex[maxVrtCount];
		plg = new tpolygon[maxPlgCount];
		vcount = vrtCount; pcount = plgCount;

		eMaxCount = maxVrtCount/2;
		edg = new tedge[eMaxCount];
		ecount = 0;

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
			tplg->n = ((tplg->pb->point - tplg->pa->point)^(tplg->pc->point - tplg->pa->point)).normalize()*(-1.0f);

			for (clusterVec::iterator it = clusters.begin(); it != clusters.end(); ++it)
			{
				if (it->aabb.isIntersect(tplg->aabb))
				{
					it->plg.push_back(tplg);
				}
			}
		}

		for (clusterVec::iterator it = clusters.begin(); it != clusters.end(); ++it)
		{
			for (ppolygonsVec::iterator jt = it->plg.begin(); jt != it->plg.end(); ++jt)
			{
				it->aabb.mMin.x = fmin(it->aabb.mMin.x, (*jt)->aabb.mMin.x);
				it->aabb.mMin.y = fmin(it->aabb.mMin.y, (*jt)->aabb.mMin.y);
				it->aabb.mMin.z = fmin(it->aabb.mMin.z, (*jt)->aabb.mMin.z);
				
				it->aabb.mMax.x = fmax(it->aabb.mMax.x, (*jt)->aabb.mMax.x);
				it->aabb.mMax.y = fmax(it->aabb.mMax.y, (*jt)->aabb.mMax.y);
				it->aabb.mMax.z = fmax(it->aabb.mMax.z, (*jt)->aabb.mMax.z);
			}
		}

		for (int i = 0; i < pcount; i++)
		{
			tpolygon* poly = &plg[i];
			int eidx[3][2] = { { poly->a, poly->b }, { poly->b, poly->c }, { poly->c, poly->a } };

			for (int j = 0; j < 3; j++)
			{
				for (int k = 0; k < ecount; k++)
				{
					tedge* edge = &edg[k];
					if (!((edge->a == eidx[j][0] && edge->b == eidx[j][1]) || 
						  (edge->a == eidx[j][1] && edge->b == eidx[j][0])) && ecount < eMaxCount - 1)
					{
						tedge* nedge = &edg[ecount];
						nedge->a = eidx[j][0];
						nedge->b = eidx[j][1];
						nedge->va = &vrt[nedge->a];
						nedge->vb = &vrt[nedge->b];
						if (!nedge->pa)
							nedge->pa = poly;
						else
							nedge->pb = poly;

						ecount++;
					}
				}
			}
		}
	}
};

void intersectPolygons(tpolygon& a, tpolygon& b)
{
	vec3 pt;
	if (IntersectLinePolygon(a.pa->point, a.pb->point, a.pc->point, a.n, b.pa->point, b.pb->point, &pt))
		getRenderStuff().addRedCube(pt);
	if (IntersectLinePolygon(a.pa->point, a.pb->point, a.pc->point, a.n, b.pb->point, b.pc->point, &pt))
		getRenderStuff().addRedCube(pt);
	if (IntersectLinePolygon(a.pa->point, a.pb->point, a.pc->point, a.n, b.pc->point, b.pa->point, &pt))
		getRenderStuff().addRedCube(pt);
	
	if (IntersectLinePolygon(b.pa->point, b.pb->point, b.pc->point, b.n, a.pa->point, a.pb->point, &pt))
		getRenderStuff().addBlueCube(pt);
	if (IntersectLinePolygon(b.pa->point, b.pb->point, b.pc->point, b.n, a.pb->point, a.pc->point, &pt))
		getRenderStuff().addBlueCube(pt);
	if (IntersectLinePolygon(b.pa->point, b.pb->point, b.pc->point, b.n, a.pc->point, a.pa->point, &pt))
		getRenderStuff().addBlueCube(pt);
}

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
						ppolygonsVec::iterator fnd = std::find(apoly->cplg.begin(), apoly->cplg.end(), bpoly);
						if (fnd == apoly->cplg.end())
						{							
							getRenderStuff().addBlueArrow(apoly->pa->point, apoly->pb->point);
							getRenderStuff().addBlueArrow(apoly->pb->point, apoly->pc->point);
							getRenderStuff().addBlueArrow(apoly->pc->point, apoly->pa->point);

							vec3 c = (apoly->pa->point + apoly->pb->point + apoly->pc->point)/3.0f;
							getRenderStuff().addBlueArrow(c, c + apoly->n*0.1f);
						
							c = (bpoly->pa->point + bpoly->pb->point + bpoly->pc->point)/3.0f;
							getRenderStuff().addRedArrow(bpoly->pa->point, bpoly->pb->point);
							getRenderStuff().addRedArrow(bpoly->pb->point, bpoly->pc->point);
							getRenderStuff().addRedArrow(bpoly->pc->point, bpoly->pa->point);
							getRenderStuff().addRedArrow(c, c + bpoly->n*0.1f);

							intersectPolygons(*apoly, *bpoly);
							
							apoly->cplg.push_back(bpoly);
							bpoly->cplg.push_back(apoly);
						}
						else
						{
							fnd = fnd;
						}
					}
				}
			}
		}
	}

	*resVerticiesCount = aVerticiesCount;
	*resPolygonsCount = aPolygonsCount;
}
