#include "test_app.h"

#include "util/serialization/serialization.h"
#include "util/serialization/serialize_test.cpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	TestApp app;
	o2::testSerialization();
	app.launch();

	return 0;
}