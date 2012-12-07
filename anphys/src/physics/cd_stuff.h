#ifndef CD_STUFF_H
#define CD_STUFF_H

#include "../util/utils.h"

struct phCollision;

struct phCollisionGeometryElement
{
	enum ElementType { ET_VERTEX = 0, ET_EDGE, ET_POLYGON, ET_ELLIPSE };
	
	unsigned int mIndexParam;
	float        mProjection;

	phCollisionGeometryElement():mIndexParam(0), mProjection(0.0f) {}

	virtual void checkIntersection(phCollisionGeometryElement* object, phCollision* collision) {}
	virtual float project(vec3& axis, vec3& origin, int index) { return 0.0f; }
	virtual ElementType getType () { return ET_VERTEX; }
	virtual void showDbgGraphics() {}
	virtual void calculateParametres() {}
	virtual void reindex(int index) { mIndexParam = index; }
	virtual bool isOnProjectionInterval(float minProj, float maxProj) { return (!(mProjection < minProj || mProjection > maxProj)); }
};

struct phCollisionVertex:public phCollisionGeometryElement
{
	phCollisionVertex();
	phCollisionVertex(const vec3& vertex);

	vec3         mVertex;

	void checkIntersection(phCollisionGeometryElement* object, phCollision* collision);
	float project(vec3& axis, vec3& origin, int index);
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
	float project(vec3& axis, vec3& origin, int index);
	inline void calculateParametres();
	ElementType getType () { return ET_EDGE; }
	void showDbgGraphics();
	void reindex(int index);
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
	float project(vec3& axis, vec3& origin, int index);
	inline void calculateParametres();
	void calculateInvertions();
	ElementType getType () { return ET_POLYGON; }
	void showDbgGraphics();
	void reindex(int index);
	bool isOnProjectionInterval(float minProj, float maxProj);
};


typedef std::vector<phCollisionGeometryElement*> phCollisionElementsList;

struct phCollisionSupportGeom
{
	typedef std::vector<float>                       ProjectionsList;

	phCollisionElementsList mElements;
	ProjectionsList         mProjectionsBuf;
	ProjectionsList         mTempProjectionsBuf;
	unsigned int            mIndexParam;

	float projectOnAxis(vec3& axis, vec3& origin);
	void showDbgGraphics();
	void calculateParametres();
	inline unsigned int generateNewIndexParam()
	{ 
		unsigned int r = mIndexParam + 1; 
		if (r > 999999) r = 0; return r; 
	}
	void initProjectionBuffers();
	void copyTempProjections();
};

void isIntersect(phCollisionVertex* cvertex, phCollisionPolygon* cpolygon, phCollision* collision);
void isIntersect(phCollisionPolygon* polygonA, phCollisionPolygon* polygonB, phCollision* collision);
void isIntersect(phCollisionEdge* cedge, phCollisionPolygon* cpolygon, phCollision* collision);
void isIntersect(phCollisionEdge* edgeA, phCollisionEdge* edgeB, phCollision* collision);

void checkIntersection(phCollisionSupportGeom* geomA, float aProjection, phCollisionSupportGeom* geomB, float bProjection,
	phCollision* collision);

#endif //CD_STUFF_H