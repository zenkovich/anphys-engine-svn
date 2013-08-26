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

	tpolygon() {}
};

struct tedge
{
	int a, b;
	vertex *va, *vb;
	polygon *pa, *pb;

	tedge() {}
};

typedef std::vector<tpolygon> polygonsVec;
typedef std::vector<tvertex> vertexVec;
typedef std::vector<tedge> edgesVec;

void mergeMeshes( vertexTexNorm* aVerticies, int aVerticiesCount, poly3* aPolygons, int aPolygonsCount, 
	              vertexTexNorm* bVerticies, int bVerticiesCount, poly3* bPolygons, int bPolygonsCount, 
				  int* resVerticiesCount, int* resPolygonsCount )
{
	getRenderStuff().reset();

	vertexVec verticies;
	polygonsVec polygons;
	edgesVec edges;

	verticies.reserve(aVerticiesCount);
	for (int i = 0; i < aVerticiesCount; i++)
	{
		verticies.push_back(tvertex());

		tvertex* vrt = &verticies.back();
		vertexTexNorm* svrt = &aVerticies[i];

		memcpy(vrt, svrt, sizeof(tvertex));
	}

	polygons.reserve(aPolygonsCount);
	for (int i = 0; i < aPolygonsCount; i++)
	{
		polygons.push_back(tpolygon());

		tpolygon* poly = &polygons.back();
		poly3* spoly = &aPolygons[i];
		
		poly->a = spoly->a;
		poly->b = spoly->b;
		poly->c = spoly->c;

		poly->pa = verticies[poly
	}

	*resVerticiesCount = aVerticiesCount;
	*resPolygonsCount = aPolygonsCount;
}
