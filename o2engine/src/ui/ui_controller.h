#ifndef UI_CONTROLLER_H
#define UI_CONTROLLER_H

#include "public.h"

#include "util/singleton.h"
#include "ui_widget.h"

OPEN_O2_NAMESPACE

class uiController: public cSingleton<uiController>
{
	typedef vector< shared<uiWidget> > WidgetsVec;

	WidgetsVec       mWidgets;
	shared<uiWidget> mFocusWidget;

public:
	uiController();
	~uiController();

	void update(float dt);
	void draw();

	shared<uiWidget> addWidget(const shared<uiWidget>& widget);
	bool removeWidget(const shared<uiWidget>& widget);
	bool removeAllWidgets();

	shared<uiWidget> getWidget(const string& idPath);

	void focusOnWidget(const shared<uiWidget>& widget);

	template<typename T>
	shared<T> getWidgetByType(const string& idPath)
	{
		return getWidget(idPath);
	}
};

#define uiHost() uiController::instancePtr()

CLOSE_O2_NAMESPACE

#endif //UI_CONTROLLER_H
