#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include "ui_widget.h"

struct cCallbackInterface;

struct uiButton:public uiWidget
{
	DEFINE_TYPE(uiButton)

protected:
	cCallbackInterface* mOnClickCallback;
	bool                mPressed;
	bool                mSelected;

public:
	uiState*            mSelectedState;
	uiState*            mPressedState;

//functions
	uiButton(uiWidgetsManager* widgetsManager, const std::string& id, cCallbackInterface* callback = NULL);
	uiButton(const uiButton& button);
	~uiButton();

	void derivedUpdate(float dt);

	void setCallback(cCallbackInterface* callback);

	int processInputMessageDerived(const cInputMessage& message);

	bool isAdjustingSizeByChilds() { return false; }
};

#endif //UI_BUTTON_H