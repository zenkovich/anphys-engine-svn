#ifndef UI_PROGRESSBAR_H
#define UI_PROGRESSBAR_H

#include "ui_drawables_list_widget.h"

OPEN_O2_NAMESPACE

class uiProgressBar: public uiDrawablesListWidget
{	
protected:

public:
	/** ctor. */
	uiProgressBar(const uiWidgetLayout& layout, const string& id = "", shared<uiWidget> parent = NULL);

	/** copy-ctor. */
	uiProgressBar(const uiProgressBar& widget);

	/** dtor. */
	virtual ~uiProgressBar();

	/** Returns clone of widget. */
	virtual shared<uiWidget> clone() const;

	//setters and getters
	/** Returns true, if widget can take focus. */
	virtual bool isFocusable() const;

protected:

	/** Calls when added some state. */
	virtual void addedState(const shared<uiState>& state) {}

	/** Drawing current widget. */
	virtual void localDraw() {}

	/** Updating current widget. */
	virtual void localUpdate(float dt) {}

	/** Calls when widget's layout updated. */
	virtual void layoutUpdated() {}

	/** Processing input message in current widget. */
	virtual bool localProcessInputMessage(const cInputMessage& msg) { return false; }

	/** Calls when widget focused. */
	virtual void onFocused();

	/** Calls when widget lost focus. */
	virtual void onFocusLost();
};

CLOSE_O2_NAMESPACE

#endif // UI_PROGRESSBAR_H
