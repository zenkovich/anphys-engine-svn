#ifndef UI_HOR_LAYOUT_H
#define UI_HOR_LAYOUT_H

#include "ui_widget.h"

OPEN_O2_NAMESPACE

class uiHorLayout: public uiWidget 
{
public:
	//Type definition
	DEFINE_TYPE(uiHorLayout);

	/** ctor. */
	uiHorLayout(const cLayout& layout, const string& id = "");

	/** copy-ctor. */
	uiHorLayout(const uiHorLayout& widget);

	/** dtor. */
	virtual ~uiHorLayout();

	/** Returns clone of widget. */
	virtual uiWidget* clone() const;

protected:
	/** Calls when widget's layout updated. */
	virtual void layoutUpdated();
};

CLOSE_O2_NAMESPACE

#endif // UI_HOR_LAYOUT_H
