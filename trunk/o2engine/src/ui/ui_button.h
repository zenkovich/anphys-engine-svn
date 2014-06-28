#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include "ui_drawables_list_widget.h"

#include "util/callback.h"
#include "util/graphics/stretch_rect.h"
#include "util/string.h"

OPEN_O2_NAMESPACE

class grText;

class uiButton:public uiDrawablesListWidget
{
protected:
	uiState* mHoverState;    /** Hover state, must have name "hover". Activates when cursor hover widget. */
	uiState* mFocusedState;  /** Focused state, must have name "focus". Activates when widget is focused. */
	uiState* mPressedState;  /** Pressed state, must have name "pressed". Activates when button pressed. */

	grText*  mCaption;

	bool     mHover;
	bool     mPressed;
	bool     mPressedByButton;

public:
	DEFINE_TYPE(uiButton);

	PROPERTY(uiButton, string) ccaption; /** C Text caption property. Using set/getCCaption. */
	PROPERTY(uiButton, wstring) caption; /** Caption property. Using set/getCaption. */

	cCallbackChain onClickEvent;     /** On click event. */
	cCallbackChain onHoverEvent;     /** On hover event. */
	cCallbackChain onHoverOffEvent;  /** On hover off event. */
	cCallbackChain onFocusedEvent;   /** On focused event. */
	cCallbackChain onFocusLostEvent; /** On focus lost event. */


	/** ctor. */
	uiButton(const cLayout& layout, const string& id = "", uiWidget* parent = NULL);

	/** copy-ctor. */
	uiButton(const uiButton& button);

	/** dtor. */
	~uiButton();

	/** Returns copy of widget. */
	uiWidget* clone() const;

	/** Returns true, if widget can take focus. */
	virtual bool isFocusable() const;

	/** Sets c text caption. */
	void setCCaption(const string& caption);

	/** Returns c text caption. */
	string getCCaption() const;

	/** Sets caption text. */
	void setCaption(const wstring& caption);

	/** Returns caption text. */
	wstring getCaption() const;

protected:
	/** Calls when added some state. */
	virtual void addedState(uiState* state);

	/** Updating current widget. */
	virtual void localUpdate(float dt);

	/** Processing input message in current widget. */
	virtual bool localProcessInputMessage(const cInputMessage& msg);

	/** Calls when widget focused. */
	virtual void onFocused();

	/** Calls when widget lost focus. */
	virtual void onFocusLost();

	/** Calls when added drawable. */
	virtual void addedDrawable(Drawable* drawable);

	/** Calls when clicked. */
	virtual void onClicked();

	/** Initializing properties. */
	void initializeProperties();
};

CLOSE_O2_NAMESPACE

#endif // UI_BUTTON_H
