#ifndef UI_CONTROLLER_H
#define UI_CONTROLLER_H

#include "public.h"

#include "util/singleton.h"
#include "ui_widget.h"

OPEN_O2_NAMESPACE

class uiController: public cSingleton<uiController>
{
	typedef vector< shared(uiWidget) > WidgetsVec;

	WidgetsVec mWidgets;

public:
	uiController();
	~uiController();

	void update(float dt);
	void draw();
};

CLOSE_O2_NAMESPACE

#endif //UI_CONTROLLER_H
