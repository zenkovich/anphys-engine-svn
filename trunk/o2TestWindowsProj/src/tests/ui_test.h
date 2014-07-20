#ifndef UI_TEST_H
#define UI_TEST_H

#include "test_interface.h"

OPEN_O2_NAMESPACE

class uiSprite;
class uiWidget;
class grSprite;
class uiProgressBar;
class IRectDrawable;
class uiScrollBar;
class uiScrollArea;

class cUITest: public ITest
{
	uiSprite* spr;
	uiWidget* wdg;
	uiWidget* rect;
	IRectDrawable* sprite;
	uiProgressBar* mTestBar;
	uiScrollBar*   mTestScrollBar;
	uiScrollArea*  mTestScrollArea;

public:
	cUITest();

	void update(float dt);
	void draw();
};

CLOSE_O2_NAMESPACE

#endif //UI_TEST_H
