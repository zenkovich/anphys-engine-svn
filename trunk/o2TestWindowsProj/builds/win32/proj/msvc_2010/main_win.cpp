#include "test_app.h"

#include "util/mem_utils/test_allocator.h"
#include "util/mem_utils/std_allocator.h"
#include "util/mem_utils/pool_allocator.h"
#include "util/timer.h"
#include "public.h"
#include "util/shared.h"
#include "shared_test.h"

using namespace o2;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	shared<A> a = share(shared<A>(mnew A()), NULL);
	a->b->c->a = share(a, a->b->c);

	TestApp app;
	app.launch();

	return 0;
}