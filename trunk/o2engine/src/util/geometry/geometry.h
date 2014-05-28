#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "public.h"
#include "util/math/rect.h"
#include "util/math/vector2.h"
#include "util/smart_ptrs.h"

OPEN_O2_NAMESPACE
	
/** Geometry interface. Processing point colliding, AABB, positioning. */
class cGeometry: public ShareObj
{
public: 
	/** Returns true, if point inside. */
	virtual bool isInside(const vec2f& point) const = 0;

	/** Returns AABB of that geometry. */
	virtual fRect getAABB() const = 0;

	/** Returns position of that geometry. */
	virtual vec2f getPosition() const = 0;

	/** Sets the position. */
	virtual void setPosition(const vec2f& pos) = 0;
};

/** Rectangle geometry, aligned by axis. */
class cRectGeometry:public cGeometry
{
	fRect mRect;

public:
	/** ctor. */
	cRectGeometry();

	/** ctor. */
	cRectGeometry(const fRect& rect);

	/** ctor. */
	cRectGeometry(float left, float top, float right, float down);

	/** ctor. */
	cRectGeometry(const vec2f& minp, const vec2f& maxp);
	
	/** Returns true, if point inside. */
	bool isInside(const vec2f& point) const;
	
	/** Returns AABB of that geometry. */
	fRect getAABB() const;
	
	/** Returns position of that geometry. */
	vec2f getPosition() const;
	
	/** Sets the position. */
	void setPosition(const vec2f& pos);

	/** Sets the size of rect. */
	void setSize(const vec2f& size);

	/** Sets rectangle. */
	void set(const fRect& rect);

	/** Sets rectangle by parameters. */
	void set(const vec2f& minp, const vec2f& maxp);
};

/** Circle geometry. Determined by center position and radius. */
class cCircleGeometry: public cGeometry
{
	vec2f mCenter;    /**< Center position. */
	float mRadius;    /**< Radius of circle. */
	float mSqrRadius; /**< Square of radius: mRadius*mRadius. */

public:
	/** ctor. */
	cCircleGeometry();

	/** ctor. */
	cCircleGeometry(const vec2f& center, float radius);

	/** Returns true, if point inside. */
	bool isInside(const vec2f& point) const;
	
	/** Returns AABB of that geometry. */
	fRect getAABB() const;
	
	/** Returns position of that geometry. */
	vec2f getPosition() const;
	
	/** Sets the position. */
	void setPosition(const vec2f& pos);

	/** Sets by parameters. */
	void set(const vec2f& center, float radius);
};

class cGroupGeometry:public cGeometry
{
	friend class cGeometry;

public:
	typedef vector< shared<cGeometry> > PartsVec;

protected:
	PartsVec mParts;
	fRect    mAABB;
	vec2f    mPosition;

public:
	cGroupGeometry();
	~cGroupGeometry();

	template<typename T>
	T& getPart(int idx) { return *((T*)mParts[idx]); }

	int getPartsCount() const;
	
	int addPart(shared<cGeometry> geom);
	void removePart(int idx);
	void removeAllParts();

	void setPosition(const vec2f& pos);
	vec2f getPosition() const;

	bool isInside(const vec2f& point) const;

	void updateAABB();
};

CLOSE_O2_NAMESPACE

#endif // GEOMETRY_H
