#include "ui_scrollbar.h"

#include "render/render_objects/2d/sprite.h"
#include "ui_state.h"
#include "util/other/callback.h"

REGIST_TYPE(uiScrollbar)

uiScrollbar::uiScrollbar(uiWidgetsManager* widgetsManager, const std::string& id, ScrollbarType type, 
	                     const vec2& size, grSprite* bkSprite, grSprite* scrollerSprite, 
	                     float minValue /*= 0.0f*/, float maxValue /*= 1.0f*/, float currentValue /*= 0.0f*/, 
						 float scollSize /*= -1*/):
	uiWidget(widgetsManager, id), mType(type), mBkSprite(bkSprite), mScrollerSprite(scrollerSprite),
	mMinValue(minValue), mMaxValue(maxValue), mCurrentValue(currentValue), mScorllerSize(scollSize),
	mChangeValueCallback(NULL)
{
	mSize = size;
	mSelectedState = new uiState(this, "");
	mPressedState = new uiState(this, "");

	mPressed = mSelected = false;

	mScrollerColor = scrollerSprite->getColor();

	updateGraphics();
}
	
uiScrollbar::uiScrollbar( const uiScrollbar& scrollbar ):uiWidget(scrollbar)
{
	mType           = scrollbar.mType;
	mSize           = scrollbar.mSize;
	mBkSprite       = new grSprite(*scrollbar.mBkSprite);
	mScrollerSprite = new grSprite(*scrollbar.mScrollerSprite);
	mMinValue       = scrollbar.mMinValue;
	mMaxValue       = scrollbar.mMaxValue;
	mScorllerSize   = scrollbar.mScorllerSize;
	mSelectedState  = new uiState(*scrollbar.mSelectedState);
	mPressedState   = new uiState(*scrollbar.mPressedState);

	mPressed = mSelected = false;

	mScrollerColor = mScrollerSprite->getColor();

	updateGraphics();
}

uiScrollbar::~uiScrollbar()
{
	safe_release(mBkSprite);
	safe_release(mScrollerSprite);
	safe_release(mSelectedState);
	safe_release(mPressedState);
}

void uiScrollbar::derivedUpdate( float dt )
{
	if (fabs(mGlobalPosition.x - mLastGlobalPosition.x) > FLT_EPSILON ||
		fabs(mGlobalPosition.y - mLastGlobalPosition.y) > FLT_EPSILON ||
		fabs(mResSize.x - mLastSize.x) > FLT_EPSILON ||
		fabs(mResSize.y - mLastSize.y) > FLT_EPSILON)
	{
		updateGraphics();
	}

	color4 bkSpriteColor = mBkSprite->getColor();
	bkSpriteColor.a = (int)(255.0f*mResTransparency);
	mBkSprite->setColor(bkSpriteColor);

	color4 scrollerSpriteColor = mResScrollerColor;
	scrollerSpriteColor.a = (int)(255.0f*mResTransparency);
	mScrollerSprite->setColor(scrollerSpriteColor);
}

void uiScrollbar::derivedDraw()
{
	mBkSprite->draw();
	mScrollerSprite->draw();
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

		pressed(message.mCursorPosition);

		res = 1;
	}
	if (message.isKeyReleased(CURSOR_BUTTON) && mPressed)
	{
		mPressed = false;
		mSelected = false;

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
	}

	return res;
}

void uiScrollbar::updateGraphics()
{
	mLastGlobalPosition = mGlobalPosition;
	mLastSize = mResSize;

	mBkSprite->setSize(mResSize).setPosition(mGlobalPosition);

	vec2 scrollerSize;
	vec2 scrollerPos;

	if (mType == ST_HORISONTAL)
	{
		scrollerSize.y = mResSize.y;

		if (mScorllerSize < 0)
		{
			scrollerSize.x = (mCurrentValue - mMinValue)/(mMaxValue - mMinValue)*mResSize.x;
			scrollerPos = mGlobalPosition;
		}
		else
		{
			const float minScrollerSize = 5.0f;
			scrollerSize.x = fmax(mResSize.x*(mScorllerSize/(mMaxValue - mMinValue)), minScrollerSize);

			scrollerPos.y = mGlobalPosition.y;
			scrollerPos.x = mGlobalPosition.x + 
				(mCurrentValue - mMinValue)/(mMaxValue-mMinValue)*(mResSize.x - scrollerSize.x);
		}
	}
	else
	{
		scrollerSize.x = mResSize.x;

		if (mScorllerSize < 0)
		{
			scrollerSize.y = (mCurrentValue - mMinValue)/(mMaxValue - mMinValue)*mResSize.y;
			scrollerPos = mGlobalPosition;
		}
		else
		{
			const float minScrollerSize = 5.0f;
			scrollerSize.y = fmax(mResSize.y*(mScorllerSize/(mMaxValue - mMinValue)), minScrollerSize);

			scrollerPos.x = mGlobalPosition.x;
			scrollerPos.y = mGlobalPosition.y + 
				(mCurrentValue - mMinValue)/(mMaxValue-mMinValue)*(mResSize.y - scrollerSize.y);
		}
	}

	mScrollerSprite->setSize(scrollerSize).setPosition(scrollerPos);
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
		scrollerSize = mScrollerSprite->getSize();
		scrollerPos = mScrollerSprite->getPosition();
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
			mCurrentValue += (point.x - mLastCursorPos.x)/mResSize.x*(mMaxValue - mMinValue + mScorllerSize);
		else
			mCurrentValue += (point.y - mLastCursorPos.y)/mResSize.y*(mMaxValue - mMinValue + mScorllerSize);
	}

	mCurrentValue = fclamp(mCurrentValue, mMinValue, mMaxValue);

	if (mChangeValueCallback)
	{
		static_cast< cCallback1Param<float>* >(mChangeValueCallback)->mArg = mCurrentValue;
		mChangeValueCallback->call();
	}

	mLastCursorPos = point;

	updateGraphics();
}

void uiScrollbar::setupInitialProperties()
{
	mResScrollerColor = mScrollerColor;
	uiWidget::setupInitialProperties();
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
	safe_release(mChangeValueCallback);

	mChangeValueCallback = callback;
}
