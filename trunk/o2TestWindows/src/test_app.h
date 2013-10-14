#ifndef TEST_APP_H
#define TEST_APP_H

#include "app/application.h"
#include "util/singleton.h"
#include "util/math/basis.h"

OPEN_O2_NAMESPACE

class cManField;

class TestApp:public cApplication, public cSingleton<TestApp>
{
	cManField* mManField;

public:	
	TestApp();
	~TestApp();

	void onUpdate(float dt);
	void onDraw();

	void processMessage(cApplacationMessage::type message);

public:
	void onInitialized();
};

CLOSE_O2_NAMESPACE

#endif //TEST_APP_H