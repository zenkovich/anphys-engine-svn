#ifndef UI_SCROLL_AREA_H
#define UI_SCROLL_AREA_H

#include "ui_widget.h"

struct uiScrollbar;

struct uiScrollArea:public uiWidget
{
	vec2 mScrollingArea;
	vec2 mViewArea;

	uiScrollbar* mHorScrollbar;
	uiScrollbar* mVerScrollbar;

	float mLastHorScrolling;
	float mLastVerScrolling;

//functions
	uiScrollArea(uiWidgetsManager* widgetsManager, const std::string& id, const vec2& size);
	uiScrollArea(const uiScrollArea& scrollarea);
	~uiScrollArea();

	void derivedUpdate(float dt);
	void derivedDraw();

	void addChild(uiWidget* widget);
	void removeChild(uiWidget* widget);
	void removeAllChilds();

protected:
	void horScrollbarChanged(float value);
	void verScrollbarChanged(float value);
};

#endif //UI_SCROLL_AREA_H