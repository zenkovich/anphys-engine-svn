#include "test_app.h"

#include "util/serialization/serialization.h"
#include "util/serialization/serialize_test.cpp"
#include "util/threads/thread.h"
#include "util/callback.h"

void someFunc(int v, int a)
{
	for (int i = 0; i < v; i++)
	{
		printf("%i: atata %i\n", a, i);
		Sleep(10);
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	TestApp app;

	int* i = mnew int;
	int* ir = mnew int[33];

	o2::cMemoryManager::instance().dump();

	safe_release(i);
	safe_release_arr(ir);
	
	o2::cMemoryManager::instance().dump();

	/*o2::cThread thread, thread2;
	thread.start(mnew o2::cCallback2Param<int, int>(&someFunc, 100, 1));
	thread2.start(mnew o2::cCallback2Param<int, int>(&someFunc, 200, 2));*/

	o2::testSerialization();
	app.launch();

	return 0;
}