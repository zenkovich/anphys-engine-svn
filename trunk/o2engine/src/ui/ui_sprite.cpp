#include "ui_sprite.h"

OPEN_O2_NAMESPACE

uiSprite::uiSprite(const uiWidgetLayout& layout, const string& id /*= ""*/, shared<uiWidget> parent /*= NULL*/):
	uiWidget(layout, id, parent)
{
}

uiSprite::uiSprite(const uiSprite& spriteWidget):
	uiWidget(spriteWidget)
{
	mSprite = spriteWidget.mSprite;
}

uiSprite::~uiSprite()
{
}

shared<uiWidget> uiSprite::clone() const
{
	return mnew uiSprite(*this);
}

void uiSprite::localDraw()
{
	mSprite.draw();
}

void uiSprite::layoutUpdated()
{
	mSprite.setPosition(mGlobalPosition);
	mSprite.setSize(mSize); 
}

CLOSE_O2_NAMESPACE