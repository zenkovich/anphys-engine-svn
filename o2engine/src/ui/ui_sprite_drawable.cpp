#include "ui_sprite_drawable.h"

#include "render_system/sprite.h"

OPEN_O2_NAMESPACE

uiSpriteDrawable::uiSpriteDrawable():
	mSprite(NULL), mOwnSprite(false)
{
}

uiSpriteDrawable::uiSpriteDrawable( grSprite* sprite, bool ownSprite /*= true*/ ):
	mSprite(sprite), mOwnSprite(ownSprite)
{
}

uiSpriteDrawable::~uiSpriteDrawable()
{
	if (mOwnSprite)
		safe_release(mSprite);
}

void uiSpriteDrawable::draw()
{
	mSprite->draw();
}

void uiSpriteDrawable::setPosition( const vec2f& pos )
{
	mSprite->setPosition(pos);
}

vec2f uiSpriteDrawable::getPosition() const
{
	return mSprite->getPosition();
}

void uiSpriteDrawable::setSize( const vec2f& size )
{
	mSprite->setSize(size);
}

vec2f uiSpriteDrawable::getSize() const
{
	return mSprite->getSize();
}

void uiSpriteDrawable::setRect( const fRect& rect )
{
	mSprite->setPosition(rect.getltCorner()).setSize(rect.getSize());
}

fRect uiSpriteDrawable::getRect() const
{
	return fRect(mSprite->getPosition(), mSprite->getPosition() + mSprite->getSize());
}

CLOSE_O2_NAMESPACE