#include "ui_sprite.h"

OPEN_O2_NAMESPACE

uiSprite::uiSprite(const uiWidgetLayout& layout, const string& id /*= ""*/, shared<uiWidget> parent /*= NULL*/):
	uiWidget(layout, id, parent), mLastTransparency(-1.0f)
{
	layoutUpdated();
}

uiSprite::uiSprite(const uiSprite& spriteWidget):
	uiWidget(spriteWidget), mLastTransparency(-1.0f)
{
	mSprite = spriteWidget.mSprite;
	layoutUpdated();
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
	if (!equals(mTransparency, mLastTransparency))
	{
		color4 spriteColor = mSprite.getColor();
		spriteColor.a = (int)(mTransparency*255.0f);
		mLastTransparency = mTransparency;
		mSprite.setColor(spriteColor);
	}

	mSprite.draw();
}

void uiSprite::layoutUpdated()
{
	mSprite.setPosition(mGlobalPosition);
	mSprite.setSize(mSize); 
}

CLOSE_O2_NAMESPACE