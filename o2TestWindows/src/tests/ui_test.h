#ifndef UI_TEST_H
#define UI_TEST_H

#include "test_interface.h"

OPEN_O2_NAMESPACE

class uiSprite;
class uiWidget;

class cUITest: public ITest
{
	shared<uiSprite> spr;
	shared<uiWidget> wdg;

public:
	cUITest();

	void update(float dt);
	void draw();
};

CLOSE_O2_NAMESPACE

#endif //UI_TEST_H
