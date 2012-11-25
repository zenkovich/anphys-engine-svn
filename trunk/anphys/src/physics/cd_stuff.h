#ifndef CD_STUFF_H
#define CD_STUFF_H

#include "../util/utils.h"

struct phCollision;

struct phCollisionGeometryElement
{
	enum ElementType { ET_VERTEX = 0, ET_EDGE, ET_POLYGON, ET_ELLIPSE };

	virtual void checkIntersection(phCollisionGeometryElement* object, phCollision* collision) {}
	virtual float project(const vec3& axis, unsigned int indexParam) { return 0.0f; }
	virtual ElementType getType () { return ET_VERTEX; }
};

struct phCollisionVertex
{
	phCollisionVertex():mIndexParam(0) {}
	phCollisionVertex(const vec3& vertex):mVertex(vertex), mIndexParam(0) {}

	vec3         mVertex;
	unsigned int mIndexParam;

	void checkIntersection(phCollisionGeometryElement* object, phCollision* collision);
	float project(const vec3& axis, unsigned int indexParam);
};

struct phCollisionEdge
{
	phCollisionVertex* mFirstVertex;
	phCollisionVertex* mSecondVertex;
	vec3               mDirection;
	vec3               mNormalizedDirection;
	float              mLength;

	phCollisionEdge():mFirstVertex(NULL), mSecondVertex(NULL) {}
	phCollisionEdge(phCollisionVertex* first, phCollisionVertex* second):mFirstVertex(first), mSecondVertex(second)
	{
		mDirection = (mSecondVertex->mVertex - mFirstVertex->mVertex);
		mLength = mDirection.len();
		mNormalizedDirection = mDirection/mLength;
	}

	void checkIntersection(phCollisionGeometryElement* object, phCollision* collision);
	float project(const vec3& axis, unsigned int indexParam);
};

struct phCollisionPolygon:public phCollisionGeometryElement
{
	phCollisionEdge* mEdges[4];
	int              mEdgesCount;
	vec3             mNormal;

	phCollisionPolygon();
	phCollisionPolygon(phCollisionEdge* a, phCollisionEdge* b, phCollisionEdge* c);
	phCollisionPolygon(phCollisionEdge* a, phCollisionEdge* b, phCollisionEdge* c, phCollisionEdge* d);

	void checkIntersection(phCollisionGeometryElement* object, phCollision* collision);
	float project(const vec3& axis, unsigned int indexParam);
};

void isIntersect(phCollisionVertex* cvertex, phCollisionPolygon* cpolygon);
void isIntersect(phCollisionPolygon* polygonA, phCollisionPolygon* polygonB);
void isIntersect(phCollisionEdge* cedge, phCollisionPolygon* cpolygon);

#endif //CD_STUFF_H