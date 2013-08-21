#ifndef MESH_MERGE_H
#define MESH_MERGE_H

#include "engine/engine_incl.h"

void mergeMeshes(vec3* aVerticies, int aVerticiesCount, poly3* aPolygons, int aPolygonsCount,
	             vec3* bVerticies, int bVerticiesCount, poly3* bPolygons, int bPolygonsCount,
				 int* resVerticiesCount, int* resPolygonsCount);

#endif //MESH_MERGE_H