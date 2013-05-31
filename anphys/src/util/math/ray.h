#ifndef RAY_H
#define RAY_H
#include "vector.h"

#define MDif 0.00000001f

struct Cray
{
	vec3 s, n;

	inline Cray() { s = vec3(0, 0, 0); s = vec3(1, 0, 0); }
	inline Cray(vec3 start, vec3 ve) { s = start; n = ve; }
	inline void Set(vec3 start, vec3 ve) { s = start; n = ve; }
};

bool IntersectLinePolygon(const vec3& pa, const vec3& pb, const vec3& pc, const vec3& norm, const vec3& la, const vec3& lb,
	                      vec3 *pt, float* tt = 0);
bool IntersectRayPolygon(vec3 pa, vec3 pb, vec3 pc, vec3 la, vec3 lb, vec3 *pt);


#endif
