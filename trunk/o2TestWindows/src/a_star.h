#ifndef A_STAR_H
#define A_STAR_H

#include <vector>

#include "public.h"

#include "util/math/vector2.h"

OPEN_O2_NAMESPACE

struct cWaypointWeb
{
	struct waypoint
	{
		vec2f mPoint;

		waypoint(const vec2f& point):mPoint(point) {}
	};
	typedef std::vector<waypoint> WaypointsVec;

	struct link
	{
		typedef std::vector<link> LinksVec;

		int mA, mB;
		float F, G, H;

		link(int a, int b, float g):mA(a), mB(b), G(g), F(0), H(0) {}
	};
	typedef std::vector<link> LinksVec;

	WaypointsVec mWaypoints;
	LinksVec     mLinks;
};

struct cWaypoint
{
	vec2f mPoint;
	float mG;

	cWaypoint(const vec2f& point, float g):mPoint(point), mG(g) {}
};
typedef std::vector<cWaypoint> WayPointsVec;

bool astarSearchPath(cWaypointWeb& web, WayPointsVec& path, int beginIdx, int endIdx);

CLOSE_O2_NAMESPACE

#endif //A_STAR_H