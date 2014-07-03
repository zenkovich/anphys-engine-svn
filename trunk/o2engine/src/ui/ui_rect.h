#ifndef UI_RECT_H
#define UI_RECT_H

#include "ui_widget.h"

#include "util/graphics/stretch_rect.h"

OPEN_O2_NAMESPACE

/** UI rect. Widget that drawing cStretchRect. */
class uiRect:public uiWidget
{
public:
	cStretchRect mStretchRect; /** Stretching rect drawable. */


	/** ctor. */
	uiRect(const cLayout& layout, const string& id = "");

	/** copy-ctor. */
	uiRect(const uiRect& rectWidget);

	/** dtor. */
	~uiRect();

	/** Returns clone of widget. */
	uiWidget* clone() const;

private:
	/** Drawing current widget. */
	void localDraw();

	/** Calls when widget layout updated. Here updating stretch rect position and size. */
	void layoutUpdated();

	/** Calls when parameter "transparency" was changed. */
	void transparencyChanged();
};

CLOSE_O2_NAMESPACE

#endif // UI_RECT_H
