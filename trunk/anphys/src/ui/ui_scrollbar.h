#ifndef UI_SCROLLBAR_H
#define UI_SCROLLBAR_H

#include "ui_widget.h"
#include "ui_binding_values.h"

#include "util/other/callback.h"

struct grSprite;
struct uiState;

struct uiScrollbar:public uiWidget, public uiBindingValues
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

	uiWidget*           mBackWidget;
	uiWidget*           mScrollerWidget;

	uiState*            mSelectedState;
	uiState*            mPressedState;

	bool                mSelected;
	bool                mPressed;

	vec2                mLastCursorPos;

	cCallbackInterface* mOnChangeValueCallback;

//functions
	uiScrollbar(uiWidgetsManager* widgetsManager, const std::string& id, ScrollbarType type, 
		        const vec2& size, uiWidget* bkWidget, uiWidget* scrollerWidget, 
				float minValue = 0.0f, float maxValue = 1.0f, float currentValue = 0.0f, float scollSize = -1);
	uiScrollbar(const uiScrollbar& scrollbar);
	~uiScrollbar();

	void derivedUpdate(float dt);

	void derivedDraw();

	int processInputMessageDerived(const cInputMessage& message);

	uiScrollbar* setScrollerSize(float size);
	float        getScrollerSize() const;

	void setChangeValueCallback(cCallbackInterface* callback);

	bool isAdjustingSizeByChilds() { return false; }

protected:
	void updateGraphics();

	bool isPointInScroller(const vec2& point);

	void pressed(const vec2& point);
	void mouseMoved(const vec2& point);

	bool checkBindedValues();
};

#endif //UI_SCROLLBAR_H