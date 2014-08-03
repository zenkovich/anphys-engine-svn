#include "ui_rect.h"

OPEN_O2_NAMESPACE

uiRect::uiRect(const cLayout& layout, const string& id /*= ""*/):
	uiWidget(layout, id)
{
	transparency.onChangeEvent.add(callback<uiRect>( this, &uiRect::transparencyChanged));
}

uiRect::uiRect(const uiRect& rectWidget):
	uiWidget(rectWidget)
{
	mStretchRect = rectWidget.mStretchRect;	
	mResTransparencyChanged.add(callback<uiRect>( this, &uiRect::transparencyChanged));
}

uiRect::~uiRect()
{
}

uiWidget* uiRect::clone() const
{
	return mnew uiRect(*this);
}

void uiRect::localDraw()
{
	mStretchRect.draw();
}

void uiRect::layoutUpdated()
{
	mStretchRect.setRect(mBounds);
}

void uiRect::transparencyChanged()
{
	mStretchRect.setTransparency(mResTransparency);
}

CLOSE_O2_NAMESPACE