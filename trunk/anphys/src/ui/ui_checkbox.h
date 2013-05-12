#ifndef UI_CHECKBOX_H
#define UI_CHECKBOX_H

#include "ui_widget.h"

struct uiCheckBox:public uiWidget
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

	void setCheck(bool flag);
	bool isChecked() const;

	int processInputMessageDerived(const cInputMessage& message);
};

#endif //UI_CHECKBOX_H