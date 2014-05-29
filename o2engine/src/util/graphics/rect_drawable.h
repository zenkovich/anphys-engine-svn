#ifndef RECT_DRAWABLE_H
#define RECT_DRAWABLE_H

#include "public.h"
#include "util/objects.h"
#include "util/property.h"

OPEN_O2_NAMESPACE

/** Basic rect drawable interface. It could be drawed, determinates by position, size, pivot and color. 
 ** pivot is in local space, in pixels. */
class IRectDrawable: public virtual IDrawable
{
protected:
	vec2f  mPosition; /** Position. */
	vec2f  mSize;     /** Size. */
	vec2f  mPivot;    /** Pivot, in local space, in pixels. */
	color4 mColor;    /** Color. */

public:
	PROPERTY(IRectDrawable, vec2f)  position;     /** Position property. Using set/getPosition. */
	PROPERTY(IRectDrawable, vec2f)  size;         /** Size property. Using set/getSize. */
	PROPERTY(IRectDrawable, vec2f)  pivot;        /** Pivot property, in local pixel space. Using set/getPivot. */
	PROPERTY(IRectDrawable, vec2f)  relPivot;     /** Relative pivot property, in local scale, 
												    * where (0, 0) - left top corner, (1, 1) - right down. 
												    * Using set/getRelativePivot. */
	PROPERTY(IRectDrawable, fRect)  rect;         /** Rect property. Sets the position and size. Using set/getRect. */
	PROPERTY(IRectDrawable, float)  transparency; /** Transparency property, changing alpha in color. Using set/getTransparency. */
	PROPERTY(IRectDrawable, color4) color;        /** Color property. Using set/getProperty. */

	/** ctor. */
	IRectDrawable(const vec2f& size = vec2f(), const vec2f& position = vec2f(), const color4& color = color4::white(), 
		          const vec2f& pivot = vec2f());

	/** copy-ctor. */
	IRectDrawable(const IRectDrawable& drawable);

	/** dtor. */
	virtual ~IRectDrawable() {}

	/** Drawing. */
	virtual void draw() {}

	/** Sets position. */
	virtual void setPosition(const vec2f& position);

	/** Returns position. */
	virtual vec2f getPosition() const;

	/** Sets size. */
	virtual void setSize(const vec2f& size);

	/** Return size. */
	virtual vec2f getSize() const;

	/** Sets pivot, in local pixel space. */
	virtual void setPivot(const vec2f& pivot);

	/** Return pivot, in local pixel space. */
	virtual vec2f getPivot() const;

	/** Sets relative pivot, in local space, where (0, 0) - left top corner, (1, 1) - right down. */
	virtual void setRelativePivot(const vec2f& relPivot);

	/** Returns relative pivot, in local space, where (0, 0) - left top corner, (1, 1) - right down. */
	virtual vec2f getRelativePivot() const;

	/** Sets rect. */
	virtual void setRect(const fRect& rect);

	/** Returns rect. */
	virtual fRect getRect() const;

	/** Sets color. */
	virtual void setColor(const color4& color);

	/** Returns color. */
	virtual color4 getColor() const;

	/** Sets transparency. Changing color alpha. */
	virtual void setTransparency(float transparency);

	/** Returns transparency(color alpha). */
	virtual float getTransparency() const;

protected:
	/** Calls when position was changed. */
	virtual void positionChanged() {}

	/** Calls when size was changed. */
	virtual void sizeChanged() {}

	/** Calls when pivot was changed. */
	virtual void pivotChanged() {}

	/** Calls when color was changed. */
	virtual void colorChanged() {}

	/** Initializing properties. */
	void initializeProperties();
};

CLOSE_O2_NAMESPACE

#endif //RECT_DRAWABLE_H
