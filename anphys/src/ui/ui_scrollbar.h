#ifndef UI_SCROLLBAR_H
#define UI_SCROLLBAR_H

#include "ui_widget.h"
#include "util/other/callback.h"

struct grSprite;
struct uiState;

struct uiScrollbar:public uiWidget
{
	DEFINE_TYPE(uiScrollbar)

	enum ScrollbarType { ST_HORISONTAL = 0, ST_VERTICAL };

	ScrollbarType       mType;
	float               mMinValue;
	float               mMaxValue;
	float               mCurrentValue;
	bool                mEnable;
	float               mScorllerSize;

	vec2                mLastGlobalPosition;
	vec2                mLastSize;

	grSprite*           mBkSprite;
	grSprite*           mScrollerSprite;

	uiState*            mSelectedState;
	uiState*            mPressedState;

	bool                mSelected;
	bool                mPressed;

	vec2                mLastCursorPos;
	color4              mScrollerColor;
	color4              mResScrollerColor;

	cCallbackInterface* mOnChangeValueCallback;

//functions
	uiScrollbar(uiWidgetsManager* widgetsManager, const std::string& id, ScrollbarType type, 
		        const vec2& size, grSprite* bkSprite, grSprite* scrollerSprite, 
				float minValue = 0.0f, float maxValue = 1.0f, float currentValue = 0.0f, float scollSize = -1);
	uiScrollbar(const uiScrollbar& scrollbar);
	~uiScrollbar();

	void derivedUpdate(float dt);
	void derivedDraw();

	int processInputMessageDerived(const cInputMessage& message);

	void setupInitialProperties();

	uiScrollbar* setScrollerSize(float size);
	float        getScrollerSize() const;

	void setChangeValueCallback(cCallbackInterface* callback);

protected:
	void updateGraphics();

	bool isPointInScroller(const vec2& point);

	void pressed(const vec2& point);
	void mouseMoved(const vec2& point);
};

#endif //UI_SCROLLBAR_H