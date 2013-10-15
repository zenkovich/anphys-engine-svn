#include "a_star.h"

#include <float.h>

#include "util/math/math.h"

OPEN_O2_NAMESPACE
	
struct wpoint
{
	vec2f mPosition;
	int   mParentIdx, mCurrentIdx;
	float f, g, h;
	wpoint(const vec2f& vp, int cur, int par, float vg, float vh):mPosition(vp), mParentIdx(par), mCurrentIdx(cur),
		g(vg), h(vh) { f = g + h; }
};
typedef std::vector<wpoint> wpointsVec;

bool astarSearchPath( cWaypointWeb& web, WayPointsVec& path, int beginIdx, int endIdx )
{
	//�������� ������ ���� 
	path.clear();

	wpointsVec open, close; //�������� � �������� ������

	//��������� ��������� ����� � ��������
	open.push_back(wpoint(web.mWaypoints[beginIdx].mPoint, beginIdx, beginIdx, 0, 0));

	bool pathFinded = false;

	//������� ���� ������
	while (open.size() > 0 && !pathFinded)
	{
		//���������� ����� ����� � �������� ������ � ���������� f
		int currentPoint = -1;
		float minf = FLT_MAX;
		int i = 0;
		for (wpointsVec::iterator it = open.begin(); it != open.end(); ++it, i++)
		{
			if (it->f < minf)
			{
				minf = it->f;
				currentPoint = i;
			}
		}

		//������� �� � �������� ������, ������� �� ���������
		close.push_back(open[currentPoint]);
		open.erase(open.begin() + currentPoint);
	
		//�������� �� �������� ������
		wpoint* curwp = &close.back();
		for (cWaypointWeb::LinksVec::iterator it = web.mWaypoints[curwp->mCurrentIdx].mLinks.begin();
			 it != web.mWaypoints[curwp->mCurrentIdx].mLinks.end(); it++)
		{
			//�������� ��, ������� ��������� � �������� ������. */
			bool skip = false;
			for (wpointsVec::iterator jt = close.begin(); jt != close.end(); ++jt)
			{
				if (jt->mCurrentIdx == it->mLinkWPId)
				{
					skip = true;
					break;
				}
			}

			if (skip)
				continue;
		
			//���� ����� ��� ��������� � �������� ������, ��������� ���������� ������ ������������
			for (wpointsVec::iterator jt = open.begin(); jt != open.end(); ++jt)
			{
				if (jt->mCurrentIdx == it->mLinkWPId)
				{
					if (it->G < jt->g)
					{
						jt->g = it->G;
						jt->mParentIdx = curwp->mCurrentIdx;
						skip = true;
						break;
					}
				}
			}

			if (skip)
				continue;

			//����� �������� ����� � �������� ������
			vec2f diff = web.mWaypoints[it->mLinkWPId].mPoint - web.mWaypoints[endIdx].mPoint;
			float h = abs(diff.x) + abs(diff.y);
			open.push_back(wpoint(web.mWaypoints[it->mLinkWPId].mPoint, it->mLinkWPId, curwp->mCurrentIdx, it->G, h));

			//���� ����� �������� �����, ������������� �����
			if (it->mLinkWPId == endIdx)
			{
				close.push_back(open.back());
				pathFinded = true;
				break;
			}
		}
	}

	if (pathFinded)
	{
		//��������������� ����
		int currid = open.back().mCurrentIdx;
		while (true)
		{
			wpoint* curr = NULL;
			for (wpointsVec::iterator jt = close.begin(); jt != close.end(); ++jt)
			{
				if (jt->mCurrentIdx == currid)
				{
					curr = &(*jt);
					break;
				}
			}

			if (!curr)
				return false;

			path.push_back(cWaypoint(curr->mPosition, curr->g));

			if (currid == beginIdx)
				break;

			currid = curr->mParentIdx;
		}

		//�������������� � ������ ��� �������
		for (int i = 0; i < (int)path.size()/2; i++)
		{
			tswap<cWaypoint>(path[i], path[path.size() - 1 - i]);
		}
	}

	return pathFinded;
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