#ifndef CD_STUFF_H
#define CD_STUFF_H

#include "../util/utils.h"

struct phCollision;

struct phCollisionGeometryElement
{
	enum ElementType { ET_VERTEX = 0, ET_EDGE, ET_POLYGON, ET_ELLIPSE };
	
	float        mProjection;

	phCollisionGeometryElement(): mProjection(0.0f) {}

	virtual float project(vec3& axis, vec3& origin) { return 0.0f; }
	virtual ElementType getType () { return ET_VERTEX; }
	virtual void showDbgGraphics() {}
	virtual void calculateParametres() {}
	virtual bool isOnProjectionInterval(float minProj, float maxProj) { return (!(mProjection < minProj || mProjection > maxProj)); }
};

inline bool phCollisionGeometryElementSortPred(phCollisionGeometryElement* a, phCollisionGeometryElement* b)
{
	return a->getType() > b->getType();
}

struct phCollisionVertex:public phCollisionGeometryElement
{
	phCollisionVertex();
	phCollisionVertex(const vec3& vertex);

	vec3         mVertex;

	void checkIntersection(phCollisionGeometryElement* object, phCollision* collision);
	float project(vec3& axis, vec3& origin);
	ElementType getType () { return ET_VERTEX; }
	void showDbgGraphics();
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

	void checkIntersection(phCollisionGeometryElement* object, phCollision* collision);
	float project(vec3& axis, vec3& origin);
	inline void calculateParametres();
	ElementType getType () { return ET_EDGE; }
	void showDbgGraphics();
	bool isOnProjectionInterval(float minProj, float maxProj);
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

	void checkIntersection(phCollisionGeometryElement* object, phCollision* collision);
	float project(vec3& axis, vec3& origin);
	inline void calculateParametres();
	void calculateInvertions();
	ElementType getType () { return ET_POLYGON; }
	void showDbgGraphics();
	bool isOnProjectionInterval(float minProj, float maxProj);
};

typedef std::vector<phCollisionGeometryElement*> phCollisionElementsList;

struct phCollisionSupportGeom
{
	phCollisionElementsList mElements;
	phCollisionElementsList mProbablyIntersectingElements;
	
	void postInitialize();

	float projectOnAxis(vec3& axis, vec3& origin, float *maxProjection);

	void calculateParametres();
	void copyTempProjections();
	
	void fillCollisionElementsList(phCollisionElementsList& elementsList, float projectionValue);
	
	void showDbgGraphics();
};

void isIntersect(phCollisionVertex* cvertex, phCollisionPolygon* cpolygon, phCollision* collision);
void isIntersect(phCollisionPolygon* polygonA, phCollisionPolygon* polygonB, phCollision* collision);
void isIntersect(phCollisionEdge* cedge, phCollisionPolygon* cpolygon, phCollision* collision);
void isIntersect(phCollisionEdge* edgeA, phCollisionEdge* edgeB, phCollision* collision);

void checkIntersection(phCollisionElementsList& elementsListA, phCollisionElementsList& elementsListB,  
	                   phCollision* collision);

#endif //CD_STUFF_H  