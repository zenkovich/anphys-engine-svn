#ifndef CD_STUFF_H
#define CD_STUFF_H

#include "../util/utils.h"

struct phCollision;

struct phCollisionGeometryElement
{
	enum ElementType { ET_VERTEX = 0, ET_EDGE, ET_POLYGON, ET_ELLIPSE };
	
	unsigned int       mIndexParam;

	phCollisionGeometryElement():mIndexParam(0) {}

	virtual void checkIntersection(phCollisionGeometryElement* object, phCollision* collision, unsigned int indexParam) {}
	virtual float project(const vec3& axis, const vec3& origin, unsigned int indexParam) { return 0.0f; }
	virtual ElementType getType () { return ET_VERTEX; }
};

struct phCollisionVertex:public phCollisionGeometryElement
{
	phCollisionVertex();
	phCollisionVertex(const vec3& vertex);

	vec3         mVertex;

	void checkIntersection(phCollisionGeometryElement* object, phCollision* collision, unsigned int indexParam);
	float project(const vec3& axis, const vec3& origin, unsigned int indexParam);
	ElementType getType () { return ET_VERTEX; }
};

struct phCollisionEdge:public phCollisionGeometryElement
{
	phCollisionVertex* mFirstVertex;
	phCollisionVertex* mSecondVertex;
	vec3               mDirection;
	vec3               mNormalizedDirection;
	vec3               mDirectionNormal;
	float              mLength;

	phCollisionEdge();
	phCollisionEdge(phCollisionVertex* first, phCollisionVertex* second);

	void checkIntersection(phCollisionGeometryElement* object, phCollision* collision, unsigned int indexParam);
	float project(const vec3& axis, const vec3& origin, unsigned int indexParam);
	inline void calculateParametres();
	ElementType getType () { return ET_EDGE; }
};

struct phCollisionPolygon:public phCollisionGeometryElement
{
	phCollisionEdge* mEdges[4];
	bool             mEdgeInvertion[4];
	short            mEdgesCount;
	vec3             mNormal;

	phCollisionPolygon();
	phCollisionPolygon(phCollisionEdge* a, phCollisionEdge* b, phCollisionEdge* c);
	phCollisionPolygon(phCollisionEdge* a, phCollisionEdge* b, phCollisionEdge* c, phCollisionEdge* d);

	void checkIntersection(phCollisionGeometryElement* object, phCollision* collision, unsigned int indexParam);
	float project(const vec3& axis, const vec3& origin, unsigned int indexParam);
	inline void calculateParametres();
	void calculateInvertions();
	ElementType getType () { return ET_POLYGON; }
};

void isIntersect(phCollisionVertex* cvertex, phCollisionPolygon* cpolygon, phCollision* collision, unsigned int indexParam);
void isIntersect(phCollisionPolygon* polygonA, phCollisionPolygon* polygonB, phCollision* collision, unsigned int indexParam);
void isIntersect(phCollisionEdge* cedge, phCollisionPolygon* cpolygon, phCollision* collision, unsigned int indexParam);
void isIntersect(phCollisionEdge* edgeA, phCollisionEdge* edgeB, phCollision* collision, unsigned int indexParam);

#endif //CD_STUFF_H