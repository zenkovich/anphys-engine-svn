#include "ui_sprite.h"

OPEN_O2_NAMESPACE

uiSprite::uiSprite(const cLayout& layout, const string& id /*= ""*/, shared<uiWidget> parent /*= NULL*/):
	uiWidget(layout, id, parent)
{
	layoutUpdated();

	transparency.onChangeEvent.add(callback<uiSprite>( this, &uiSprite::transparencyChanged));
}

uiSprite::uiSprite(const uiSprite& spriteWidget):
	uiWidget(spriteWidget)
{
	mSprite = spriteWidget.mSprite;
	layoutUpdated();
	
	transparency.onChangeEvent.add(callback<uiSprite>( this, &uiSprite::transparencyChanged));
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

void uiSprite::transparencyChanged()
{
	mSprite.setTransparency(mTransparency);
}

CLOSE_O2_NAMESPACE