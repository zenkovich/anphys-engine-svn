#ifndef TEST_INTERFACE_H
#define TEST_INTERFACE_H

#include "public.h"

OPEN_O2_NAMESPACE

class cApplication;
class cInputMessage;

class ITest
{
protected:
	cApplication* mApplication;
	cInputMessage* mInputMessage;

public:
	ITest(cApplication* app, cInputMessage* inputMsg):
	  mApplication(app), mInputMessage(inputMsg) {}

	virtual ~ITest() {}

	virtual void draw() {}
	virtual void update(float dt) {}
};

CLOSE_O2_NAMESPACE

#endif // TEST_INTERFACE_H
