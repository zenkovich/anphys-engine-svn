#include "geometry.h"

OPEN_O2_NAMESPACE

cGroupGeometry::cGroupGeometry()
{
}

cGroupGeometry::~cGroupGeometry()
{
}

int cGroupGeometry::getPartsCount() const
{
	return mParts.size();
}

int cGroupGeometry::addPart( cGeometry* geom )
{
	mParts.push_back(geom);
	updateAABB();

	return mParts.size() - 1;
}

void cGroupGeometry::removePart( int idx )
{
	if (idx < 0 || idx > (int)mParts.size() - 1)
		return;

	safe_release(mParts[idx]);
	mParts.erase(mParts.begin() + idx);

	updateAABB();
}

void cGroupGeometry::removeAllParts()
{
	for (PartsVec::iterator it = mParts.begin(); it != mParts.end(); ++it)
		safe_release(*it);

	mAABB = fRect();
}

bool cGroupGeometry::isInside( const vec2f& point ) const
{
	if (!mAABB.isInside(point))
		return false;

	for (PartsVec::const_iterator it = mParts.cbegin(); it != mParts.cend(); ++it) 
		if ((*it)->isInside(point))
			return true;

	return false;
}

void cGroupGeometry::updateAABB()
{
	if (mParts.size() == 0)
		return;

	for (PartsVec::iterator it = mParts.begin(); it != mParts.end(); ++it)
	{
		fRect aabb = (*it)->getAABB();
		if (it == mParts.begin())
			mAABB = aabb;
		else
			mAABB = mAABB + aabb;
	}
}

void cGroupGeometry::setPosition( const vec2f& pos )
{
	vec2f delta = pos - mPosition;
	mPosition = pos;

	for (PartsVec::iterator it = mParts.begin(); it != mParts.end(); ++it)
		(*it)->setPosition((*it)->getPosition() + delta);
}

vec2f cGroupGeometry::getPosition() const
{
	return mPosition;
}

cRectGeometry::cRectGeometry() :
	mRect() { }

cRectGeometry::cRectGeometry( const fRect& rect ) :
	mRect(rect) { }

cRectGeometry::cRectGeometry( float left, float top, float right, float down ) :
	mRect(left, top, right, down) { }

cRectGeometry::cRectGeometry( const vec2f& minp, const vec2f& maxp ) :
	mRect(minp, maxp) { }

bool cRectGeometry::isInside( const vec2f& point ) const
{
	return mRect.isInside(point);
}

fRect cRectGeometry::getAABB() const
{
	return mRect;
}

vec2f cRectGeometry::getPosition() const
{
	return mRect.getltCorner();
}

void cRectGeometry::setPosition( const vec2f& pos )
{
	mRect = mRect + (pos - mRect.getltCorner());
}

void cRectGeometry::setSize( const vec2f& size )
{
	mRect.right = mRect.left + size.x; mRect.down = mRect.top + size.y;
}

void cRectGeometry::set( const fRect& rect )
{
	mRect = rect;
}

void cRectGeometry::set( const vec2f& minp, const vec2f& maxp )
{
	mRect.left = minp.x; mRect.right = maxp.x; 
	mRect.top = minp.y; mRect.down = maxp.y;
}


cCircleGeometry::cCircleGeometry() :
	mRadius(0), mSqrRadius(0) { }

cCircleGeometry::cCircleGeometry( const vec2f& center, float radius ) :
	mCenter(center), mRadius(radius)
{
	mSqrRadius = radius*radius;
}

bool cCircleGeometry::isInside( const vec2f& point ) const
{
	vec2f d = point - mCenter; return d*d < mSqrRadius;
}

fRect cCircleGeometry::getAABB() const
{
	return fRect(mCenter - vec2f(mRadius, mRadius), mCenter + vec2f(mRadius, mRadius));
}

vec2f cCircleGeometry::getPosition() const
{
	return mCenter;
}

void cCircleGeometry::setPosition( const vec2f& pos )
{
	mCenter = pos;
}

void cCircleGeometry::set( const vec2f& center, float radius )
{
	mCenter = center; mRadius = radius;
}

CLOSE_O2_NAMESPACE