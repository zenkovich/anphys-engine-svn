#include "support_geom.h"

void findNearPoints( vec3* points, int pointsCount, vec3* result, int &resCount, vec3 dir, vec3 orig, 
	                float minProjection, float maxProjection )
{
	resCount = 0;
	for (int i = 0; i < pointsCount; i++)
	{
		float project = (points[i] - orig)*dir;

		*gLog << formatStr("point [%i] ( %.2f %.2f %.2f ) proj %.3f (%.3f...%.3f)\n", i,
			points[i].x, points[i].y, points[i].z, project, minProjection, maxProjection);

		if (project < minProjection || project > maxProjection) continue;
		result[resCount++] = points[i];
	}
}
