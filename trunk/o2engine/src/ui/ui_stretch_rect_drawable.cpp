#include "ui_stretch_rect_drawable.h"

#include "util/graphics/stretch_rect.h"

OPEN_O2_NAMESPACE

uiStretchRectDrawable::uiStretchRectDrawable():
	mStretchRect(NULL)
{
}

uiStretchRectDrawable::uiStretchRectDrawable( shared<cStretchRect> stretchRect ):
	mStretchRect(stretchRect)
{
}

uiStretchRectDrawable::~uiStretchRectDrawable()
{
	safe_release(mStretchRect);
}

void uiStretchRectDrawable::draw()
{
	mStretchRect->draw();
}

void uiStretchRectDrawable::setPosition( const vec2f& pos )
{
	mStretchRect->setPosition(pos);
}

vec2f uiStretchRectDrawable::getPosition() const
{
	return mStretchRect->getPosition();
}

void uiStretchRectDrawable::setSize( const vec2f& size )
{
	mStretchRect->setSize(size);
}

vec2f uiStretchRectDrawable::getSize() const
{
	return mStretchRect->getSize();
}

void uiStretchRectDrawable::setRect( const fRect& rect )
{
	mStretchRect->setRect(rect);
}

fRect uiStretchRectDrawable::getRect() const
{
	return mStretchRect->getRect();
}

CLOSE_O2_NAMESPACE