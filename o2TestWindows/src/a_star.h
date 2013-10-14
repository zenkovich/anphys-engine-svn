#ifndef A_STAR_H
#define A_STAR_H

#include <vector>

#include "public.h"

#include "util/math/vector2.h"

OPEN_O2_NAMESPACE

struct cWaypointWeb
{
	struct link
	{
		typedef std::vector<link> LinksVec;

		int mLinkWPId;;
		float F, G, H;

		link(int linkId, float g):mLinkWPId(linkId), G(g), F(0), H(0) {}
	};
	typedef std::vector<link> LinksVec;

	struct waypoint
	{
		vec2f    mPoint;
		LinksVec mLinks;

		waypoint(const vec2f& point):mPoint(point) {}
	};
	typedef std::vector<waypoint> WaypointsVec;

	WaypointsVec mWaypoints;

	int getNearestWaypoint(const vec2f& point) const;
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