#ifndef UI_RECT_H
#define UI_RECT_H

#include "ui_widget.h"

#include "util/graphics/stretch_rect.h"

OPEN_O2_NAMESPACE

class uiRect:public uiWidget
{
public:
	cStretchRect mStretchRect;

	uiRect(const uiWidgetLayout& layout, const string& id = "", shared<uiWidget> parent = NULL);

	/** copy-ctor. */
	uiRect(const uiRect& rectWidget);

	/** dtor. */
	~uiRect();

	/** Returns clone of widget. */
	shared<uiWidget> clone() const;

private:
	/** Drawing current widget. */
	void localDraw();

	void layoutUpdated();
};

CLOSE_O2_NAMESPACE

#endif // UI_RECT_H
