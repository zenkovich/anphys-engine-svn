#include "mesh_merge.h"


void mergeMeshes( vertexTexNorm* aVerticies, int aVerticiesCount, poly3* aPolygons, int aPolygonsCount, 
	              vertexTexNorm* bVerticies, int bVerticiesCount, poly3* bPolygons, int bPolygonsCount, 
				  int* resVerticiesCount, int* resPolygonsCount )
{
	*resVerticiesCount = aVerticiesCount;
	*resPolygonsCount = aPolygonsCount;
}
