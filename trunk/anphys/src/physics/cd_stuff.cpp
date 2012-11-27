#include "cd_stuff.h"

#include "collision.h"
#include "collision_point.h"

phCollisionVertex::phCollisionVertex():phCollisionGeometryElement() {}

phCollisionVertex::phCollisionVertex( const vec3& vertex ):phCollisionGeometryElement(), mVertex(vertex) {}

void phCollisionVertex::checkIntersection( phCollisionGeometryElement* object, phCollision* collision, 
	                                       unsigned int indexParam )
{
	if (mIndexParam == indexParam) return;

	if (object->getType() == ET_POLYGON) isIntersect(this, static_cast<phCollisionPolygon*>(object), collision, indexParam);
}

float phCollisionVertex::project( const vec3& axis, const vec3& origin, unsigned int indexParam )
{
	if (mIndexParam == indexParam) return 0.0f;

	mIndexParam = indexParam;
	return (mVertex - origin)*axis;
}

phCollisionEdge::phCollisionEdge():mFirstVertex(NULL), mSecondVertex(NULL), mLength(0.0f), phCollisionGeometryElement() {}

phCollisionEdge::phCollisionEdge( phCollisionVertex* first, phCollisionVertex* second ):mFirstVertex(first),
	mSecondVertex(second), phCollisionGeometryElement()
{
	calculateParametres();
}

void phCollisionEdge::checkIntersection( phCollisionGeometryElement* object, phCollision* collision, unsigned int indexParam )
{
	if (mIndexParam == indexParam) return;

	if (object->getType() == ET_EDGE)         isIntersect(this, static_cast<phCollisionEdge*>(object), collision, indexParam);
	else if (object->getType() == ET_POLYGON) isIntersect(this, static_cast<phCollisionPolygon*>(object), collision, indexParam);
}

float phCollisionEdge::project( const vec3& axis, const vec3& origin, unsigned int indexParam )
{	
	if (mIndexParam == indexParam) return 0.0f;

	float projFirst = mFirstVertex->project(axis, origin, indexParam);
	float projSecond = mSecondVertex->project(axis, origin, indexParam);

	if (fabs(projFirst) > fabs(projSecond)) return projFirst; 
	return projSecond;
}

void phCollisionEdge::calculateParametres()
{
	mDirection = mSecondVertex->mVertex - mFirstVertex->mVertex;
	mLength = mDirection.len();
	mNormalizedDirection = mDirection/mLength;
}

phCollisionPolygon::phCollisionPolygon():phCollisionGeometryElement(), mEdgesCount(0) { }

phCollisionPolygon::phCollisionPolygon( phCollisionEdge* a, phCollisionEdge* b, phCollisionEdge* c )
{
	mEdges[0] = a;
	mEdges[1] = b;
	mEdges[2] = c;
	mEdgesCount = 3;
	calculateParametres();
	calculateInvertions();
}

void phCollisionPolygon::calculateInvertions()
{
	vec3 center;
	for (short i = 0; i < mEdgesCount; i++)
	{
		center += mEdges[i]->mFirstVertex->mVertex + mEdges[i]->mSecondVertex->mVertex;
	}
	center /= (float)(mEdgesCount*2);
	for (short i = 0; i < mEdgesCount; i++)
	{
		mEdgeInvertion[i] = (center*(mEdges[i]->mDirectionNormal^mNormal)) < 0.0f;
	}
}

phCollisionPolygon::phCollisionPolygon( phCollisionEdge* a, phCollisionEdge* b, phCollisionEdge* c, phCollisionEdge* d )
{	
	mEdges[0] = a;
	mEdges[1] = b;
	mEdges[2] = c;
	mEdges[3] = d;
	mEdgesCount = 4;
	calculateParametres();
	calculateInvertions();
}

void phCollisionPolygon::checkIntersection( phCollisionGeometryElement* object, phCollision* collision, unsigned int indexParam )
{
	if (mIndexParam == indexParam) return;

	if (object->getType() == ET_VERTEX)       isIntersect(static_cast<phCollisionVertex*>(object), this, collision, indexParam);
	else if (object->getType() == ET_EDGE)    isIntersect(static_cast<phCollisionEdge*>(object), this, collision, indexParam);
	else if (object->getType() == ET_POLYGON) isIntersect(this, static_cast<phCollisionPolygon*>(object), collision, indexParam);
}

float phCollisionPolygon::project( const vec3& axis, const vec3& origin, unsigned int indexParam )
{
	if (mIndexParam == indexParam) return 0.0f;

	float maxProjection = 0.0f;
	for (short i = 0; i < mEdgesCount; i++)
	{
		float proj = mEdges[i]->project(axis, origin, indexParam);
		if (fabs(proj) > fabs(maxProjection))
		{
			maxProjection = proj;
		}
	}

	return maxProjection;
}

void phCollisionPolygon::calculateParametres()
{
	mNormal = mEdges[0]->mNormalizedDirection^mEdges[1]->mNormalizedDirection;

	for (short i = 0; i < mEdgesCount; i++)
		mEdges[i]->mDirectionNormal = mEdges[i]->mNormalizedDirection^mNormal;
}

void isIntersect( phCollisionVertex* cvertex, phCollisionPolygon* cpolygon, phCollision* collision, unsigned int indexParam )
{
	float lastProjection = 0.0f;
	for (short i = 0; i < cpolygon->mEdgesCount; i++)
	{
		float projection = cpolygon->mEdges[i]->mDirectionNormal*
			(cvertex->mVertex - cpolygon->mEdges[i]->mFirstVertex->mVertex);

		if (cpolygon->mEdgeInvertion[i]) projection = -projection;

		if (i > 0)
		{
			if ((lastProjection < 0 && projection > 0) ||
				(lastProjection > 0 && projection < 0)) return;
		}
		
		lastProjection = projection;
	}

	//need find same point
	phCollisionPoint* cPoint = collision->mPoints->push_back();
	cPoint->mDepth = (cvertex->mVertex - cpolygon->mEdges[0]->mFirstVertex->mVertex)*cpolygon->mNormal;
	cPoint->mPoint = cvertex->mVertex + cpolygon->mNormal*(cPoint->mDepth*0.5f);
	cPoint->mNormal = cpolygon->mNormal;
}

void isIntersect( phCollisionPolygon* polygonA, phCollisionPolygon* polygonB, phCollision* collision, unsigned int indexParam )
{

}

void isIntersect( phCollisionEdge* cedge, phCollisionPolygon* cpolygon, phCollision* collision, unsigned int indexParam )
{
	for (short i = 0; i < cpolygon->mEdgesCount; i++)
	{
		phCollisionEdge* polyEdge = cpolygon->mEdges[i];
		if (polyEdge->mIndexParam == indexParam) continue;

		isIntersect(cedge, polyEdge, collision, indexParam);
	}

	if (cedge->mIndexParam == indexParam) return;

	if (cedge->mFirstVertex->mIndexParam != indexParam) 
		isIntersect(cedge->mFirstVertex, cpolygon, collision, indexParam);

	if (cedge->mSecondVertex->mIndexParam != indexParam) 
		isIntersect(cedge->mSecondVertex, cpolygon, collision, indexParam);
}

void isIntersect( phCollisionEdge* edgeA, phCollisionEdge* edgeB, phCollision* collision, unsigned int indexParam )
{
	float aProj, bProj;
	DistLines(edgeA->mFirstVertex->mVertex, edgeA->mNormalizedDirection,
		      edgeB->mFirstVertex->mVertex, edgeB->mNormalizedDirection,
			  &aProj, &bProj);

	if (aProj < 0.0f || aProj > edgeA->mLength || bProj < 0.0f || bProj > edgeB->mLength) return;
	
	vec3 aProjPoint = edgeA->mFirstVertex->mVertex + edgeA->mNormalizedDirection*aProj;
	vec3 bProjPoint = edgeB->mFirstVertex->mVertex + edgeB->mNormalizedDirection*bProj;

	//need find same point
	phCollisionPoint* cPoint = collision->mPoints->push_back();
	cPoint->mNormal = bProjPoint - aProjPoint;
	cPoint->mDepth = cPoint->mNormal.len();
	cPoint->mNormal /= cPoint->mDepth;
	cPoint->mPoint = (bProjPoint + aProjPoint)*0.5f;
}
