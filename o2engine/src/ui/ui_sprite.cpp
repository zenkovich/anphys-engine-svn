#include "ui_sprite.h"

OPEN_O2_NAMESPACE

uiSprite::uiSprite(const uiWidgetLayout& layout, const string& id /*= ""*/, shared<uiWidget> parent /*= NULL*/):
	uiWidget(layout, id, parent)
{
	layoutUpdated();

	getProperty<float>("transparency")->mOnChange.add(
		callback<uiSprite>( tempShared<uiSprite>(this), &uiSprite::transparencyChanged));
}

uiSprite::uiSprite(const uiSprite& spriteWidget):
	uiWidget(spriteWidget)
{
	mSprite = spriteWidget.mSprite;
	layoutUpdated();

	getProperty<float>("transparency")->mOnChange.add(
		callback<uiSprite>( tempShared<uiSprite>(this), &uiSprite::transparencyChanged));
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
	color4 spriteColor = mSprite.getColor();
	spriteColor.a = (int)(mTransparency*255.0f);
	mSprite.setColor(spriteColor);
}

CLOSE_O2_NAMESPACE