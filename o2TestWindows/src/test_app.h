#ifndef TEST_APP_H
#define TEST_APP_H

#include "app/application.h"
#include "util/singleton.h"

class TestApp:public o2::cApplication, public o2::cSingleton<TestApp>
{
public:

	TestApp():cApplication() {}
	~TestApp() {}

	void onUpdate(float dt) {}
};

#endif //TEST_APP_H