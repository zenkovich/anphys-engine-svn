#ifndef MESH_MERGE_H
#define MESH_MERGE_H

#include "engine/engine_incl.h"

void mergeMeshes(vertexTexNorm* aVerticies, int aVerticiesCount, poly3* aPolygons, int aPolygonsCount,
	             vertexTexNorm* bVerticies, int bVerticiesCount, poly3* bPolygons, int bPolygonsCount,
				 int* resVerticiesCount, int* resPolygonsCount);

#endif //MESH_MERGE_H