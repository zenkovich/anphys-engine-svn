#include "test_app.h"

#include "public.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	o2::TestApp app;
	app.launch();

	return 0;
}