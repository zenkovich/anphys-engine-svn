#ifndef UI_CHECKBOX_H
#define UI_CHECKBOX_H

#include "ui_widget.h"
#include "ui_binding_values.h"

struct uiCheckBox:public uiWidget, public uiBindingValues
{
	DEFINE_TYPE(uiCheckBox)

	uiWidget* mCheckBack;
	uiWidget* mCheckThumb;
	uiWidget* mInformationWidget;

	uiState*  mSelectedState;
	uiState*  mPressedState;

	bool      mSelected;
	bool      mPressed;

protected:
	bool      mChecked;

public:
	uiCheckBox(uiWidgetsManager* widgetsManager, const std::string& id, uiWidget* checkBack, uiWidget* checkThumb,
		       uiWidget* informationWidget);
	~uiCheckBox();

	void derivedUpdate(float dt);

	void setCheck(bool flag);
	bool isChecked() const;

	int processInputMessageDerived(const cInputMessage& message);

protected:
	void checkBindedValues();
};

#endif //UI_CHECKBOX_H