#include "ui_rect.h"

OPEN_O2_NAMESPACE

uiRect::uiRect(const uiWidgetLayout& layout, const string& id /*= ""*/, shared<uiWidget> parent /*= NULL*/):
	uiWidget(layout, id, parent)
{
	transparency.onChangeEvent.add(callback<uiRect>( tempShared<uiRect>(this), &uiRect::transparencyChanged));
}

uiRect::uiRect(const uiRect& rectWidget):
	uiWidget(rectWidget)
{
	mStretchRect = rectWidget.mStretchRect;	
	transparency.onChangeEvent.add(callback<uiRect>( tempShared<uiRect>(this), &uiRect::transparencyChanged));
}

uiRect::~uiRect()
{
}

shared<uiWidget> uiRect::clone() const
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
	mStretchRect.setTransparency(mTransparency);
}

CLOSE_O2_NAMESPACE