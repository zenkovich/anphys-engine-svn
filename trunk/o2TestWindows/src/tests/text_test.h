#ifndef TEXT_TEST_H
#define TEXT_TEST_H

#include "test_interface.h"

OPEN_O2_NAMESPACE

class grText;

class cTextTest: public ITest 
{
	grText* mText;
	int mTouchCorner;

public:
	cTextTest();

	void update(float dt);
	void draw();
};

CLOSE_O2_NAMESPACE

#endif // TEXT_TEST_H
