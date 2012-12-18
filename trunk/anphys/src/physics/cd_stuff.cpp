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

void phCollisionVertex::project( vec3& axis, vec3& origin, unsigned int index )
{
	if (mIndex != index)
	{
		mProjection = (mVertex - origin)*axis;
		mIndex = index;
	}
}

void phCollisionVertex::showDbgGraphics()
{
	getRenderStuff().addRedCube(mVertex);
}

phCollisionEdge::phCollisionEdge():
	mFirstVertex(NULL), mSecondVertex(NULL), mLength(0.0f), phCollisionGeometryElement() 
{
	mPolygons[0] = mPolygons[1] = NULL;
}

phCollisionEdge::phCollisionEdge( phCollisionVertex* first, phCollisionVertex* second ):mFirstVertex(first),
	mSecondVertex(second), phCollisionGeometryElement()
{
	mPolygons[0] = mPolygons[1] = NULL;
	calculateParametres();
}

void phCollisionEdge::checkIntersection( phCollisionGeometryElement* object, phCollision* collision )
{
	if (object->getType() == ET_EDGE)         isIntersect(this, static_cast<phCollisionEdge*>(object), collision);
	else if (object->getType() == ET_POLYGON) isIntersect(this, static_cast<phCollisionPolygon*>(object), collision);
}

void phCollisionEdge::project( vec3& axis, vec3& origin, unsigned int index )
{	
	if (mIndex != index)
	{
		mFirstVertex->project(axis, origin, index);
		mSecondVertex->project(axis, origin, index);

		if (mFirstVertex->mProjection > mSecondVertex->mProjection) mProjection = mFirstVertex->mProjection; 
		else                                                        mProjection = mSecondVertex->mProjection;

		mIndex = index;
	}
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

bool phCollisionEdge::isOnProjectionInterval( float minProj, float maxProj )
{
	return mFirstVertex->isOnProjectionInterval(minProj, maxProj) && 
		   mSecondVertex->isOnProjectionInterval(minProj, maxProj);
}

void phCollisionEdge::fillSupportGeomData( phCollisionElementsList& elementsList, vec3& axis )
{
	elementsList.push_back(this);
	
	phCollisionVertex* vertexa = mFirstVertex;
	phCollisionVertex* vertexb = mSecondVertex;
	elementsList.push_back(vertexa);
	elementsList.push_back(vertexb);

	for (int i = 0; i < 2; i++)
	{
		phCollisionPolygon* poly = mPolygons[0];
		if (!poly) return;

		float axisProjection = axis*poly->mNormal;
		if (axisProjection < 0.83f) continue;

		elementsList.push_back(poly);
		for (short j = 0; j < poly->mEdgesCount; j++)
		{
			phCollisionEdge* edge = poly->mEdges[j];
			if (edge != this) elementsList.push_back(edge);

			phCollisionVertex* pushVertex = NULL;
			if (poly->mEdgeInvertion[j]) pushVertex = edge->mSecondVertex;
			else                         pushVertex = edge->mFirstVertex;

			if (!(pushVertex == vertexa || pushVertex == vertexb))
				elementsList.push_back(pushVertex);
		}
	}
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
		if (mEdges[i]->mPolygons[0] == NULL) mEdges[i]->mPolygons[0] = this;
		else                                 mEdges[i]->mPolygons[1] = this;
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

void phCollisionPolygon::project( vec3& axis, vec3& origin, unsigned int index )
{
	if (mIndex != index)
	{
		float maxProjection = 0.0f;
		for (short i = 0; i < mEdgesCount; i++)
		{
			phCollisionEdge* cedge = mEdges[i];

			if (!(cedge->mIndex != index)) continue;

			cedge->project(axis, origin, index);
			if (cedge->mProjection > maxProjection) maxProjection = cedge->mProjection;
		}
		mProjection = maxProjection;

		mIndex = index;
	}
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

bool phCollisionPolygon::isOnProjectionInterval( float minProj, float maxProj )
{
	for (short i = 0; i < mEdgesCount; i++)
	{
		if (mEdgeInvertion[i])
		{
			if (!mEdges[i]->mSecondVertex->isOnProjectionInterval(minProj, maxProj)) return false;
		}
		else
		{
			if (!mEdges[i]->mFirstVertex->isOnProjectionInterval(minProj, maxProj)) return false;
		}
	}

	return true;
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

void phCollisionSupportGeom::projectOnAxis( vec3& axis, vec3& origin, float *maxProjection )
{	
	float currentMaxProjection = 0.0f;

	unsigned int tempIndex = generateNewIndexParam();

	for(phCollisionElementsList::iterator it = mElements.begin(); it != mElements.end(); ++it)
	{
		phCollisionGeometryElement* celement = *it;

		if (celement->getType() == phCollisionGeometryElement::ET_VERTEX) break;

		celement->project(axis, origin, tempIndex);

		if (celement->mProjection > currentMaxProjection)
			currentMaxProjection = celement->mProjection;
	}

	if (currentMaxProjection > *maxProjection)
	{
		*maxProjection = currentMaxProjection;
		float projDifference = 0.05f;
		float minProj = currentMaxProjection - projDifference;
		float maxProj = currentMaxProjection + projDifference;

		phCollisionElementsList::iterator jt = mProbablyIntersectingElements.begin();
		for(phCollisionElementsList::iterator it = mElements.begin(); it != mElements.end(); ++it)
		{
			if ((*it)->mIndex == tempIndex && (*it)->isOnProjectionInterval(minProj, maxProj))
			{
				(*jt) = *it;
				++jt;
			}
		}
		if (jt != mProbablyIntersectingElements.end()) (*jt) = NULL;
	}

	mIndex = tempIndex;
}

void phCollisionSupportGeom::showDbgGraphics()
{	
	unsigned int tempIndexParam = generateNewIndexParam();

	for(phCollisionElementsList::iterator it = mElements.begin(); it != mElements.end(); it++)
	{
		if ((*it)->mIndex != tempIndexParam) (*it)->showDbgGraphics();
	}

	mIndex = tempIndexParam;
}

void phCollisionSupportGeom::calculateParametres()
{
	unsigned int tempIndexParam = generateNewIndexParam();

	for(phCollisionElementsList::iterator it = mElements.begin(); it != mElements.end(); it++)
	{
		if ((*it)->mIndex != tempIndexParam) (*it)->calculateParametres();
	}

	mIndex = tempIndexParam;
}

void phCollisionSupportGeom::postInitialize()
{
	std::sort(mElements.begin(), mElements.end(), phCollisionGeometryElementSortPred);

	for (int i = 0; i < (int)mElements.size(); i++)
		mProbablyIntersectingElements.push_back(NULL);
}

unsigned int phCollisionSupportGeom::generateNewIndexParam()
{
	unsigned int newIndex = mIndex + 1;
	if (newIndex > 99999999) newIndex = 0;
	return newIndex;
}
