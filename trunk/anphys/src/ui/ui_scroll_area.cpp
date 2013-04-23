#include "ui_scroll_area.h"

#include "ui_scrollbar.h"

REGIST_TYPE(uiScrollArea)

uiScrollArea::uiScrollArea(uiWidgetsManager* widgetsManager, const std::string& id, const vec2& size):
	uiWidget(widgetsManager, id)
{
	setSize(size);
	mViewArea = size;

	mHorScrollbar = NULL;
	mVerScrollbar = NULL;

	mLastHorScrolling = mLastVerScrolling = 0.0f;

	mContentWidget = new uiWidget(widgetsManager, "content");

	uiWidget::addChild(mContentWidget);
	mContentWidget->setClipping(true);
}

uiScrollArea::uiScrollArea( const uiScrollArea& scrollarea ):
	uiWidget(scrollarea)
{
	mViewArea = scrollarea.mViewArea;
	setSize(scrollarea.mSize);

	mContentWidget = NULL;
	mHorScrollbar = NULL;
	mVerScrollbar = NULL;

	for (WidgetsList::iterator it = mChilds.begin(); it != mChilds.end(); ++it)
	{
		if ((*it)->mId == "content" && (*it)->getType() == uiWidget::getStaticType())
			mContentWidget = *it;

		if ((*it)->getType() == uiScrollbar::getStaticType())
		{
			if (static_cast<uiScrollbar*>(*it)->mType == uiScrollbar::ST_HORISONTAL)
				mHorScrollbar = static_cast<uiScrollbar*>(*it);
			else 
				mVerScrollbar = static_cast<uiScrollbar*>(*it);
		}
	}
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
	mContentWidget->addChild(widget);

	recalculateScrollingArea();
}

void uiScrollArea::removeChild( uiWidget* widget )
{
	mContentWidget->removeChild(widget);

	recalculateScrollingArea();
}

void uiScrollArea::removeAllChilds()
{
	mContentWidget->removeAllChilds();

	recalculateScrollingArea();
}

void uiScrollArea::horScrollbarChanged( float value )
{
	float diff = value - mLastHorScrolling;
	vec2 vdiff(-diff, 0.0f);

	//mContentWidget->setPosition(mContentWidget->getPosition() + vdiff);
	for (WidgetsList::iterator it = mContentWidget->mChilds.begin(); it != mContentWidget->mChilds.end(); ++it)
	{
		(*it)->setPosition((*it)->getPosition() + vdiff);
	}

	mLastHorScrolling = value;
}

void uiScrollArea::verScrollbarChanged( float value )
{
	float diff = value - mLastVerScrolling;
	vec2 vdiff(0.0f, -diff);

	for (WidgetsList::iterator it = mContentWidget->mChilds.begin(); it != mContentWidget->mChilds.end(); ++it)
	{
		(*it)->setPosition((*it)->getPosition() + vdiff);
	}

	mLastVerScrolling = value;
}

void uiScrollArea::setHorScrollbar( uiScrollbar* scrollbar )
{
	if (mHorScrollbar)
		uiWidget::removeChild(scrollbar);

	mHorScrollbar = scrollbar;
	if (mHorScrollbar)
	{
		uiWidget::addChild(mHorScrollbar);
		mLastHorScrolling = mHorScrollbar->mCurrentValue;

		mHorScrollbar->setChangeValueCallback(
			new cCallback1Param< float, uiScrollArea >(this, &uiScrollArea::horScrollbarChanged));
	}

	recalculateScrollingArea();
}

void uiScrollArea::setVerScrollbar( uiScrollbar* scrollbar )
{
	if (mVerScrollbar)
		uiWidget::removeChild(scrollbar);

	mVerScrollbar = scrollbar;
	if (mVerScrollbar)
	{
		uiWidget::addChild(mVerScrollbar);
		mLastVerScrolling = mVerScrollbar->mCurrentValue;

		mVerScrollbar->setChangeValueCallback(
			new cCallback1Param< float, uiScrollArea >(this, &uiScrollArea::verScrollbarChanged));
	}

	recalculateScrollingArea();
}

void uiScrollArea::recalculateScrollingArea()
{
	mViewArea = mSize;

	mScrollingArea = mContentWidget->getSize();

	bool enableHorScrollbar = mScrollingArea.x > mViewArea.x;
	bool enableVerScrollbar = mScrollingArea.y > mViewArea.y;

	if (enableHorScrollbar)
		mViewArea.y -= mHorScrollbar->getSize().y;

	if (enableVerScrollbar)
		mViewArea.x -= mVerScrollbar->getSize().x;

	if (mHorScrollbar)
	{
		if (enableHorScrollbar)
		{
			mHorScrollbar->setPosition(vec2(0.0f, mSize.y - mHorScrollbar->getSize().y));
			mHorScrollbar->setSize(vec2(mViewArea.x, mHorScrollbar->getSize().y));
			mHorScrollbar->show();

			mHorScrollbar->mMinValue = 0.0f;
			mHorScrollbar->mMaxValue = mScrollingArea.x - mViewArea.x;
			mHorScrollbar->setScrollerSize((mHorScrollbar->mMaxValue - mHorScrollbar->mMinValue)*
				mViewArea.x/mScrollingArea.x);
		}
		else
		{
			mHorScrollbar->hide();
		}
	}

	if (mVerScrollbar)
	{
		if (enableVerScrollbar)
		{
			mVerScrollbar->setPosition(vec2(mSize.x - mVerScrollbar->getSize().x, 0.0f));
			mVerScrollbar->setSize(vec2(mVerScrollbar->getSize().x, mViewArea.y));
			mVerScrollbar->show();			

			mVerScrollbar->mMinValue = 0.0f;
			mVerScrollbar->mMaxValue = mScrollingArea.y - mViewArea.y;
			mVerScrollbar->setScrollerSize((mVerScrollbar->mMaxValue - mVerScrollbar->mMinValue)*
				mViewArea.y/mScrollingArea.y);
		}
		else
		{
			mVerScrollbar->hide();
		}
	}

	mContentWidget->setSize(mViewArea);
}
