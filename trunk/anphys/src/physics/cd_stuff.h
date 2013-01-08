#ifndef CD_STUFF_H
#define CD_STUFF_H

#include "../util/utils.h"

struct phCollision;
struct phCollisionGeometryElement;
struct phCollisionPoint;
struct phCollisionGeometryPart;

typedef std::vector<phCollisionGeometryElement*> phCollisionElementsList;

struct phCollisionGeometryElementAndPointLink
{
	phCollisionGeometryElement* mElement;
	phCollisionPoint*           mPoint;

	phCollisionGeometryElementAndPointLink(): mElement(NULL), mPoint(NULL) {}
	phCollisionGeometryElementAndPointLink(phCollisionGeometryElement* element, phCollisionPoint* point):
		mElement(element), mPoint(point) {}
};
typedef std::vector<phCollisionGeometryElementAndPointLink> phCollisionGeometryElementAndPointLinksList;

struct phCollisionSupportGeom;

struct phCollisionGeometryElement
{
	enum ElementType { ET_VERTEX = 0, ET_EDGE, ET_POLYGON, ET_ELLIPSE };
	
	float                   mProjection;
	unsigned int            mIndex;
	phCollisionSupportGeom* mSupportGeom;

	phCollisionGeometryElementAndPointLinksList mStoredPoints;

	phCollisionGeometryElement(): mProjection(0.0f), mIndex(0) {}
	
	virtual void project(vec3& axis, vec3& origin, unsigned int index) { }
	virtual ElementType getType () { return ET_VERTEX; }
	virtual void showDbgGraphics() {}
	virtual void calculateParametres() {}
	virtual bool isOnProjectionInterval(float minProj, float maxProj) { return (!(mProjection < minProj || mProjection > maxProj)); }
	virtual void fillSupportGeomData(phCollisionElementsList& elementsList, vec3& axis) {}
	virtual void postInitialize() {}
	inline phCollisionPoint* findContactPoint(phCollisionGeometryElement* element)
	{
		for (phCollisionGeometryElementAndPointLinksList::iterator it = mStoredPoints.begin();
			 it != mStoredPoints.end(); ++it)
		{
			if (it->mElement == element) return it->mPoint;
		}

		return NULL;
	}
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
	float               mLength;

	phCollisionEdge();
	phCollisionEdge(phCollisionVertex* first, phCollisionVertex* second);

	void project(vec3& axis, vec3& origin, unsigned int index);
	inline void calculateParametres();
	ElementType getType () { return ET_EDGE; }
	void showDbgGraphics();
	bool isOnProjectionInterval(float minProj, float maxProj);
	void fillSupportGeomData(phCollisionElementsList& elementsList, vec3& axis);
	void postInitialize() { calculateParametres(); }
};

struct phCollisionPolygon:public phCollisionGeometryElement
{
	phCollisionEdge* mEdges[4];
	bool             mEdgeInvertion[4];
	short            mEdgesCount;
	vec3             mNormal;
	vec3             mEdgeDirectionNormal[4];

	phCollisionPolygon();
	phCollisionPolygon(phCollisionEdge* a, phCollisionEdge* b, phCollisionEdge* c);
	phCollisionPolygon(phCollisionEdge* a, phCollisionEdge* b, phCollisionEdge* c, phCollisionEdge* d);

	void project(vec3& axis, vec3& origin, unsigned int index);
	inline void calculateParametres();
	void calculateInvertions();
	ElementType getType () { return ET_POLYGON; }
	void showDbgGraphics();
	bool isOnProjectionInterval(float minProj, float maxProj);
	void postInitialize() { calculateInvertions(); calculateParametres(); }
};  

struct phCollisionSupportGeom
{
	phCollisionElementsList  mElements;
	phCollisionElementsList  mProbablyIntersectingElements;
	unsigned int             mIndex;
	phCollisionGeometryPart* mCollisionPart;
	
	void postInitialize();
	void projectOnAxis(vec3& axis, vec3& origin, float *maxProjection);
	void calculateParametres();	
	unsigned int generateNewIndexParam();
	void showDbgGraphics();

	void addElement(phCollisionGeometryElement* element);
};

void isIntersect(phCollisionVertex* cvertex, phCollisionPolygon* cpolygon, phCollision* collision, bool inv);
void isIntersect(phCollisionEdge* edgeA, phCollisionEdge* edgeB, phCollision* collision, vec3& normal);

void checkIntersection(phCollisionElementsList& elementsListA, phCollisionElementsList& elementsListB,  
	                   phCollision* collision, vec3& normal);

#endif //CD_STUFF_H  