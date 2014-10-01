#ifndef UI_CONTEXT_MENU_H
#define UI_CONTEXT_MENU_H

#include "ui_drawables_list_widget.h"
#include "render_system/texture.h"
#include "util/string.h"

OPEN_O2_NAMESPACE

class uiButton;

class uiContextMenu : public uiDrawablesListWidget
{
	uiButton*  mButtonSample;
	uiWidget*  mDelimerSample;

public:
	DEFINE_TYPE(uiContextMenu);

	/** ctor. */
	uiContextMenu(const cLayout& layout, const string& id = "");

	/** copy-ctor. */
	uiContextMenu(const uiContextMenu& contextMenu);

	/** dtor. */
	virtual ~uiContextMenu();

	/** Returns clone of widget. */
	virtual uiWidget* clone() const;

	/** Adding button element. If position negative, adding at end. */
	uiButton* addButtonElement(const wstring& caption, const string& key = "", const string& iconTextureName = "", 
		                       int position = -1);

	/* Adding delimer element. If position negative, adding at end. */
	uiWidget* addDelimer(int position = -1);

	/** Adding custom widget element. If position negative, adding at end. */
	uiWidget* addElement(uiWidget* widget, int position = -1);

	/** Setting up samples elements. */
	void setupElementsSamples(uiWidget* delimer, uiButton* button);

protected:
	/** Calls when widget's layout updated. */
	virtual void layoutUpdated();

	/** Processing input message in current widget. */
	virtual bool localProcessInputMessage(const cInputMessage& msg);
};

CLOSE_O2_NAMESPACE

#endif //UI_CONTEXT_MENU_H
