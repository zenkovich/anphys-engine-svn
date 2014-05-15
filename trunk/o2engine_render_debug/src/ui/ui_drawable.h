#ifndef UI_DRAWABLE_H
#define UI_DRAWABLE_H

#include "public.h"

#include "util/objects.h"
#include "util/math/vector2.h"
#include "util/math/rect.h"

OPEN_O2_NAMESPACE

class uiDrawable:public IDynamicDrawable
{
public:	
	/** Setting the position. */
	virtual void setPosition(const vec2f& pos) = 0;

	/** Returns the position. */
	virtual vec2f getPosition() const = 0;

	/** Setting the size. */
	virtual void setSize(const vec2f& size) = 0;

	/** Returns the size. */
	virtual vec2f getSize() const = 0;

	/** Setting rect. */
	virtual void setRect(const fRect& rect) = 0;

	/** Returns the rect. */
	virtual fRect getRect() const = 0;
};

CLOSE_O2_NAMESPACE

#endif // UI_DRAWABLE_H
