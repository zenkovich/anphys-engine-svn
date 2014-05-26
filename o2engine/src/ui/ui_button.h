#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include "ui_widget.h"

#include "util/callback.h"
#include "util/graphics/stretch_rect.h"

OPEN_O2_NAMESPACE

class uiButton:public uiWidget
{
protected:
	shared<uiState> mHoverState;    /** Hover state. Activates when cursor hover widget. */
	shared<uiState> mFocusedState;  /** Focused state. Activates when widget is focused. */
	shared<uiState> mPressedState;  /** Pressed state. Activates when button pressed. */

public:
	DEFINE_TYPE(uiButton);
	
	shared<cStretchRect> mRegularDrawable;  /** Regular button state drawable. */
	shared<cStretchRect> mFocusedDrawable;  /** Focused state drawable. */
	shared<cStretchRect> mHoverDrawable;    /** Hover state drawable. */
	shared<cStretchRect> mPressedDrawable;  /** Pressed state drawable. */

	cCallbackChain onClickEvent;            /** On click event. */
	cCallbackChain onHoverEvent;            /** On hover event. */
	cCallbackChain onHoverOffEvent;         /** On hover off event. */
	cCallbackChain onFocusedEvent;          /** On focused event. */
	cCallbackChain onFocusLostEvent;        /** On focus lost event. */


	/** ctor. */
	uiButton(const uiWidgetLayout& layout, 
		     const shared<cStretchRect>& regularDrawable, const shared<cStretchRect>& hoverDrawable = NULL,
		     const shared<cStretchRect>& focusedDrawable = NULL, const shared<cStretchRect>& pressedDrawable = NULL, 
			 const string& id = "", shared<uiWidget> parent = NULL);

	/** copy-ctor. */
	uiButton(const uiButton& button);

	/** dtor. */
	~uiButton();

	/** Returns copy of widget. */
	shared<uiWidget> clone() const;

	/** Returns true, if widget can take focus. */
	virtual bool isFocusable() const;

protected:
	/** Calls when added some state. */
	virtual void addedState(const shared<uiState>& state);

	/** Drawing current widget. */
	virtual void localDraw();

	/** Updating current widget. */
	virtual void localUpdate(float dt);

	/** Calls when widget's layout updated. */
	virtual void layoutUpdated();

	/** Processing input message in current widget. */
	virtual bool localProcessInputMessage(const cInputMessage& msg);

	/** Calls when widget focused. */
	virtual void onFocused();

	/** Calls when widget lost focus. */
	virtual void onFocusLost();
};

CLOSE_O2_NAMESPACE

#endif // UI_BUTTON_H
