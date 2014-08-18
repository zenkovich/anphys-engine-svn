#ifndef UI_VER_LAYOUT_H
#define UI_VER_LAYOUT_H

#include "ui_widget.h"

OPEN_O2_NAMESPACE

class uiVerLayout: public uiWidget 
{
	float      mWidgetsDistance;
	WidgetsVec mFlexibleWidgets;

public:
	//Type definition
	DEFINE_TYPE(uiVerLayout);

	/** ctor. */
	uiVerLayout(const cLayout& layout, float widgetsDistance = 10.0f, const string& id = "");

	/** copy-ctor. */
	uiVerLayout(const uiVerLayout& widget);

	/** dtor. */
	virtual ~uiVerLayout();

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

#endif // UI_VER_LAYOUT_H
