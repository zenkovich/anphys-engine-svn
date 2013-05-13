#include "ui_scrollbar.h"

#include "render/render_objects/2d/sprite.h"
#include "ui_state.h"
#include "util/other/callback.h"
#include "ui_manager.h"

REGIST_TYPE(uiScrollbar)

uiScrollbar::uiScrollbar(uiWidgetsManager* widgetsManager, const std::string& id, ScrollbarType type, 
	                     const vec2& size, uiWidget* bkWidget, uiWidget* scrollerWidget, 
	                     float minValue /*= 0.0f*/, float maxValue /*= 1.0f*/, float currentValue /*= 0.0f*/, 
						 float scollSize /*= -1*/):
	uiWidget(widgetsManager, id), mType(type), mBackWidget(bkWidget), mScrollerWidget(scrollerWidget),
	mMinValue(minValue), mMaxValue(maxValue), mCurrentValue(currentValue), mScorllerSize(scollSize),
	mOnChangeValueCallback(NULL)
{
	mSize = size;
	mSelectedState = new uiState(this, "");
	mPressedState = new uiState(this, "");

	mPressed = mSelected = false;

	addChild(mBackWidget);
	addChild(mScrollerWidget);

	updateGraphics();
}
	
uiScrollbar::uiScrollbar( const uiScrollbar& scrollbar ):uiWidget(scrollbar)
{
	mType           = scrollbar.mType;
	mSize           = scrollbar.mSize;
	mMinValue       = scrollbar.mMinValue;
	mMaxValue       = scrollbar.mMaxValue;
	mScorllerSize   = scrollbar.mScorllerSize;
	mSelectedState  = new uiState(*scrollbar.mSelectedState);
	mPressedState   = new uiState(*scrollbar.mPressedState);

	mPressed = mSelected = false;

	updateGraphics();
}

uiScrollbar::~uiScrollbar()
{
	safe_release(mBackWidget);
	safe_release(mScrollerWidget);
	safe_release(mSelectedState);
	safe_release(mPressedState);
	safe_release(mOnChangeValueCallback);
}

void uiScrollbar::derivedUpdate( float dt )
{
	if (fabs(mGlobalPosition.x - mLastGlobalPosition.x) > FLT_EPSILON ||
		fabs(mGlobalPosition.y - mLastGlobalPosition.y) > FLT_EPSILON ||
		fabs(mResSize.x - mLastSize.x) > FLT_EPSILON ||
		fabs(mResSize.y - mLastSize.y) > FLT_EPSILON || checkBindedValues())
	{
		updateGraphics();
	}

	/*mBackWidget->update(dt);
	mScrollerWidget->update(dt);*/
}

void uiScrollbar::derivedDraw()
{
	/*mBackWidget->draw();
	mScrollerWidget->draw();*/
}

int uiScrollbar::processInputMessageDerived( const cInputMessage& message )
{
	int res = 0;

	if (!mVisible)
		return res;

	if (!mPressed && !message.isKeyDown(CURSOR_BUTTON))
	{
		if (mSelected && !isPointInScroller(message.mCursorPosition))
		{
			mSelected = false;
			mSelectedState->deactivate();
		}
		if (!mSelected && isPointInScroller(message.mCursorPosition))
		{
			mSelected = true;
			mSelectedState->activate();

			res = 1;
		}
	}
	if (message.isKeyPressed(CURSOR_BUTTON) && isPointInScroller(message.mCursorPosition))
	{
		mPressed = true;
		mSelected = false;
		mSelectedState->deactivate();
		mPressedState->activate();

		mWidgetsManager->setWidgetFocused(this);

		pressed(message.mCursorPosition);

		res = 1;
	}
	if (message.isKeyReleased(CURSOR_BUTTON) && mPressed)
	{
		mPressed = false;
		mSelected = false;

		mWidgetsManager->unfocusWidget(this);

		mSelectedState->deactivate();
		mPressedState->deactivate();
	}

	if (mPressed)
	{
		if (fabs(message.mCursorPosition.x - mLastCursorPos.x) > 1.0f ||
			fabs(message.mCursorPosition.y - mLastCursorPos.y) > 1.0f)
		{
			mouseMoved(message.mCursorPosition);
		}

		res = 1;
	}

	return res;
}

void uiScrollbar::updateGraphics()
{
	mLastGlobalPosition = mGlobalPosition;
	mLastSize = mResSize;

	mBackWidget->setSize(mResSize);;

	vec2 scrollerSize;
	vec2 scrollerPos;

	if (mType == ST_HORISONTAL)
	{
		scrollerSize.y = mResSize.y;

		if (mScorllerSize < 0)
		{
			scrollerSize.x = (mCurrentValue - mMinValue)/(mMaxValue - mMinValue)*mResSize.x;
			//scrollerPos = mGlobalPosition;
		}
		else
		{
			const float minScrollerSize = 5.0f;
			scrollerSize.x = fmax(mResSize.x*(mScorllerSize/(mMaxValue - mMinValue)), minScrollerSize);

			//scrollerPos.y = /*mGlobalPosition.y*/0;
			scrollerPos.x = (mCurrentValue - mMinValue)/(mMaxValue-mMinValue)*(mResSize.x - scrollerSize.x);
		}
	}
	else
	{
		scrollerSize.x = mResSize.x;

		if (mScorllerSize < 0)
		{
			scrollerSize.y = (mCurrentValue - mMinValue)/(mMaxValue - mMinValue)*mResSize.y;
			//scrollerPos = mGlobalPosition;
		}
		else
		{
			const float minScrollerSize = 5.0f;
			scrollerSize.y = fmax(mResSize.y*(mScorllerSize/(mMaxValue - mMinValue)), minScrollerSize);

			//scrollerPos.x = mGlobalPosition.x;
			scrollerPos.y = (mCurrentValue - mMinValue)/(mMaxValue-mMinValue)*(mResSize.y - scrollerSize.y);
		}
	}

	mScrollerWidget->setSize(scrollerSize)->setPosition(scrollerPos);
}

bool uiScrollbar::isPointInScroller( const vec2& point )
{
	vec2 scrollerPos;
	vec2 scrollerSize;

	if (mScorllerSize < 0)
	{
		scrollerSize = mResSize;
		scrollerPos = mGlobalPosition;
	}
	else
	{
		scrollerSize = mScrollerWidget->getSize();
		scrollerPos = mScrollerWidget->getGlobalPosition();
	}

	if (point.x < scrollerPos.x || point.y < scrollerPos.y ||
		point.x > scrollerPos.x + scrollerSize.x || point.y > scrollerPos.y + scrollerSize.y)
		return false;

	return true;
}

void uiScrollbar::pressed( const vec2& point )
{
	mLastCursorPos = point;

	mouseMoved(point);
}

void uiScrollbar::mouseMoved( const vec2& point )
{
	if (mScorllerSize < 0)
	{
		if (mType == ST_HORISONTAL)
			mCurrentValue = (point.x - mGlobalPosition.x)/mResSize.x*(mMaxValue - mMinValue) + mMinValue;
		else
			mCurrentValue = (point.y - mGlobalPosition.y)/mResSize.y*(mMaxValue - mMinValue) + mMinValue;
	}
	else
	{
		if (mType == ST_HORISONTAL)
			mCurrentValue += (point.x - mLastCursorPos.x)/(mResSize.x - mResSize.x*mScorllerSize/(mMaxValue - mMinValue))*
			                 (mMaxValue - mMinValue);
		else
		    mCurrentValue += (point.y - mLastCursorPos.y)/(mResSize.y - mResSize.y*mScorllerSize/(mMaxValue - mMinValue))*
		                     (mMaxValue - mMinValue);
	}

	mCurrentValue = fclamp(mCurrentValue, mMinValue, mMaxValue);

	if (mOnChangeValueCallback)
	{
		static_cast< cCallback1Param<float>* >(mOnChangeValueCallback)->mArg = mCurrentValue;
		mOnChangeValueCallback->call();
	}

	mLastCursorPos = point;
	
	setBindingValue(&mCurrentValue, BindValuePrototype::t_float);

	updateGraphics();
}

uiScrollbar* uiScrollbar::setScrollerSize( float size )
{
	mScorllerSize = size;
	updateGraphics();
	return this;
}

float uiScrollbar::getScrollerSize() const
{
	return mScorllerSize;
}

void uiScrollbar::setChangeValueCallback( cCallbackInterface* callback )
{
	safe_release(mOnChangeValueCallback);

	mOnChangeValueCallback = callback;
}

bool uiScrollbar::checkBindedValues()
{
	bool changedValue = false;
	for (BindValuesList::iterator it = mBindValues.begin(); it != mBindValues.end(); ++it)
	{
		if ((*it)->checkValue())
		{
			float newValue;
			(*it)->getValue(&newValue, BindValuePrototype::t_float);

			mCurrentValue = newValue;
			changedValue = true;
		}
	}	
	
	return changedValue;
}
