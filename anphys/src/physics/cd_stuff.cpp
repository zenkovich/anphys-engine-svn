#include "cd_stuff.h"

#include <algorithm>

#include "collision.h"
#include "collision_point.h"

//utils
#include "../util/debug/render_stuff.h"

phCollisionVertex::phCollisionVertex():phCollisionGeometryElement() {}

phCollisionVertex::phCollisionVertex( const vec3& vertex ):phCollisionGeometryElement(), mVertex(vertex) {}

void phCollisionVertex::checkIntersection( phCollisionGeometryElement* object, phCollision* collision)
{
	if (object->getType() == ET_POLYGON) isIntersect(this, static_cast<phCollisionPolygon*>(object), collision);
}

float phCollisionVertex::project( vec3& axis, vec3& origin, int index )
{
	mIndexParam = index;
	return (mVertex - origin)*axis;
}

void phCollisionVertex::showDbgGraphics()
{
	getRenderStuff().addRedCube(mVertex);
}

phCollisionEdge::phCollisionEdge():mFirstVertex(NULL), mSecondVertex(NULL), mLength(0.0f), phCollisionGeometryElement() {}

phCollisionEdge::phCollisionEdge( phCollisionVertex* first, phCollisionVertex* second ):mFirstVertex(first),
	mSecondVertex(second), phCollisionGeometryElement()
{
	calculateParametres();
}

void phCollisionEdge::checkIntersection( phCollisionGeometryElement* object, phCollision* collision )
{
	if (object->getType() == ET_EDGE)         isIntersect(this, static_cast<phCollisionEdge*>(object), collision);
	else if (object->getType() == ET_POLYGON) isIntersect(this, static_cast<phCollisionPolygon*>(object), collision);
}

float phCollisionEdge::project( vec3& axis, vec3& origin, int index )
{	
	float projFirst = (mFirstVertex->mIndexParam != index) ? mFirstVertex->project(axis, origin, index):0.0f;
	float projSecond = (mSecondVertex->mIndexParam != index) ? mSecondVertex->project(axis, origin, index):0.0f;

	if (fabs(projFirst) > fabs(projSecond)) return projFirst; 
	return projSecond;
}

void phCollisionEdge::calculateParametres()
{
	mDirection = mSecondVertex->mVertex - mFirstVertex->mVertex;
	mLength = mDirection.len();
	mNormalizedDirection = mDirection/mLength;

	//*gLog << formatStr("n dir %.3f %.3f %.3f\n", mNormalizedDirection.x, mNormalizedDirection.y, mNormalizedDirection.z);
}

void phCollisionEdge::showDbgGraphics()
{
	mFirstVertex->showDbgGraphics();
	mSecondVertex->showDbgGraphics();
	
	getRenderStuff().addBlueArrow(mFirstVertex->mVertex, mSecondVertex->mVertex);
	getRenderStuff().addBlueArrow(mFirstVertex->mVertex, mFirstVertex->mVertex + mNormalizedDirection);
	getRenderStuff().addRedArrow((mFirstVertex->mVertex + mSecondVertex->mVertex)*0.5f, 
		                          (mFirstVertex->mVertex + mSecondVertex->mVertex)*0.5f + mDirectionNormal);
}

void phCollisionEdge::reindex( int index )
{
	mIndexParam = index;
	mFirstVertex->mIndexParam = index;
	mSecondVertex->mIndexParam = index;
}

bool phCollisionEdge::isOnProjectionInterval( float minProj, float maxProj )
{
	return mFirstVertex->isOnProjectionInterval(minProj, maxProj) || 
		   mSecondVertex->isOnProjectionInterval(minProj, maxProj);
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

void phCollisionPolygon::checkIntersection( phCollisionGeometryElement* object, phCollision* collision )
{
	if (object->getType() == ET_VERTEX)       isIntersect(static_cast<phCollisionVertex*>(object), this, collision);
	else if (object->getType() == ET_EDGE)    isIntersect(static_cast<phCollisionEdge*>(object), this, collision);
	else if (object->getType() == ET_POLYGON) isIntersect(this, static_cast<phCollisionPolygon*>(object), collision);
}

float phCollisionPolygon::project( vec3& axis, vec3& origin, int index )
{
	float maxProjection = 0.0f;
	float absMaxProjection = 0.0f;
	for (short i = 0; i < mEdgesCount; i++)
	{
		if (!(mEdges[i]->mIndexParam != index)) continue;

		float proj = mEdges[i]->project(axis, origin, index);
		float absProj = fabs(proj);
		if (absProj > absMaxProjection)
		{
			maxProjection = proj;
			absMaxProjection = absProj;
		}
	}

	mIndexParam = index;

	return maxProjection;
}

void phCollisionPolygon::calculateParametres()
{
	mNormal = mEdges[0]->mNormalizedDirection^mEdges[1]->mNormalizedDirection;

	for (short i = 0; i < mEdgesCount; i++)
		mEdges[i]->mDirectionNormal = mEdges[i]->mNormalizedDirection^mNormal;
}

void phCollisionPolygon::showDbgGraphics()
{
	vec3 center;
	for (short i = 0; i < mEdgesCount; i++)
	{
		mEdges[i]->showDbgGraphics();
		center = center + mEdges[i]->mFirstVertex->mVertex + mEdges[i]->mSecondVertex->mVertex;
	}
	
	center /= (float)(mEdgesCount*2);

	getRenderStuff().addBlueCube(center);
	getRenderStuff().addGreenArrow(center, center + mNormal);
}

void phCollisionPolygon::reindex( int index )
{
	mIndexParam = index;
	for (short i = 0; i < mEdgesCount; i++)
		mEdges[i]->reindex(index);
}

bool phCollisionPolygon::isOnProjectionInterval( float minProj, float maxProj )
{
	for (short i = 0; i < mEdgesCount; i++)
	{
		if (mEdgeInvertion[i])
		{
			if (mEdges[i]->mSecondVertex->isOnProjectionInterval(minProj, maxProj)) return true;
		}
		else
		{
			if (mEdges[i]->mFirstVertex->isOnProjectionInterval(minProj, maxProj)) return true;
		}
	}

	return false;
}

void isIntersect( phCollisionVertex* cvertex, phCollisionPolygon* cpolygon, phCollision* collision )
{
	for (short i = 0; i < cpolygon->mEdgesCount; i++)
	{
		float projection = cpolygon->mEdges[i]->mDirectionNormal*
			(cvertex->mVertex - cpolygon->mEdges[i]->mFirstVertex->mVertex);

		if (cpolygon->mEdgeInvertion[i]) projection = -projection;

		if (projection > 0.0f) return;
	}

	//need find same point
	phCollisionPoint* cPoint = collision->mPoints->push_back();
	cPoint->mDepth = (cvertex->mVertex - cpolygon->mEdges[0]->mFirstVertex->mVertex)*cpolygon->mNormal;
	cPoint->mPoint = cvertex->mVertex + cpolygon->mNormal*(cPoint->mDepth*0.5f);
	cPoint->mNormal = cpolygon->mNormal;
}

void isIntersect( phCollisionPolygon* polygonA, phCollisionPolygon* polygonB, phCollision* collision )
{
	for (short i = 0; i < polygonA->mEdgesCount; i++)
	{
		phCollisionEdge* edgeA = polygonA->mEdges[i];
		phCollisionVertex* vertexA;
		if (polygonA->mEdgeInvertion[i]) vertexA = edgeA->mSecondVertex;
		else                             vertexA = edgeA->mFirstVertex;

		bool vertexInPolygon = true;

		for (short j = 0; j < polygonB->mEdgesCount; j++)
		{
			phCollisionEdge* edgeB = polygonA->mEdges[i];

			float vertexProjection = (vertexA->mVertex - edgeB->mFirstVertex->mVertex)*edgeB->mDirectionNormal;
			if (vertexProjection > 0.0f) vertexInPolygon = false;

			float aProj, bProj;
			DistLines(edgeA->mFirstVertex->mVertex, edgeA->mNormalizedDirection,
		              edgeB->mFirstVertex->mVertex, edgeB->mNormalizedDirection,
			          &aProj, &bProj);

			if (!(aProj < 0.0f || aProj > edgeA->mLength || bProj < 0.0f || bProj > edgeB->mLength))
			{	
				vec3 aProjPoint = edgeA->mFirstVertex->mVertex + edgeA->mNormalizedDirection*aProj;
				vec3 bProjPoint = edgeB->mFirstVertex->mVertex + edgeB->mNormalizedDirection*bProj;

				//need find same point
				phCollisionPoint* cPoint = collision->mPoints->push_back();
				cPoint->mNormal = bProjPoint - aProjPoint;
				cPoint->mDepth = cPoint->mNormal.len();
				cPoint->mNormal /= cPoint->mDepth;
				cPoint->mPoint = (bProjPoint + aProjPoint)*0.5f;
			}
		}

		if (vertexInPolygon)
		{
			//need find same point
			phCollisionPoint* cPoint = collision->mPoints->push_back();
			cPoint->mDepth = (vertexA->mVertex - polygonB->mEdges[0]->mFirstVertex->mVertex)*polygonB->mNormal;
			cPoint->mPoint = vertexA->mVertex + polygonB->mNormal*(cPoint->mDepth*0.5f);
			cPoint->mNormal = polygonB->mNormal;
		}
	}
}

void isIntersect( phCollisionEdge* cedge, phCollisionPolygon* cpolygon, phCollision* collision )
{
	for (short i = 0; i < cpolygon->mEdgesCount; i++)
	{
		phCollisionEdge* polyEdge = cpolygon->mEdges[i];

		isIntersect(cedge, polyEdge, collision);
	}
}

void isIntersect( phCollisionEdge* edgeA, phCollisionEdge* edgeB, phCollision* collision )
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

void checkIntersection( phCollisionSupportGeom* geomA, float aProjection, 
	                    phCollisionSupportGeom* geomB, float bProjection, phCollision* collision )
{
	float projectionDefference = 0.05f;
	
	unsigned int tempIndexParamA = geomA->generateNewIndexParam();
	unsigned int tempIndexParamB = geomB->generateNewIndexParam();

	for (phCollisionElementsList::reverse_iterator it = geomA->mElements.rbegin(); it != geomA->mElements.rend(); it++)
	{
		phCollisionGeometryElement* elemA = *it;
		if (!(elemA->mIndexParam != tempIndexParamA)) continue;

		for (phCollisionElementsList::reverse_iterator jt = geomB->mElements.rbegin(); jt != geomB->mElements.rend(); jt++)
		{			
			phCollisionGeometryElement* elemB = *it;
			if (!(elemB->mIndexParam != tempIndexParamB)) continue;

			elemA->checkIntersection(elemB, collision);
		}

		elemA->reindex(tempIndexParamA);
	}
	
	geomA->mIndexParam = tempIndexParamA;
	geomB->mIndexParam = tempIndexParamB;
}

void checkIntersection( phCollisionElementsList& elementsListA, phCollisionElementsList& elementsListB,  
	                   phCollision* collision )
{
	for (phCollisionElementsList::iterator ielement = elementsListA.begin(); ielement != elementsListA.end(); 
		 ++ielement)
	{
		phCollisionGeometryElement* elementA = *ielement;
		for (phCollisionElementsList::iterator jelement = elementsListA.begin(); jelement != elementsListB.end(); 
				++jelement)
		{
			phCollisionGeometryElement* elementB = *jelement;
			
			elementA->checkIntersection(elementB, collision);
		}
	}
}

float phCollisionSupportGeom::projectOnAxis( vec3& axis, vec3& origin, float *maxProjection )
{	
	float currentMaxProjection = 0.0f;

	for(phCollisionElementsList::iterator it = mElements.begin(); it != mElements.end(); ++it)
	{
		if ((*it)->getType() != phCollisionGeometryElement::ET_VERTEX) continue;

		(*it)->mProjection = (*it)->project(axis, origin);

		if ((*it)->mProjection > currentMaxProjection)
			currentMaxProjection = (*it)->mProjection;
	}

	if (currentMaxProjection > *maxProjection)
	{
		float projDifference = 0.05f;
		mProbablyIntersectingElements.clear();

		phCollisionElementsList::iterator jt = mProbablyIntersectingElements.begin();
		for(phCollisionElementsList::iterator it = mElements.begin(); it != mElements.end(); ++it)
		{
			if ((*it)->isOnProjectionInterval())
			{
				(*jt) = *it;
				++jt;
			}
		}
		if (jt != mProbablyIntersectingElements.end()) (*jt) = NULL;
	}

	return;
}

void phCollisionSupportGeom::showDbgGraphics()
{	
	unsigned int tempIndexParam = generateNewIndexParam();

	for(phCollisionElementsList::iterator it = mElements.begin(); it != mElements.end(); it++)
	{
		if ((*it)->mIndexParam != tempIndexParam) (*it)->showDbgGraphics();
	}

	mIndexParam = tempIndexParam;
}

void phCollisionSupportGeom::calculateParametres()
{
	unsigned int tempIndexParam = generateNewIndexParam();

	for(phCollisionElementsList::iterator it = mElements.begin(); it != mElements.end(); it++)
	{
		if ((*it)->mIndexParam != tempIndexParam) (*it)->calculateParametres();
	}

	mIndexParam = tempIndexParam;
}

void phCollisionSupportGeom::postInitialize()
{
	std::sort(mElements.begin(), mElements.end(), phCollisionGeometryElementSortPred);

	for(phCollisionElementsList::iterator it = mElements.begin(); it != mElements.end(); it++)
	{
		mTempProjectionsBuf.push_back(0.0f);
		mProjectionsBuf.push_back(0.0f);
	}
}

void phCollisionSupportGeom::copyTempProjections()
{
	ProjectionsList::iterator iproj = mTempProjectionsBuf.begin();
	for (phCollisionElementsList::iterator ielement = mElements.begin(); ielement != mElements.end(); ++ielement, ++iproj)
	{
		(*ielement)->mProjection = (*iproj);
	}
}

void phCollisionSupportGeom::fillCollisionElementsList( phCollisionElementsList& elementsList, float projectionValue )
{

	int tempIndex = generateNewIndexParam();

	elementsList.clear();
	for (phCollisionElementsList::iterator ielement = mElements.begin(); ielement != mElements.end(); ++ielement)
	{
		float diff = (*ielement)->mProjection - projectionValue;
		if ((*ielement)->mIndexParam != tempIndex && fabs(diff) > projDifference) continue;

		(*ielement)->reindex(tempIndex);
		elementsList.push_back(*ielement);
	}

	mIndexParam = tempIndex;
}