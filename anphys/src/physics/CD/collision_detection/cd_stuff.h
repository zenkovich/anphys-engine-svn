#ifndef CD_STUFF_H
#define CD_STUFF_H

#include "util/utils.h"

struct phCollision;
struct phCollisionGeometryElement;
struct phCollisionPoint;
struct phCollisionGeometryPart;

typedef std::vector<phCollisionGeometryElement*> phCollisionElementsList;

struct phCollisionGeometryElementAndPointLink
{
	phCollisionGeometryElement* mElement;
	phCollisionPoint*           mPoint;
	unsigned int                mIndex;

	phCollisionGeometryElementAndPointLink(): mElement(NULL), mPoint(NULL), mIndex(0) {}
	phCollisionGeometryElementAndPointLink(phCollisionGeometryElement* element, phCollisionPoint* point):
		mElement(element), mPoint(point), mIndex(0) {}
};
typedef std::vector<phCollisionGeometryElementAndPointLink> phCollisionGeometryElementAndPointLinksList;

struct phCollisionSupportGeom;

struct phCollisionGeometryElement
{
	enum ElementType { ET_VERTEX = 0, ET_EDGE, ET_POLYGON, ET_ELLIPSE };
	
	float                   mProjection;
	unsigned int            mIndex;
	phCollisionSupportGeom* mSupportGeom;

	phCollisionGeometryElementAndPointLinksList  mStoredPoints[2];
	phCollisionGeometryElementAndPointLinksList* mCurrentStoredPoints;
	phCollisionGeometryElementAndPointLinksList* mOtherStoredPoints;

	phCollisionGeometryElement(): mProjection(0.0f), mIndex(0) 
	{ 
		mStoredPoints[0].reserve(16); mStoredPoints[1].reserve(16); 
		mCurrentStoredPoints = &mStoredPoints[0]; mOtherStoredPoints = &mStoredPoints[1]; 
	}
	
	virtual void project(vec3& axis, vec3& origin, unsigned int index) { }
	virtual ElementType getType () { return ET_VERTEX; }
	virtual void showDbgGraphics() {}
	virtual void calculateParametres(AABB& aabb) {}
	virtual bool isOnProjectionInterval(float minProj, float maxProj) { return (!(mProjection < minProj || mProjection > maxProj)); }
	virtual void fillSupportGeomData(phCollisionElementsList& elementsList, vec3& axis) {}
	virtual void postInitialize(AABB& aabb) {}

	inline void storeContactPoint(phCollisionGeometryElement* element, phCollisionPoint* point)
	{
		mCurrentStoredPoints->push_back(phCollisionGeometryElementAndPointLink(element, point));
	}

	inline phCollisionGeometryElementAndPointLink* findContactPoint(phCollisionGeometryElement* element)
	{
		for (phCollisionGeometryElementAndPointLinksList::iterator it = mOtherStoredPoints->begin();
			 it != mOtherStoredPoints->end(); ++it)
		{
			if (it->mElement == element) return &(*it);
		}

		return NULL;
	}

	inline void eraseUnavailableLinks()
	{
		mOtherStoredPoints->clear();
		phCollisionGeometryElementAndPointLinksList* buf = mCurrentStoredPoints;
		mCurrentStoredPoints = mOtherStoredPoints;
		mOtherStoredPoints = buf;
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
	void calculateParametres(AABB& aabb) 
	{ 
		if (mVertex.x > aabb.mMax.x) aabb.mMax.x = mVertex.x;
		if (mVertex.y > aabb.mMax.y) aabb.mMax.y = mVertex.y;
		if (mVertex.z > aabb.mMax.z) aabb.mMax.z = mVertex.z;
		if (mVertex.x < aabb.mMin.x) aabb.mMin.x = mVertex.x;
		if (mVertex.y < aabb.mMin.y) aabb.mMin.y = mVertex.y;
		if (mVertex.z < aabb.mMin.z) aabb.mMin.z = mVertex.z;
	}
	void postInitialize(AABB& aabb) { calculateParametres(aabb); }
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
	inline void calculateParametres(AABB& aabb);
	ElementType getType () { return ET_EDGE; }
	void showDbgGraphics();
	bool isOnProjectionInterval(float minProj, float maxProj);
	void fillSupportGeomData(phCollisionElementsList& elementsList, vec3& axis);
	void postInitialize(AABB& aabb) { calculateParametres(aabb); }
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
	inline void calculateParametres(AABB& aabb);
	void calculateInvertions();
	ElementType getType () { return ET_POLYGON; }
	void showDbgGraphics();
	bool isOnProjectionInterval(float minProj, float maxProj);
	void postInitialize(AABB& aabb) { calculateInvertions(); calculateParametres(aabb); }
};  

struct phCollisionSupportGeom
{
	phCollisionElementsList  mElements;
	phCollisionElementsList  mProbablyIntersectingElements;
	unsigned int             mIndex;
	phCollisionGeometryPart* mCollisionPart;
	
	void postInitialize(AABB& aabb);
	void projectOnAxis(vec3& axis, vec3& origin, float *maxProjection);
	void calculateParametres(AABB& aabb);	
	unsigned int generateNewIndexParam();
	void showDbgGraphics();

	void addElement(phCollisionGeometryElement* element);
};

void isIntersect(phCollisionVertex* cvertex, phCollisionPolygon* cpolygon, phCollision* collision, bool inv);
void isIntersect(phCollisionEdge* edgeA, phCollisionEdge* edgeB, phCollision* collision, vec3& normal);

void checkIntersection(phCollisionElementsList& elementsListA, phCollisionElementsList& elementsListB,  
	                   phCollision* collision, vec3& normal);

#endif //CD_STUFF_H  