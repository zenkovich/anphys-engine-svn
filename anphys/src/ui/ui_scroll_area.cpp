#include "ui_scroll_area.h"

#include "ui_scrollbar.h"


uiScrollArea::uiScrollArea(uiWidgetsManager* widgetsManager, const std::string& id, const vec2& size):
	uiWidget(widgetsManager, id)
{
	setSize(size);
	mViewArea = size;

	mHorScrollbar = new uiScrollbar(
}

uiScrollArea::uiScrollArea( const uiScrollArea& scrollarea )
{

}

uiScrollArea::~uiScrollArea()
{

}

void uiScrollArea::derivedUpdate( float dt )
{

}

void uiScrollArea::derivedDraw()
{

}

void uiScrollArea::addChild( uiWidget* widget )
{

}

void uiScrollArea::removeChild( uiWidget* widget )
{

}

void uiScrollArea::removeAllChilds()
{

}

void uiScrollArea::horScrollbarChanged( float value )
{

}

void uiScrollArea::verScrollbarChanged( float value )
{

}
