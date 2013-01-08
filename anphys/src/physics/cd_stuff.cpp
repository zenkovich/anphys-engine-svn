#include "cd_stuff.h"

#include <algorithm>

#include "collision.h"
#include "collision_point.h"

//utils
#include "../util/debug/render_stuff.h"

phCollisionVertex::phCollisionVertex():phCollisionGeometryElement() {}

phCollisionVertex::phCollisionVertex( const vec3& vertex ):phCollisionGeometryElement(), mVertex(vertex) {}

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
}

bool phCollisionEdge::isOnProjectionInterval( float minProj, float maxProj )
{
	return mFirstVertex->isOnProjectionInterval(minProj, maxProj) && 
		   mSecondVertex->isOnProjectionInterval(minProj, maxProj);
}

void phCollisionEdge::fillSupportGeomData( phCollisionElementsList& elementsList, vec3& axis )
{
	phCollisionElementsList::iterator it = elementsList.begin();
	while(*it != NULL) ++it;


	(*it) = this; ++it;
	
	phCollisionVertex* vertexa = mFirstVertex;
	phCollisionVertex* vertexb = mSecondVertex;
	(*it) = vertexa; ++it;
	(*it) = vertexb; ++it;

	for (int i = 0; i < 2; i++)
	{
		phCollisionPolygon* poly = mPolygons[i];
		if (!poly) return;

		float axisProjection = axis*poly->mNormal;
		if (axisProjection < 0.95f) continue;
		
		(*it) = poly; ++it;
		for (short j = 0; j < poly->mEdgesCount; j++)
		{
			phCollisionEdge* edge = poly->mEdges[j];
			if (edge != this) { (*it) = edge; ++it; }

			phCollisionVertex* pushVertex = NULL;
			if (poly->mEdgeInvertion[j]) pushVertex = edge->mSecondVertex;
			else                         pushVertex = edge->mFirstVertex;

			if (!(pushVertex == vertexa || pushVertex == vertexb))
			{
				(*it) = pushVertex; ++it;
			}
		}
	}

	(*it) = NULL;
}

phCollisionPolygon::phCollisionPolygon():phCollisionGeometryElement(), mEdgesCount(0) { }

phCollisionPolygon::phCollisionPolygon( phCollisionEdge* a, phCollisionEdge* b, phCollisionEdge* c )
{
	mEdges[0] = a;
	mEdges[1] = b;
	mEdges[2] = c;
	mEdgeInvertion[0] = mEdgeInvertion[1] = mEdgeInvertion[2] = false;
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
	
	mNormal = mEdges[0]->mNormalizedDirection^mEdges[1]->mNormalizedDirection;
	for (short i = 0; i < mEdgesCount; i++)
	{
		float projection = (center - mEdges[i]->mFirstVertex->mVertex)*(mEdges[i]->mNormalizedDirection^mNormal);
		mEdgeInvertion[i] = projection > 0.0f;
		if (mEdgeInvertion[i]) mEdgeDirectionNormal[i] = mEdgeDirectionNormal[i]*(-1.0f);

		if (mEdges[i]->mPolygons[0] == NULL)      mEdges[i]->mPolygons[0] = this;
		else if (mEdges[i]->mPolygons[1] == NULL) mEdges[i]->mPolygons[1] = this;
	}
}

phCollisionPolygon::phCollisionPolygon( phCollisionEdge* a, phCollisionEdge* b, phCollisionEdge* c, phCollisionEdge* d )
{	
	mEdges[0] = a;
	mEdges[1] = b;
	mEdges[2] = c;
	mEdges[3] = d;	
	mEdgeInvertion[0] = mEdgeInvertion[1] = mEdgeInvertion[2] = mEdgeInvertion[3] = false;
	mEdgesCount = 4;
	calculateParametres();
	calculateInvertions();
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
	{
		mEdgeDirectionNormal[i] = mEdges[i]->mNormalizedDirection^mNormal;
		if (mEdgeInvertion[i]) mEdgeDirectionNormal[i] = mEdgeDirectionNormal[i]*-1.0f;
	}
}

void phCollisionPolygon::showDbgGraphics()
{
	vec3 center;
	for (short i = 0; i < mEdgesCount; i++)
	{
		//mEdges[i]->showDbgGraphics();
		vec3 centerAdd = mEdges[i]->mFirstVertex->mVertex + mEdges[i]->mSecondVertex->mVertex;
		center = center + centerAdd;

		getRenderStuff().addRedArrow(centerAdd*0.5f, centerAdd*0.5f + mEdgeDirectionNormal[i]*0.2f);
	}
	
	center /= (float)(mEdgesCount*2);

	getRenderStuff().addBlueCube(center);
	getRenderStuff().addGreenArrow(center, center + mNormal*0.2f);
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

void isIntersect( phCollisionVertex* cvertex, phCollisionPolygon* cpolygon, phCollision* collision, bool inv )
{
	for (short i = 0; i < cpolygon->mEdgesCount; i++)
	{
		float projection = cpolygon->mEdgeDirectionNormal[i]*
			(cvertex->mVertex - cpolygon->mEdges[i]->mFirstVertex->mVertex);

		//if (cpolygon->mEdgeInvertion[i]) projection = -projection;

		/*if (projection > 0.0f)
		{
			vec3 cnt = cpolygon->mEdges[i]->mFirstVertex->mVertex + 
				cpolygon->mEdges[i]->mNormalizedDirection*
				 ((cvertex->mVertex - cpolygon->mEdges[i]->mFirstVertex->mVertex)*cpolygon->mEdges[i]->mNormalizedDirection);
			getRenderStuff().addRedArrow(cnt, cnt + cpolygon->mEdgeDirectionNormal[i]*projection);
		}
		else
		{
			vec3 cnt = cpolygon->mEdges[i]->mFirstVertex->mVertex + 
				cpolygon->mEdges[i]->mNormalizedDirection*
				 ((cvertex->mVertex - cpolygon->mEdges[i]->mFirstVertex->mVertex)*cpolygon->mEdges[i]->mNormalizedDirection);
			getRenderStuff().addBlueArrow(cnt, cnt + cpolygon->mEdgeDirectionNormal[i]*projection);
		}*/

		if (projection > 0.0f) return;
	}

	//need find same point
	phCollisionPoint* cPoint = cvertex->findContactPoint(cpolygon);

	bool storedPoint = true;
	if (!cPoint) 
	{
		cPoint = collision->addPoint();
		storedPoint = false;
	}

	cPoint->mDepth = (cvertex->mVertex - cpolygon->mEdges[0]->mFirstVertex->mVertex)*cpolygon->mNormal;
	cPoint->mPoint = cvertex->mVertex - cpolygon->mNormal*(cPoint->mDepth*0.5f);
	cPoint->mNormal = cpolygon->mNormal;
	if (inv)  cPoint->mDepth *= -1.0f;
	else      cPoint->mNormal *= -1.0f;

	cPoint->mPartObjectA = cvertex->mSupportGeom->mCollisionPart;
	cPoint->mPartObjectB = cpolygon->mSupportGeom->mCollisionPart;

	cPoint->mCollision->mIndex = cPoint->mCollision->mTempIndex;
}

void isIntersect( phCollisionEdge* edgeA, phCollisionEdge* edgeB, phCollision* collision, vec3& normal )
{
	float aProj, bProj;
	if (!DistLines(edgeA->mFirstVertex->mVertex, edgeA->mNormalizedDirection,
		           edgeB->mFirstVertex->mVertex, edgeB->mNormalizedDirection,
			       &aProj, &bProj)) return;
	
	/*	
	getRenderStuff().addRedCube(aProjPoint);
	getRenderStuff().addRedCube(bProjPoint);
	getRenderStuff().addRedArrow(aProjPoint, bProjPoint);*/

	if (aProj < 0.001f || aProj > edgeA->mLength || bProj < 0.001f || bProj > edgeB->mLength) return;

	vec3 aProjPoint = edgeA->mFirstVertex->mVertex + edgeA->mNormalizedDirection*aProj;
	vec3 bProjPoint = edgeB->mFirstVertex->mVertex + edgeB->mNormalizedDirection*bProj;

	vec3 pointNormal = aProjPoint - bProjPoint;
	float length = pointNormal.len();
	pointNormal /= length;

	float proj = pointNormal*normal;
	if (fabs(proj) < 0.95f) return;

	//need find same point
	phCollisionPoint* cPoint = edgeA->findContactPoint(edgeB);
	if (!cPoint) cPoint = collision->addPoint();
	cPoint->mNormal = pointNormal;
	cPoint->mDepth = length;
	cPoint->mPoint = (bProjPoint + aProjPoint)*0.5f;
	cPoint->mPartObjectA = edgeA->mSupportGeom->mCollisionPart;
	cPoint->mPartObjectB = edgeB->mSupportGeom->mCollisionPart;
	cPoint->mCollision->mIndex = cPoint->mCollision->mTempIndex;
}

void checkIntersection( phCollisionElementsList& elementsListA, phCollisionElementsList& elementsListB,  
	                   phCollision* collision, vec3& normal )
{
	for (phCollisionElementsList::iterator ielement = elementsListA.begin(); ielement != elementsListA.end(); 
		 ++ielement)
	{
		phCollisionGeometryElement* elementA = *ielement;

		if (!elementA) break;

		phCollisionGeometryElement::ElementType elementAType = elementA->getType();

		for (phCollisionElementsList::iterator jelement = elementsListB.begin(); jelement != elementsListB.end(); 
				++jelement)
		{
			phCollisionGeometryElement* elementB = *jelement;

			if (!elementB) break;

			phCollisionGeometryElement::ElementType elementBType = elementB->getType();
			
			if (elementAType == phCollisionGeometryElement::ET_VERTEX &&
				elementBType == phCollisionGeometryElement::ET_POLYGON) 
				isIntersect(static_cast<phCollisionVertex*>(elementA),
				            static_cast<phCollisionPolygon*>(elementB), collision, true);
			
			if (elementAType == phCollisionGeometryElement::ET_POLYGON &&
				elementBType == phCollisionGeometryElement::ET_VERTEX) 
				isIntersect(static_cast<phCollisionVertex*>(elementB),
				            static_cast<phCollisionPolygon*>(elementA), collision, false);
			
			if (elementAType == phCollisionGeometryElement::ET_EDGE &&
				elementBType == phCollisionGeometryElement::ET_EDGE) 
				isIntersect(static_cast<phCollisionEdge*>(elementA),
				            static_cast<phCollisionEdge*>(elementB), collision, normal);
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

	for(phCollisionElementsList::reverse_iterator it = mElements.rbegin(); it != mElements.rend(); it++)
	{
		if ((*it)->mIndex != tempIndexParam) (*it)->calculateParametres();
	}

	mIndex = tempIndexParam;
}

void phCollisionSupportGeom::postInitialize()
{
	std::sort(mElements.begin(), mElements.end(), phCollisionGeometryElementSortPred);

	for (phCollisionElementsList::reverse_iterator it = mElements.rbegin(); it != mElements.rend(); it++)
		(*it)->postInitialize();

	for (int i = 0; i < (int)mElements.size(); i++)
		mProbablyIntersectingElements.push_back(NULL);

	mIndex = 0;
}

unsigned int phCollisionSupportGeom::generateNewIndexParam()
{
	unsigned int newIndex = mIndex + 1;
	if (newIndex > 99999999) newIndex = 0;
	return newIndex;
}

void phCollisionSupportGeom::addElement( phCollisionGeometryElement* element )
{
	mElements.push_back(element);
	element->mSupportGeom = this;
}
