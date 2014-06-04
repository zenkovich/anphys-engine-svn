#ifndef INPUT_MSG_TEST_H
#define INPUT_MSG_TEST_H

#include "test_interface.h"

OPEN_O2_NAMESPACE

class grText;

class InputMsgTest: public ITest 
{
	grText*        mText;

public:
	InputMsgTest();
	~InputMsgTest();

	void draw();
	void update(float dt);
};

CLOSE_O2_NAMESPACE

#endif // INPUT_MSG_TEST_H
