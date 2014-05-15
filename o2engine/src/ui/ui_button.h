#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include "ui_widget.h"

#include "util/callback.h"

class cStretchRect;

OPEN_O2_NAMESPACE

class uiButton:public uiWidget
{
protected:
	shared(cStretchRect) mRegularDrawable;
	shared(cStretchRect) mSelectedDrawable;
	shared(cStretchRect) mHoverDrawable;
	shared(cStretchRect) mPressedDrawable;

	shared(ICallback) mOnClick;
	shared(ICallback) mOnSelect;

public:
	DEFINE_TYPE(uiButton);

	//properties
	PROPERTY(uiButton, shared(ICallback)) onClick;
	PROPERTY(uiButton, shared(ICallback)) onSelect;


	uiButton(shared(ICallback) onClickCB = NULL, shared(ICallback) onSelectCB = NULL, const string& id = "", 
		     shared(uiWidget) parent = NULL, const vec2f& localPos = vec2f());

	uiButton(const uiButton& button);

	~uiButton();

	shared(uiWidget) clone() const;

	void setOnClickCallback(const shared(ICallback)& callback);
	shared(ICallback) getOnClickCallback() const;

	void setOnSelectCallback(const shared(ICallback)& callback);
	shared(ICallback) getOnSelectCallback() const;
};

CLOSE_O2_NAMESPACE

#endif // UI_BUTTON_H
