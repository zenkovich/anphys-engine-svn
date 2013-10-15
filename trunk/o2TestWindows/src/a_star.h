#ifndef A_STAR_H
#define A_STAR_H

#include <vector>

#include "public.h"

#include "util/math/vector2.h"

OPEN_O2_NAMESPACE

/** Сеть возможных путей. */
struct cWaypointWeb
{
	/** Соединение двух точек пути. */
	struct link
	{
		typedef std::vector<link> LinksVec;

		int mLinkWPId; /**< Индекс соединяемой точки. */
		float G;       /**< Оценка эффективности соединения. */

		/** констр. */
		link(int linkId, float g):mLinkWPId(linkId), G(g) {}
	};
	typedef std::vector<link> LinksVec;

	/** Точка пути. СОдержит позицию и список соединающихся соседей. */
	struct waypoint
	{
		vec2f    mPoint; /**< Координаты. */
		LinksVec mLinks; /**< Соседи. */

		/** констр. */
		waypoint(const vec2f& point):mPoint(point) {}
	};
	typedef std::vector<waypoint> WaypointsVec;

	WaypointsVec mWaypoints; /**< Список точек движения. */

	/** Возвращает индекс ближайшей точки пути к указанной точке. */
	int getNearestWaypoint(const vec2f& point) const;
};

/** Точка пути. */
struct cWaypoint
{
	vec2f mPoint; /**< Координаты. */
	float mG;     /**< Оценка сложности достижения точки. */

	/** констр. */
	cWaypoint(const vec2f& point, float g):mPoint(point), mG(g) {}
};
typedef std::vector<cWaypoint> WayPointsVec;

/** Ищет путь от точки beginIdx до endIdx, алгоритм - A*. */
bool astarSearchPath(cWaypointWeb& web, WayPointsVec& path, int beginIdx, int endIdx);

CLOSE_O2_NAMESPACE

#endif //A_STAR_H