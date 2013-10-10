#include "test_app.h"

#include "util/mem_utils/test_allocator.h"
#include "util/mem_utils/std_allocator.h"
#include "util/mem_utils/pool_allocator.h"
#include "util/timer.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	std::string str = "123/312";
	int p = str.find('*');
	std::string ss = str.substr(0, p);


	TestApp app;

	o2::cPoolAllocator poolAllocator(4, 4*1024);
	
	void* p1 = poolAllocator.alloc(1);
	void* p2 = poolAllocator.alloc(1);
	void* p3 = poolAllocator.alloc(1);
	void* p4 = poolAllocator.alloc(1);
	
	poolAllocator.free(p2);
	poolAllocator.free(p4);
	poolAllocator.free(p3);
	poolAllocator.free(p1);
	
	p1 = poolAllocator.alloc(1);
	p2 = poolAllocator.alloc(1);
	p3 = poolAllocator.alloc(1);
	p4 = poolAllocator.alloc(1);

	app.launch();

	return 0;
}