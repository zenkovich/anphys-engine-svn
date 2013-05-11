#ifndef UI_WINDOW_H
#define UI_WINDOW_H

#include "ui_widget.h"

struct uiScrollArea;
struct uiLabel;
struct uiButton;

struct uiWindow:public uiWidget
{
	DEFINE_TYPE(uiWindow)

	uiWidget* mWindowHead;
	uiButton* mCloseButton;
	uiLabel*  mWindowCaption;
	uiWidget* mWindowContent;
	uiWidget* mBackWidget;


	uiState*      mHeadSelectingState;
	uiState*      mHeadPressedState;

	bool          mHeadSelected;
	bool          mHeadPressed;

	vec2          mLastCursorPos;

//functions
	uiWindow(uiWidgetsManager* widgetsManager, const std::string& id, const vec2& size, const vec2& pos,
		     uiWidget* head, uiWidget* content, uiWidget* backWidget, uiButton* closeButton = NULL, uiLabel* windowCaption = NULL);
	~uiWindow();

	void addChild(uiWidget* widget);
	void removeChild(uiWidget* widget);
	void removeAllChilds();

	uiWidget* getWidget(const std::string& id) const;

	int processInputMessageDerived(const cInputMessage& message);

	void onCloseBtn();

	bool isAdjustingSizeByChilds() { return false; }
};

#endif //UI_WINDOW_H