#ifndef UI_CONTROLLER_H
#define UI_CONTROLLER_H

#include "public.h"

#include "util/singleton.h"
#include "ui_widget.h"

OPEN_O2_NAMESPACE

/** User interface controller. Processing and drawing widgets. */
class uiController: public cSingleton<uiController>
{
	typedef vector< shared<uiWidget> > WidgetsVec;

	uiWidget         mBasicWidget;
	shared<uiWidget> mFocusWidget; /** Focused widget. */

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
	shared<uiWidget> addWidget(const shared<uiWidget>& widget);

	/** Removing widget. */
	bool removeWidget(const shared<uiWidget>& widget);

	/** Removing all widgets. */
	bool removeAllWidgets();

	/** Returns widget with id path. */
	shared<uiWidget> getWidget(const string& idPath);

	/** Sets focus on specified widget. */
	void focusOnWidget(const shared<uiWidget>& widget);

	/** Returns widget with id path, casting to reque type. */
	template<typename T>
	shared<T> getWidgetByType(const string& idPath)
	{
		return getWidget(idPath);
	}
};

/** Basic application ui controller host. */
#define uiHost() uiController::instancePtr()

CLOSE_O2_NAMESPACE

#endif //UI_CONTROLLER_H
