#include "ray.h"
#include "mmath.h"

bool IntersectLinePolygon(vec3 pa, vec3 pb, vec3 pc, vec3 la, vec3 lb, vec3 *pt, vec3 norm)
{
	
	vec3 orig = la;

	float ln = (lb - la).len();
	vec3 dir = (lb - la)/ln;

	if (norm*dir < 0) dir *= -1.0f;

	vec3 v0 = pa, 
		 v1 = pb, 
		 v2 = pc;

    vec3 edge1 = v1-v0;
    vec3 edge2 = v2-v0;

    vec3 pvec = dir^edge2;

    float det = edge1*pvec;
    if (absf(det) < MDif)
        return false;

    vec3 tvec = orig - v0;

    vec3 qvec = tvec^edge1;
    float t = (edge2*qvec)/det;

	
	if (t < 0 || t > ln) return false;
	*pt = la + dir*t;

    float u = tvec*pvec;
	if (u < 0 || u > det)
	{
		return false;
	}

    float v = dir*qvec;
    if (v < 0 || u + v > det)
	{
        return false;
	}

	return true;
}

bool IntersectRayPolygon(vec3 pa, vec3 pb, vec3 pc, vec3 la, vec3 lb, vec3 *pt)
{	
	vec3 orig = la;

	float ln = (lb - la).len();
	vec3 dir = (lb - la)/ln;

	vec3 v0 = pa, 
		 v1 = pb, 
		 v2 = pc;

    vec3 edge1 = v1 - v0;
    vec3 edge2 = v2 - v0;

    vec3 pvec;
    pvec = dir ^ edge2;

    float det = edge1*pvec;
    if (absf(det) < MDif)
        return false;

    vec3 tvec = orig - v0;

    vec3 qvec = tvec ^ edge1;
    float t = (edge2*qvec)/det;

	
	*pt = la + dir*t;

    float u = tvec*pvec;
	if (u < 0 || u > det)
	{
		return false;
	}

    float v = dir*qvec;
    if (v < 0 || u + v > det)
	{
        return false;
	}

	return true;
}
