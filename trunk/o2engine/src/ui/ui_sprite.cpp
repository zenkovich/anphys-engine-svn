#include "ui_sprite.h"

OPEN_O2_NAMESPACE

uiSprite::uiSprite(const layout& layout, const string& id /*= ""*/):
	uiWidget(layout, id)
{
	layoutUpdated();

	mResTransparencyChanged.add(callback<uiSprite>( this, &uiSprite::transparencyChanged));
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

uiWidget* uiSprite::clone() const
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
	mSprite.setTransparency(mResTransparency);
}

CLOSE_O2_NAMESPACE