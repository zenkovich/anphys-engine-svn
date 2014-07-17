#include "ui_scrollarea.h"

#include "ui_scroll_bar.h"
#include "util/time_utils.h"
#include "render_system/render_system.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(uiScrollArea);

uiScrollArea::uiScrollArea( const cLayout& layout, const string& id /*= ""*/ ):
	uiDrawablesListWidget(layout, id), mHorScrollbar(NULL), mVerScrollbar(NULL)
{
}

uiScrollArea::uiScrollArea( const uiScrollArea& scrollarea ):
	uiDrawablesListWidget(scrollarea)
{
	if (scrollarea.mHorScrollbar)
		mHorScrollbar = getWidgetByType<uiScrollBar>(scrollarea.getWidgetPath(scrollarea.mHorScrollbar));
	else
		mHorScrollbar = NULL;
	
	if (scrollarea.mVerScrollbar)
		mVerScrollbar = getWidgetByType<uiScrollBar>(scrollarea.getWidgetPath(scrollarea.mVerScrollbar));
	else
		mVerScrollbar = NULL;

	mClippingLayout = scrollarea.mClippingLayout;
}

uiScrollArea::~uiScrollArea()
{
}

uiWidget* uiScrollArea::clone() const
{
	return mnew uiScrollArea(*this);
}

void uiScrollArea::draw()
{
	if (!mVisible || mDrawedAtFrame == timeUtils()->getCurrentFrame())
		return;

	mDrawedAtFrame = timeUtils()->getCurrentFrame();

	localDraw();

	renderSystem()->setupScissorRect(mClippingLayout.getRect());
	renderSystem()->enableScissorTest();

	FOREACH(WidgetsVec, mChildWidgets, it)
		(*it)->draw();

	renderSystem()->disableScissorTest();
}

bool uiScrollArea::isFocusable() const
{

}

void uiScrollArea::updateLayout()
{

}

void uiScrollArea::localDraw()
{

}

void uiScrollArea::localUpdate( float dt )
{

}

void uiScrollArea::localUpdateLayout()
{

}

void uiScrollArea::layoutUpdated()
{

}

bool uiScrollArea::localProcessInputMessage( const cInputMessage& msg )
{

}

void uiScrollArea::onFocused()
{

}

void uiScrollArea::onFocusLost()
{

}

void uiScrollArea::initializeProperties()
{

}


CLOSE_O2_NAMESPACE