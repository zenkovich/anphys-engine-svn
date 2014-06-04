#include "test_app.h"

#include "util/mem_utils/test_allocator.h"
#include "util/mem_utils/std_allocator.h"
#include "util/mem_utils/pool_allocator.h"
#include "util/timer.h"

struct A
{
	char a1;
	char a2;

	A():a1(0), a2(1) {}
};

struct B: public A
{
	char b1;
	char b2;

	B():b1(2), b2(3) {}
};

struct C: public B
{
	char c1;
	char c2;

	C():c1(4), c2(5) {}
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	A a;
	B b;
	C c;

	A* a_pa = &a;
	B* b_pb = &b;
	C* c_pc = &c;
	A* c_pa = (A*)&c;
	B* c_pb = (B*)&c;
	A* b_pa = (A*)&b;

	o2::TestApp app;
	app.launch();

	return 0;
}