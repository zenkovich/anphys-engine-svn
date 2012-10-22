#ifndef AABB_H
#define AABB_H

#include "../utils.h"

struct AABB
{
	vec3 mMin, mMax;

	AABB():mMin(0), mMax(0) {}
	AABB(const vec3& minp, const vec3& maxp):mMin(minp), mMax(maxp) {}

	inline bool isIntersect(const AABB& aabb)
	{
		return !(mMin.x > aabb.mMax.x || mMin.y > aabb.mMax.y || mMin.z > aabb.mMax.z ||
			     mMax.x < aabb.mMin.x || mMax.y < aabb.mMin.y || mMax.z < aabb.mMin.z);
	}

	inline bool isIntersect(const vec3& point)
	{
		return !(point.x < mMin.x || point.y < mMin.y || point.z < mMin.z ||
			     point.x > mMax.x || point.y > mMax.y || point.z > mMax.z);
	}

	inline void computeFromPoints(vec3* points, int count)
	{
		mMin = mMax = points[0];
		for (int i = 1; i < count; i++)
		{
			if (points[i].x < mMin.x) mMin.x = points[i].x;
			if (points[i].y < mMin.y) mMin.y = points[i].y;
			if (points[i].z < mMin.z) mMin.z = points[i].z;
			
			if (points[i].x > mMax.x) mMax.x = points[i].x;
			if (points[i].y > mMax.y) mMax.y = points[i].y;
			if (points[i].z > mMax.z) mMax.z = points[i].z;
		}
	}
};

#endif //AABB_H