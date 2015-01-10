#ifndef UI_HOR_LAYOUT_H
#define UI_HOR_LAYOUT_H

#include "ui_widget.h"

OPEN_O2_NAMESPACE

class uiHorLayout: public uiWidget 
{
	float      mWidgetsDistance;
	WidgetsVec mFlexibleWidgets;

public:
	//Type definition
	DEFINE_TYPE(uiHorLayout);

	/** ctor. */
	uiHorLayout(const layout& layout, float widgetsDistance = 10.0f, const string& id = "");

	/** copy-ctor. */
	uiHorLayout(const uiHorLayout& widget);

	/** dtor. */
	virtual ~uiHorLayout();

	/** Returns clone of widget. */
	virtual uiWidget* clone() const;

	/** Adding child widget. If position negative, adding at end. */
	virtual uiWidget* addChild(uiWidget* widget, int position = -1);

	/** Sets widgets distance. */
	void setWidgetsDistance(float distance);

	/** Returns widgets distance. */
	float getWidgetsDistance() const;

protected:
	/** Calls when widget's layout updated. */
	virtual void layoutUpdated();
};

CLOSE_O2_NAMESPACE

#endif // UI_HOR_LAYOUT_H
