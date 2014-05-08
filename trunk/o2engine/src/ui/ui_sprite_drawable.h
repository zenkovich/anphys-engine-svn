#ifndef UI_SPRITE_DRAWABLE_H
#define UI_SPRITE_DRAWABLE_H

#include "ui_drawable.h"

OPEN_O2_NAMESPACE

class grSprite;

class uiSpriteDrawable:public uiDrawable
{
	shared(grSprite) mSprite;    /**< Sprite. */
	bool             mOwnSprite; /**< True, if sprite contains here and can be removed. */

public:
	/** ctor. */
	uiSpriteDrawable();

	/** ctor. 
	 ** @param sprite - sprite ptr
	 ** @param ownSprite - is sprite own and can be removed */
	uiSpriteDrawable(shared(grSprite) sprite, bool ownSprite = true);

	/** dtor. */
	~uiSpriteDrawable();

	/** Dummy. */
	void update(float dt) {}

	/** Draw sprite. */
	void draw();

	/** Set sprite. */
	void setSprite(shared(grSprite) sprite, bool ownSprite = true);

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

#endif // UI_SPRITE_DRAWABLE_H
