#include "mesh_merge.h"


void mergeMeshes( vec3* aVerticies, int aVerticiesCount, poly3* aPolygons, int aPolygonsCount, 
	              vec3* bVerticies, int bVerticiesCount, poly3* bPolygons, int bPolygonsCount, 
				  int* resVerticiesCount, int* resPolygonsCount )
{
	*resVerticiesCount = aVerticiesCount;
	*resPolygonsCount = aPolygonsCount;
}
