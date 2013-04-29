#ifndef UI_TEXT_EDIT_H
#define UI_TEXT_EDIT_H

#include "ui_widget.h"

struct uiLabel;
struct cCallbackInterface;

struct uiTextEdit:public uiWidget
{
	DEFINE_TYPE(uiTextEdit)

	uiLabel*            mLabel;
	uiWidget*           mBackWidget;
	unsigned int        mCaretPos;

	uiState*            mSelectedState;
	uiState*            mFocusedState;
	bool                mSelected;
	bool                mPressed;

	cCallbackInterface* mOnTextChangedCallback;

	vec2                mLastGlobalPosition;

//functions
	uiTextEdit(uiWidgetsManager* widgetsManager, const std::string& id, uiLabel* label, uiWidget* backWidget);
	uiTextEdit(const uiTextEdit& textWidget);
	~uiTextEdit();

	void         derivedUpdate(float dt);
	void         derivedDraw();

	int          processInputMessageDerived(const cInputMessage& message);

	void         onFocused();
	void         onUnfocused();

	uiTextEdit&  setText(const std::string& text);
	std::string& getText();

	uiTextEdit*  clone();
};

#endif //UI_TEXT_EDIT_H