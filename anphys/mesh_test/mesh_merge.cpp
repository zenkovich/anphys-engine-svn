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

struct tcontour
{
	struct line
	{
		tvertex a, b;

		line(const tvertex& va, const tvertex& vb):a(va), b(vb) {}
	};
	typedef std::vector<line> LinesVec;
	typedef std::vector<LinesVec> ContourVec;

	LinesVec lines;
	ContourVec contours;

	void getContours();
};

template<typename T>
void tswap(T& a, T& b) { T buf = a; a = b; b = buf; }

void tcontour::getContours()
{
	do 
	{		
		contours.push_back(LinesVec());
		contours.back().push_back(lines.back());
		lines.pop_back();
		
		vec3 begin = contours.back()[0].a.point;
		vec3 curr = contours.back()[0].b.point;

		float pr = 0.01f;

		bool cnt = false;
		do 
		{
			bool fnd = false;
			for (LinesVec::iterator it = lines.begin(); it != lines.end() && !fnd;)
			{
				float d1 = len(curr, it->a.point);
				float d2 = len(curr, it->b.point);
				if (d1 < pr || d2 < pr)
				{
					if (d2 < pr)
					{
						tswap(it->a, it->b);
					}

					curr = it->b.point;
					contours.back().push_back(*it);
					it = lines.erase(it);
					fnd = true;
					//break;
				}
				else ++it;
			}

			cnt = (fnd && len(curr, begin) >= pr);
		}
		while(cnt);
	}
	while (!lines.empty());
}

struct tpolygon
{
	int a, b, c;
	tvertex *pa, *pb, *pc;
	tedge *ea, *eb, *ec; 
	vec3 n;

	AABB aabb;

	ppolygonsVec cplg;

	tcontour contour;
	pedgesVec ue;

	tpolygon() {  }
	void finCont();
};

struct tedge
{
	int a, b;
	tvertex *va, *vb;
	tpolygon *pa, *pb;
	float len;

	tedge() {}
};

void tpolygon::finCont()
{	
	tedge* ee[3] = { ea, eb, ec };
	for (int i = 0; i < 3; i++)
	{
		pedgesVec::iterator fnd = std::find(ue.begin(), ue.end(), ee[i]);
		if (fnd != ue.end())
			continue;

		contour.lines.push_back(tcontour::line(*ee[i]->va, *ee[i]->vb));
	}

	contour.getContours();
}

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

		eMaxCount = plgCount*2 + 10000;
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
			tedge** edgePts[3] = { &poly->ea, &poly->eb, &poly->ec };

			for (int j = 0; j < 3; j++)
			{
				tedge* edge = NULL;
				for (int k = 0; k < ecount; k++)
				{
					tedge* eddg = &edg[k];
					if ( (eddg->a == eidx[j][0] && eddg->b == eidx[j][1]) || (eddg->a == eidx[j][1] && eddg->b == eidx[j][0]) )
					{
						edge = eddg;
						break;
					}
				}

				if (!edge)
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

					nedge->len = (nedge->va->point - nedge->vb->point).len();

					if (ecount > eMaxCount)
					{
						ecount = ecount;
					}
					edge = nedge;

					float lenn = (nedge->va->point - nedge->vb->point).len();
					if (lenn < 0.2f)
						lenn = lenn;
				}

				*edgePts[j] = edge;
			}
		}

		/*for (int i = 0; i < pcount; i++)
		{
			tpolygon* poly = &plg[i];

			vec3 c = (poly->pa->point + poly->pb->point + poly->pc->point)/3.0f;
			getRenderStuff().addGreenArrow(c, (poly->ea->va->point + poly->ea->vb->point)*0.5f);
			getRenderStuff().addGreenArrow(c, (poly->eb->va->point + poly->eb->vb->point)*0.5f);
			getRenderStuff().addGreenArrow(c, (poly->ec->va->point + poly->ec->vb->point)*0.5f);
			
			getRenderStuff().addBlueArrow(poly->ea->va->point, poly->ea->vb->point);
			getRenderStuff().addBlueArrow(poly->eb->va->point, poly->eb->vb->point);
			getRenderStuff().addBlueArrow(poly->ec->va->point, poly->ec->vb->point);
		}*/
	}
};

void intersectPolygons(tpolygon& a, tpolygon& b)
{
	vec3 pt;
	int ic = 0;
	tvertex va, vb;
	tvertex x;
	float t;

	if (IntersectLinePolygon(a.pa->point, a.pb->point, a.pc->point, a.n, b.ea->va->point, b.ea->vb->point, &pt, &t))
	{
		va.point = pt;
		getRenderStuff().addRedCube(pt);

		x.point = pt;
		tvertex ff = ( (fabs(t) < b.ea->len*0.5f) ? (*b.ea->va):(*b.ea->vb) );
		b.contour.lines.push_back(tcontour::line(x, ff));
		b.ue.push_back(b.ea);

		//getRenderStuff().addRedArrow((b.ea->va->point + b.ea->vb->point)*0.5f, pt);
		ic++;
	}
	if (IntersectLinePolygon(a.pa->point, a.pb->point, a.pc->point, a.n, b.eb->va->point, b.eb->vb->point, &pt, &t))
	{
		if (ic == 0)
		{
			va.point = pt;
		}
		else
		{
			vb.point = pt;
		}

		x.point = pt;
		tvertex ff = ( (fabs(t) < b.eb->len*0.5f) ? (*b.eb->va):(*b.eb->vb) );
		b.contour.lines.push_back(tcontour::line(x, ff));
		b.ue.push_back(b.eb);

		//getRenderStuff().addRedArrow((b.eb->va->point + b.eb->vb->point)*0.5f, pt);
		getRenderStuff().addRedCube(pt);
		ic++;
	}
	if (IntersectLinePolygon(a.pa->point, a.pb->point, a.pc->point, a.n, b.ec->va->point, b.ec->vb->point, &pt, &t))
	{
		if (ic == 0)
		{
			va.point = pt;
		}
		else
		{
			vb.point = pt;
		}

		x.point = pt;
		tvertex ff = ( (fabs(t) < b.ec->len*0.5f) ? (*b.ec->va):(*b.ec->vb) );
		b.contour.lines.push_back(tcontour::line(x, ff));
		b.ue.push_back(b.ec);

		//getRenderStuff().addRedArrow((b.ec->va->point + b.ec->vb->point)*0.5f, pt);
		getRenderStuff().addRedCube(pt);
		ic++;
	}
	
	if (IntersectLinePolygon(b.pa->point, b.pb->point, b.pc->point, b.n, a.ea->va->point, a.ea->vb->point, &pt, &t))
	{
		if (ic == 0)
		{
			va.point = pt;
		}
		else
		{
			vb.point = pt;
		}

		x.point = pt;
		tvertex ff = ( (fabs(t) < a.ea->len*0.5f) ? (*a.ea->va):(*a.ea->vb) );
		a.contour.lines.push_back(tcontour::line(x, ff));
		a.ue.push_back(a.ea);

		//getRenderStuff().addRedArrow((a.ea->va->point + a.ea->vb->point)*0.5f, pt);
		getRenderStuff().addRedCube(pt);
		ic++;
	}
	if (IntersectLinePolygon(b.pa->point, b.pb->point, b.pc->point, b.n, a.eb->va->point, a.eb->vb->point, &pt, &t))
	{
		if (ic == 0)
		{
			va.point = pt;
		}
		else
		{
			vb.point = pt;
		}

		x.point = pt;
		tvertex ff = ( (fabs(t) < a.eb->len*0.5f) ? (*a.eb->va):(*a.eb->vb) );
		a.contour.lines.push_back(tcontour::line(x, ff));
		a.ue.push_back(a.eb);

		//getRenderStuff().addRedArrow((a.eb->va->point + a.eb->vb->point)*0.5f, pt);
		getRenderStuff().addRedCube(pt);
		ic++;
	}
	if (IntersectLinePolygon(b.pa->point, b.pb->point, b.pc->point, b.n, a.ec->va->point, a.ec->vb->point, &pt, &t))
	{
		if (ic == 0)
		{
			va.point = pt;
		}
		else
		{
			vb.point = pt;
		}

		x.point = pt;
		tvertex ff = ( (fabs(t) < a.ec->len*0.5f) ? (*a.ec->va):(*a.ec->vb) );
		a.contour.lines.push_back(tcontour::line(x, ff));
		a.ue.push_back(a.ec);

		//getRenderStuff().addRedArrow((a.ec->va->point + a.ec->vb->point)*0.5f, pt);
		getRenderStuff().addRedCube(pt);
		ic++;
	}

	if (ic > 0)
	{
		a.contour.lines.push_back(tcontour::line(va, vb));
		b.contour.lines.push_back(tcontour::line(va, vb));
	}
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
							/*getRenderStuff().addBlueArrow(apoly->pa->point, apoly->pb->point);
							getRenderStuff().addBlueArrow(apoly->pb->point, apoly->pc->point);
							getRenderStuff().addBlueArrow(apoly->pc->point, apoly->pa->point);

							vec3 c = (apoly->pa->point + apoly->pb->point + apoly->pc->point)/3.0f;
							//getRenderStuff().addBlueArrow(c, c + apoly->n*0.1f);
						
							c = (bpoly->pa->point + bpoly->pb->point + bpoly->pc->point)/3.0f;
							getRenderStuff().addRedArrow(bpoly->pa->point, bpoly->pb->point);
							getRenderStuff().addRedArrow(bpoly->pb->point, bpoly->pc->point);
							getRenderStuff().addRedArrow(bpoly->pc->point, bpoly->pa->point);*/
							//getRenderStuff().addRedArrow(c, c + bpoly->n*0.1f);

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

	for (int i = 0; i < amesh.pcount; i++)
	{
		int cid = 0;
		tpolygon* poly = &amesh.plg[i];
		if (!poly->contour.lines.empty())
		{
			poly->finCont();
			for (tcontour::ContourVec::iterator it = poly->contour.contours.begin(); it != poly->contour.contours.end(); ++it)
			{
				for (tcontour::LinesVec::iterator jt = it->begin(); jt != it->end(); ++jt)
				{
					if (cid%2 == 0)
					{
						getRenderStuff().addGreenArrow(jt->a.point, jt->b.point);
						getRenderStuff().addGreenArrow(jt->a.point, it->begin()->b.point);
					}
					else
					{
						getRenderStuff().addBlueArrow(jt->a.point, jt->b.point);
						getRenderStuff().addBlueArrow(jt->a.point, it->begin()->b.point);
					}
				}

				cid++;
			}

			/*for (tcontour::LinesVec::iterator it = poly->contour.lines.begin(); it != poly->contour.lines.end(); ++it)
			{
				//getRenderStuff().addGreenArrow(it->a.point, it->b.point);
			}*/
		}
	}

	*resVerticiesCount = aVerticiesCount;
	*resPolygonsCount = aPolygonsCount;
}
