#ifndef UI_CHECKBOX_H
#define UI_CHECKBOX_H

#include "ui_drawables_list_widget.h"

#include "util/callback.h"
#include "util/string.h"

OPEN_O2_NAMESPACE

class grText;

class uiCheckBox: public uiDrawablesListWidget
{
	uiState* mHoverState;    /** Hover state, must have name "hover". Activates when cursor hover checkbox. */
	uiState* mFocusedState;  /** Focused state, must have name "focus". Activates when checkbox is focused. */
	uiState* mPressedState;  /** Pressed state, must have name "pressed". Activates when checkbox pressed. */
	uiState* mCheckedState;  /** Checked state, must have name "pressed". Activates when checkbox checked. */

	grText*  mCaption;       

	bool     mHover;
	bool     mPressed;
	bool     mPressedByButton;
	bool     mChecked;

public:
	DEFINE_TYPE(uiCheckBox);

	PROPERTY(uiCheckBox, string)  ccaption; /** C Text caption property. Using set/getCCaption. */
	PROPERTY(uiCheckBox, wstring) caption;  /** Caption property. Using set/getCaption. */
	PROPERTY(uiCheckBox, bool)    checked;  /** Check property. Using set/isChecked. */

	cCallbackChain onClickEvent;     /** On click event. */
	cCallbackChain onHoverEvent;     /** On hover event. */
	cCallbackChain onHoverOffEvent;  /** On hover off event. */
	cCallbackChain onFocusedEvent;   /** On focused event. */
	cCallbackChain onFocusLostEvent; /** On focus lost event. */
	cCallbackChain onCheckOn;        /** On check on event. */
	cCallbackChain onCheckOff;       /** Oncheck off event. */


	/** ctor. */
	uiCheckBox(const cLayout& layout, const string& id = "", uiWidget* parent = NULL);

	/** copy-ctor. */
	uiCheckBox(const uiCheckBox& button);

	/** dtor. */
	~uiCheckBox();

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

	/** Sets checkbox checking. */
	void setChecked(bool checked);

	/** Returns true, when checked. */
	bool isChecked() const;

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

	/** Initializing properties. */
	void initializeProperties();
};

CLOSE_O2_NAMESPACE

#endif // UI_CHECKBOX_H
