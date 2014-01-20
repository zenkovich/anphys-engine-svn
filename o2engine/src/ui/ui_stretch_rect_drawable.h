#ifndef UI_STRETCH_RECT_DRAWABLE_H
#define UI_STRETCH_RECT_DRAWABLE_H

#include "ui_drawable.h"

OPEN_O2_NAMESPACE

class cStretchRect;

class uiStretchRectDrawable: public uiDrawable
{
public:
	cStretchRect* mStretchRect;


	/** ctor. */
	uiStretchRectDrawable();

	/** ctor. */
	uiStretchRectDrawable(cStretchRect* stretchRect);

	/** dtor. */
	~uiStretchRectDrawable();

	/** Dummy. */
	void update(float dt) {}

	/** Draw sprite. */
	void draw();

	/** Setting the position. */
	void setPosition(const vec2f& pos);

	/** Returns the position. */
	vec2f getPosition() const;

	/** Setting the size. */
	void setSize(const vec2f& size);

	/** Returns the size. */
	vec2f getSize() const;

	/** Setting rect. */
	void setRect(const fRect& rect);

	/** Returns the rect. */
	fRect getRect() const;
};

CLOSE_O2_NAMESPACE

#endif // UI_STRETCH_RECT_DRAWABLE_H
