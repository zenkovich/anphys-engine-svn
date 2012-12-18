#ifndef CD_STUFF_H
#define CD_STUFF_H

#include "../util/utils.h"

struct phCollision;

struct phCollisionGeometryElement
{
	enum ElementType { ET_VERTEX = 0, ET_EDGE, ET_POLYGON, ET_ELLIPSE };
	
	float        mProjection;
	unsigned int mIndex;

	phCollisionGeometryElement(): mProjection(0.0f), mIndex(0) {}
	
	virtual void checkIntersection(phCollisionGeometryElement* object, phCollision* collision) {}
	virtual void project(vec3& axis, vec3& origin, unsigned int index) { }
	virtual ElementType getType () { return ET_VERTEX; }
	virtual void showDbgGraphics() {}
	virtual void calculateParametres() {}
	virtual bool isOnProjectionInterval(float minProj, float maxProj) { return (!(mProjection < minProj || mProjection > maxProj)); }
	virtual void fillSupportGeomData(phCollisionElementsList& elementsList, vec3& axis) {}
};

typedef std::vector<phCollisionGeometryElement*> phCollisionElementsList;

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
	void project(vec3& axis, vec3& origin, unsigned int index);
	ElementType getType () { return ET_VERTEX; }
	void showDbgGraphics();
};

struct phCollisionPolygon;

struct phCollisionEdge:public phCollisionGeometryElement
{
	phCollisionVertex*  mFirstVertex;
	phCollisionVertex*  mSecondVertex;
	phCollisionPolygon* mPolygons[2];
	vec3                mDirection;
	vec3                mNormalizedDirection;
	vec3                mDirectionNormal;
	float               mLength;

	phCollisionEdge();
	phCollisionEdge(phCollisionVertex* first, phCollisionVertex* second);

	void checkIntersection(phCollisionGeometryElement* object, phCollision* collision);
	void project(vec3& axis, vec3& origin, unsigned int index);
	inline void calculateParametres();
	ElementType getType () { return ET_EDGE; }
	void showDbgGraphics();
	bool isOnProjectionInterval(float minProj, float maxProj);
	void fillSupportGeomData(phCollisionElementsList& elementsList, vec3& axis);
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
	void project(vec3& axis, vec3& origin, unsigned int index);
	inline void calculateParametres();
	void calculateInvertions();
	ElementType getType () { return ET_POLYGON; }
	void showDbgGraphics();
	bool isOnProjectionInterval(float minProj, float maxProj);
};

struct phCollisionSupportGeom
{
	phCollisionElementsList mElements;
	phCollisionElementsList mProbablyIntersectingElements;
	unsigned int            mIndex;
	
	void postInitialize();

	void projectOnAxis(vec3& axis, vec3& origin, float *maxProjection);

	void calculateParametres();
	
	unsigned int generateNewIndexParam();

	void showDbgGraphics();
};

void isIntersect(phCollisionVertex* cvertex, phCollisionPolygon* cpolygon, phCollision* collision);
void isIntersect(phCollisionPolygon* polygonA, phCollisionPolygon* polygonB, phCollision* collision);
void isIntersect(phCollisionEdge* cedge, phCollisionPolygon* cpolygon, phCollision* collision);
void isIntersect(phCollisionEdge* edgeA, phCollisionEdge* edgeB, phCollision* collision);

void checkIntersection(phCollisionElementsList& elementsListA, phCollisionElementsList& elementsListB,  
	                   phCollision* collision);

#endif //CD_STUFF_H  