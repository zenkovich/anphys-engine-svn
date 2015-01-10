#include "ui_scrollarea.h"

#include "ui_scroll_bar.h"
#include "util/time_utils.h"
#include "render_system/render_system.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(uiScrollArea);

uiScrollArea::uiScrollArea( const layout& layout, uiScrollBar* horBarSample /*= NULL*/, 
	                        uiScrollBar* verBarSample /*= NULL*/, const string& id /*= ""*/ ):
	uiDrawablesListWidget(layout, id), mHorScrollbar(NULL), mVerScrollbar(NULL)
{
	setHorScrollbar(horBarSample);
	setVerScrollbar(verBarSample);

	initializeProperties();
}

uiScrollArea::uiScrollArea( const uiScrollArea& scrollarea ):
	uiDrawablesListWidget(scrollarea), mHorScrollbar(NULL), mVerScrollbar(NULL)
{
	if (scrollarea.mHorScrollbar)
		setHorScrollbar(getWidgetByType<uiScrollBar>(scrollarea.mHorScrollbar->getId()));
	else 
		setHorScrollbar(NULL);

	if (scrollarea.mVerScrollbar)
		setVerScrollbar(getWidgetByType<uiScrollBar>(scrollarea.mVerScrollbar->getId()));
	else
		setVerScrollbar(NULL);

	mClippingLayout = scrollarea.mClippingLayout;

	initializeProperties();
}

uiScrollArea::~uiScrollArea()
{
}

uiWidget* uiScrollArea::clone() const
{
	return mnew uiScrollArea(*this);
}

void uiScrollArea::setHorScrollbar( uiScrollBar* scrollbar )
{
	if (mHorScrollbar)
		removeChild(mHorScrollbar);

	if (!scrollbar)
		return;

	addChild(scrollbar);
	mHorScrollbar = scrollbar;
	mHorScrollbar->onValueChangedEvent.add(callback<uiScrollArea>(this, &uiScrollArea::scrollChanged));
}

void uiScrollArea::setVerScrollbar( uiScrollBar* scrollbar )
{
	if (mVerScrollbar)
		removeChild(mVerScrollbar);

	if (!scrollbar)
		return;
	
	addChild(scrollbar);
	mVerScrollbar = scrollbar;
	mVerScrollbar->onValueChangedEvent.add(callback<uiScrollArea>(this, &uiScrollArea::scrollChanged));
}

void uiScrollArea::draw()
{
	if (!mVisible || mDrawedAtFrame == timeUtils()->getCurrentFrame())
		return;

	mDrawedAtFrame = timeUtils()->getCurrentFrame();

	localDraw();

	if (mHorScrollbar)
		mHorScrollbar->mDrawedAtFrame = mDrawedAtFrame;

	if (mVerScrollbar)
		mVerScrollbar->mDrawedAtFrame = mDrawedAtFrame;

	renderSystem()->setupScissorRect(mClippingLayout.getRect());
	renderSystem()->enableScissorTest();

	FOREACH(WidgetsVec, mChildWidgets, it)
		(*it)->draw();

	renderSystem()->disableScissorTest();

	if (mHorScrollbar)
	{
		mHorScrollbar->mDrawedAtFrame = mDrawedAtFrame - 1;
		mHorScrollbar->draw();
	}

	if (mVerScrollbar)
	{
		mVerScrollbar->mDrawedAtFrame = mDrawedAtFrame - 1;
		mVerScrollbar->draw();
	}
}

bool uiScrollArea::isFocusable() const
{
	return true;
}

void uiScrollArea::updateLayout()
{
	localUpdateLayout();
	mClippingLayout.update(mGlobalPosition, mSize);

	vec2f tempGlobPos = mGlobalPosition;
	mGlobalPosition = mClippingLayout.mPosition - mSmoothScroll;
	
	fRect contentSize = mClippingLayout.getRect() - mSmoothScroll;
	FOREACH(WidgetsVec, mChildWidgets, it)
	{
		(*it)->updateLayout();

		if (*it == mHorScrollbar || *it == mVerScrollbar)
			continue;

		contentSize = contentSize + (*it)->mBounds;
	}

	mGlobalPosition = tempGlobPos;

	contentSize.right += 5.0f;
	contentSize.down += 5.0f;

	if (mHorScrollbar)
	{
		mHorScrollbar->setValueRange(0.0f, max(contentSize.getSizeX() - mClippingLayout.mSize.x, 0.1f));
		mHorScrollbar->setBarSize(mClippingLayout.mSize.x/contentSize.getSizeX()*mHorScrollbar->getMaxValue());
		mHorScrollbar->updateLayout();
		mHorScrollbar->setVisible(mHorScrollbar->getBarSize() < mHorScrollbar->getMaxValue() - mHorScrollbar->getMinValue());
	}

	if (mVerScrollbar)
	{
		mVerScrollbar->setValueRange(0.0f, max(contentSize.getSizeY() - mClippingLayout.mSize.y, 0.1f));
		mVerScrollbar->setBarSize(mClippingLayout.mSize.y/contentSize.getSizeY()*mVerScrollbar->getMaxValue());
		mVerScrollbar->updateLayout();
		mVerScrollbar->setVisible(mVerScrollbar->getBarSize() < mVerScrollbar->getMaxValue() - mVerScrollbar->getMinValue());
	}
	
}

void uiScrollArea::localUpdate( float dt )
{
	float scrollingCoef = 20.0f;
	vec2f lastSmoothScroll = mSmoothScroll;
	mSmoothScroll = lerp(mSmoothScroll, mScroll, clamp(dt*scrollingCoef, 0.0f, 1.0f));

	if (!equals(lastSmoothScroll, mSmoothScroll)) {
		updateLayout();
		onScrolled.call();
	}
}

bool uiScrollArea::localProcessInputMessage( const InputMessage& msg )
{
	bool insideClipping = mClippingLayout.getRect().isInside(msg.getCursorPos());	

	float scrollCoef = 0.1f;
	float mouseWheelDelta = msg.getMouseWheelDelta()*scrollCoef;
	if (!equals(mouseWheelDelta, 0.0f))
	{
		if (mVerScrollbar)
			mVerScrollbar->setValueClamped(mVerScrollbar->getValue() - mouseWheelDelta);
		else if (mHorScrollbar)
			mHorScrollbar->setValueClamped(mHorScrollbar->getValue() - mouseWheelDelta);
	}

	if (insideClipping)
		return false;

	if (mHorScrollbar)
		mHorScrollbar->processInputMessage(msg);	

	if (mVerScrollbar)
		mVerScrollbar->processInputMessage(msg);

	return true;
}

void uiScrollArea::onFocused()
{
	uiDrawablesListWidget::onFocused();
}

void uiScrollArea::onFocusLost()
{
	uiDrawablesListWidget::onFocusLost();
}

void uiScrollArea::setScroll( const vec2f& scroll )
{
	mScroll = scroll;
}

vec2f uiScrollArea::getScroll() const
{
	return mScroll;
}

void uiScrollArea::scrollChanged()
{
	if (mHorScrollbar)
		mScroll.x = mHorScrollbar->getValue();

	if (mVerScrollbar)
		mScroll.y = mVerScrollbar->getValue();

	updateLayout();
}

void uiScrollArea::initializeProperties()
{
	REG_PROPERTY(uiScrollArea, scroll, setScroll, getScroll);
}


CLOSE_O2_NAMESPACE