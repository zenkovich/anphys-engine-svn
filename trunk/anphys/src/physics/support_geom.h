#ifndef SUPPORT_GEOM_H
#define SUPPORT_GEOM_H

#include "../util/utils.h"

void findNearPoints(vec3* points, int pointsCount, vec3* result, int &resCount, vec3 dir, vec3 orig, 
	                float minProjection, float maxProjection);

#endif //SUPPORT_GEOM_H