#ifndef UI_CONTROLLER_H
#define UI_CONTROLLER_H

#include "public.h"

#include "util/singleton.h"
#include "ui_widget.h"
#include "ui_label.h"

OPEN_O2_NAMESPACE

/** User interface controller. Processing and drawing widgets. */
class uiController: public cSingleton<uiController>
{
	typedef vector<uiWidget*> WidgetsVec;

	uiWidget  mBasicWidget;        /** Basic widget. */

	uiWidget* mHintWidget;         /** Hint widget. */
	uiLabel*  mHintWidgetLabel;    /** Hint label inside hint widget. */

	uiWidget* mFocusWidget;        /** Focused widget. */
	bool      mChangedFocusWidget; /** True, if focus widget need to change. */

public:
	/** ctor. */
	uiController();

	/* dtor. */
	~uiController();

	/** Updating widgets and processing input message. */
	void update(float dt);

	/** Drawing widgets. */
	void draw();

	/** Adding widget. */
	uiWidget* addWidget(uiWidget* widget);

	/** Removing widget. */
	bool removeWidget(uiWidget* widget);

	/** Removing all widgets. */
	bool removeAllWidgets();

	/** Returns widget with id path. */
	uiWidget* getWidget(const string& idPath);

	/** Sets focus on specified widget. */
	void focusOnWidget(uiWidget* widget);

	/** Returns widget with id path, casting to reque type. */
	template<typename T>
	T* getWidgetByType(const string& idPath)
	{
		return getWidget(idPath);
	}

private:
	/** Sets widget hint. */
	void setHintWidget(uiWidget* hintWidget, const string& labelWidgetPath = "");

	/** Label recursive search function. */
	uiLabel* FindLabel(uiWidget* widget);

	/** Shows hint at widget. */
	void showHint(uiWidget* hintingWidget);
};

/** Basic application ui controller host. */
#define uiHost() uiController::instancePtr()

CLOSE_O2_NAMESPACE

#endif //UI_CONTROLLER_H
