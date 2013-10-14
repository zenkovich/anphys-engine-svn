#include "a_star.h"

#include <float.h>

OPEN_O2_NAMESPACE
	
bool astarSearchPath( cWaypointWeb& web, WayPointsVec& path, int beginIdx, int endIdx )
{
	path.clear();
	path.push_back(cWaypoint(web.mWaypoints[beginIdx].mPoint, 0.0f));
	path.push_back(cWaypoint(web.mWaypoints[10].mPoint, 1.0f));
	path.push_back(cWaypoint(web.mWaypoints[endIdx].mPoint, 1.0f));

	return true;
}


int cWaypointWeb::getNearestWaypoint( const vec2f& point ) const
{
	float minDist = FLT_MAX;
	int res = 0;
	int idx = 0;
	for (WaypointsVec::const_iterator it = mWaypoints.cbegin(); it != mWaypoints.cend(); ++it, idx++)
	{
		float dist = (it->mPoint - point).len();
		if (dist < minDist)
		{
			res = idx;
			minDist = dist;
		}
	}

	return res;
}

CLOSE_O2_NAMESPACE