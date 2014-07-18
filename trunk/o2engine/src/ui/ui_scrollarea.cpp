#include "ui_scrollarea.h"

#include "ui_scroll_bar.h"
#include "util/time_utils.h"
#include "render_system/render_system.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(uiScrollArea);

uiScrollArea::uiScrollArea( const cLayout& layout, uiScrollBar* horBarSample = NULL, uiScrollBar* verBarSample = NULL, 
	                        const string& id /*= ""*/ ):
	uiDrawablesListWidget(layout, id), mHorScrollbar(NULL), mVerScrollbar(NULL)
{
	setHorScrollbarSample(horBarSample);
	setVerScrollbarSample(verBarSample);
}

uiScrollArea::uiScrollArea( const uiScrollArea& scrollarea ):
	uiDrawablesListWidget(scrollarea)
{
	setHorScrollbarSample(scrollarea.mHorScrollbar);
	setVerScrollbarSample(scrollarea.mVerScrollbar);

	mClippingLayout = scrollarea.mClippingLayout;
}

uiScrollArea::~uiScrollArea()
{
	safe_release(mHorScrollbar);
	safe_release(mVerScrollbar);
}

uiWidget* uiScrollArea::clone() const
{
	return mnew uiScrollArea(*this);
}

void uiScrollArea::setHorScrollbarSample( uiScrollBar* scrollbarSample )
{
	safe_release(mHorScrollbar);

	if (scrollbarSample)
		mHorScrollbar = mnew uiScrollBar(*scrollbarSample);
}

void uiScrollArea::setVerScrollbarSample( uiScrollBar* scrollbarSample )
{
	safe_release(mVerScrollbar);

	if (scrollbarSample)
		mVerScrollbar = mnew uiScrollBar(*scrollbarSample);
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

	if (mHorScrollbar)
		mHorScrollbar->draw();

	if (mVerScrollbar)
		mVerScrollbar->draw();
}

bool uiScrollArea::isFocusable() const
{
	return true;
}

void uiScrollArea::localUpdate( float dt )
{
	if (mHorScrollbar)
		mHorScrollbar->update(dt);
	
	if (mVerScrollbar)
		mVerScrollbar->update(dt);
}

void uiScrollArea::layoutUpdated()
{
	mClippingLayout.update(mGlobalPosition, mSize);
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