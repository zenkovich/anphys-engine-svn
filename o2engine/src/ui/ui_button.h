#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include "widget.h"

#include "util/callback.h"

OPEN_O2_NAMESPACE

class uiButton:public uiWidget
{
	ICallback* mOnClick;
	ICallback* mOnSelect;

public:
	DEFINE_TYPE(uiButton);

	//properties
	PROPERTY(uiButton, ICallback*) onClick;
	PROPERTY(uiButton, ICallback*) onSelect;


	uiButton(ICallback* onClickCB = NULL, ICallback* onSelectCB = NULL, const string& id = "", uiWidget* parent = NULL, 
		     const vec2f& localPos = vec2f());

	uiButton(const uiButton& button);

	~uiButton();

	uiWidget* clone() const;


};

CLOSE_O2_NAMESPACE

#endif // UI_BUTTON_H
