#include "a_star.h"

OPEN_O2_NAMESPACE
	
bool astarSearchPath( cWaypointWeb& web, WayPointsVec& path, int beginIdx, int endIdx )
{
	path.clear();
	path.push_back(cWaypoint(web.mWaypoints[beginIdx].mPoint, 0.0f));
	path.push_back(cWaypoint(web.mWaypoints[10].mPoint, 1.0f));
	path.push_back(cWaypoint(web.mWaypoints[endIdx].mPoint, 1.0f));
}

CLOSE_O2_NAMESPACE