#ifndef UI_HOR_LAYOUT_WIDGET_H
#define UI_HOR_LAYOUT_WIDGET_H

#include "ui_widget.h"

struct uiHorLayoutWidget:public uiWidget
{
	float mCurrentWidth;
	float mWidgetsDistance;

	DEFINE_TYPE(uiHorLayoutWidget);

	//functions
	uiHorLayoutWidget(uiWidgetsManager* widgetsManager, const std::string& id);

	void addChild(uiWidget* widget);
};

struct uiVertLayoutWidget:public uiWidget
{
	float mCurrentHeight;
	float mWidgetsDistance;

	DEFINE_TYPE(uiVertLayoutWidget);

	//functions
	uiVertLayoutWidget(uiWidgetsManager* widgetsManager, const std::string& id);

	void addChild(uiWidget* widget);
};

#endif //UI_HOR_LAYOUT_WIDGET_H