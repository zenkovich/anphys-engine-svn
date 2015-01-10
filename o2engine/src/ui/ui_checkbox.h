#ifndef UI_CHECKBOX_H
#define UI_CHECKBOX_H

#include "ui_button.h"

#include "util/callback.h"
#include "util/string.h"

OPEN_O2_NAMESPACE

class grText;

class uiCheckBox: public uiButton
{
	uiState* mCheckedState;  /** Checked state, must have name "checked". Activates when checkbox checked. */
	bool     mChecked;       /** True, if checked. */

public:
	DEFINE_TYPE(uiCheckBox);

	PROPERTY(uiCheckBox, bool) checked;  /** Check property. Using set/isChecked. */

	CallbackChain onCheckOn;            /** On check on event. */
	CallbackChain onCheckOff;           /** On check off event. */


	/** ctor. */
	uiCheckBox(const layout& layout, const string& id = "");

	/** copy-ctor. */
	uiCheckBox(const uiCheckBox& checkbox);

	/** dtor. */
	~uiCheckBox();

	/** Returns copy of widget. */
	uiWidget* clone() const;

	/** Sets checkbox checking. */
	void setChecked(bool checked);

	/** Returns true, when checked. */
	bool isChecked() const;

protected:
	/** Calls when clicked. */
	virtual void onClicked();

	/** Initializing properties. */
	void initializeProperties();
};

CLOSE_O2_NAMESPACE

#endif // UI_CHECKBOX_H
