#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include "ui_widget.h"

#include "util/callback.h"
#include "util/graphics/stretch_rect.h"

OPEN_O2_NAMESPACE

class uiButton:public uiWidget
{
public:
	typedef vector< shared<cStretchRect> > RectsVec;

protected:
	shared<uiState> mHoverState;    /** Hover state. Activates when cursor hover widget. */
	shared<uiState> mFocusedState;  /** Focused state. Activates when widget is focused. */
	shared<uiState> mPressedState;  /** Pressed state. Activates when button pressed. */
	RectsVec        mDrawables;     /** Drawables vector. */

	bool            mHover;
	bool            mPressed;

public:
	DEFINE_TYPE(uiButton);

	cCallbackChain onClickEvent;            /** On click event. */
	cCallbackChain onHoverEvent;            /** On hover event. */
	cCallbackChain onHoverOffEvent;         /** On hover off event. */
	cCallbackChain onFocusedEvent;          /** On focused event. */
	cCallbackChain onFocusLostEvent;        /** On focus lost event. */


	/** ctor. */
	uiButton(const uiWidgetLayout& layout, const string& id = "", shared<uiWidget> parent = NULL);

	/** copy-ctor. */
	uiButton(const uiButton& button);

	/** dtor. */
	~uiButton();

	/** Returns copy of widget. */
	shared<uiWidget> clone() const;

	/** Returns true, if widget can take focus. */
	virtual bool isFocusable() const;

	/** Adding drawable and return index of drawable. */
	int addDrawable(const shared<cStretchRect>& drawable);

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
