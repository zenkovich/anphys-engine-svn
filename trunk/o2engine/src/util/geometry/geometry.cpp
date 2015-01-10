#include "geometry.h"

OPEN_O2_NAMESPACE

GroupGeometry::GroupGeometry()
{
}

GroupGeometry::~GroupGeometry()
{
}

int GroupGeometry::getPartsCount() const
{
	return mParts.size();
}

int GroupGeometry::addPart( Geometry* geom )
{
	mParts.push_back(geom);
	updateAABB();

	return mParts.size() - 1;
}

void GroupGeometry::removePart( int idx )
{
	if (idx < 0 || idx > (int)mParts.size() - 1)
		return;

	safe_release(mParts[idx]);
	mParts.erase(mParts.begin() + idx);

	updateAABB();
}

void GroupGeometry::removeAllParts()
{
	for (PartsVec::iterator it = mParts.begin(); it != mParts.end(); ++it)
		safe_release(*it);

	mAABB = fRect();
}

bool GroupGeometry::isInside( const vec2f& point ) const
{
	if (!mAABB.isInside(point))
		return false;

	for (PartsVec::const_iterator it = mParts.cbegin(); it != mParts.cend(); ++it) 
		if ((*it)->isInside(point))
			return true;

	return false;
}

void GroupGeometry::updateAABB()
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

void GroupGeometry::setPosition( const vec2f& pos )
{
	vec2f delta = pos - mPosition;
	mPosition = pos;

	for (PartsVec::iterator it = mParts.begin(); it != mParts.end(); ++it)
		(*it)->setPosition((*it)->getPosition() + delta);
}

vec2f GroupGeometry::getPosition() const
{
	return mPosition;
}

RectGeometry::RectGeometry() :
	mRect() { }

RectGeometry::RectGeometry( const fRect& rect ) :
	mRect(rect) { }

RectGeometry::RectGeometry( float left, float top, float right, float down ) :
	mRect(left, top, right, down) { }

RectGeometry::RectGeometry( const vec2f& minp, const vec2f& maxp ) :
	mRect(minp, maxp) { }

bool RectGeometry::isInside( const vec2f& point ) const
{
	return mRect.isInside(point);
}

fRect RectGeometry::getAABB() const
{
	return mRect;
}

vec2f RectGeometry::getPosition() const
{
	return mRect.getltCorner();
}

void RectGeometry::setPosition( const vec2f& pos )
{
	mRect = mRect + (pos - mRect.getltCorner());
}

void RectGeometry::setSize( const vec2f& size )
{
	mRect.right = mRect.left + size.x; mRect.down = mRect.top + size.y;
}

void RectGeometry::set( const fRect& rect )
{
	mRect = rect;
}

void RectGeometry::set( const vec2f& minp, const vec2f& maxp )
{
	mRect.left = minp.x; mRect.right = maxp.x; 
	mRect.top = minp.y; mRect.down = maxp.y;
}


CircleGeometry::CircleGeometry() :
	mRadius(0), mSqrRadius(0) { }

CircleGeometry::CircleGeometry( const vec2f& center, float radius ) :
	mCenter(center), mRadius(radius)
{
	mSqrRadius = radius*radius;
}

bool CircleGeometry::isInside( const vec2f& point ) const
{
	vec2f d = point - mCenter; return d*d < mSqrRadius;
}

fRect CircleGeometry::getAABB() const
{
	return fRect(mCenter - vec2f(mRadius, mRadius), mCenter + vec2f(mRadius, mRadius));
}

vec2f CircleGeometry::getPosition() const
{
	return mCenter;
}

void CircleGeometry::setPosition( const vec2f& pos )
{
	mCenter = pos;
}

void CircleGeometry::set( const vec2f& center, float radius )
{
	mCenter = center; mRadius = radius;
}

CLOSE_O2_NAMESPACE