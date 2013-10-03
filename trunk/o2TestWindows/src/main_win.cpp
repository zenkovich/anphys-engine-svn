#include "test_app.h"

#include "util/mem_utils/test_allocator.h"
#include "util/mem_utils/std_allocator.h"
#include "util/timer.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	TestApp app;
	
	o2::cTestAllocator testAllocator(10*1024*1024);
	o2::cStdAllocator testAllocator2;

	const int maxValues = 80;
	const int maxSize = 16/1024;
	const int minSize = 3;
	const int testIterations = 1024*1024;
	const int changeBlocksCount = 4;

	o2::IAllocator* allocatorTests[] = { &testAllocator, &testAllocator2 };
	int allocatorTestsCount = 2;

	for (int j = 0; j < allocatorTestsCount; j++)
	{
		o2::IAllocator* allocator = allocatorTests[j];

		o2::cTimer timer;
		timer.reset();

		void* p[maxValues];
		int c[changeBlocksCount];
		for (int i = 0; i < testIterations; i++)
		{
			if (i < maxValues)
			{
				int size = minSize + rand()%(maxSize - minSize);
				p[i] = allocator->alloc(size);
				if (!p[i])
				{
					printf("failed to alloc %i\n", i);
				}
				/*else
				{
					printf("alloc %i: %x (%i bytes)\n", i, p[i], size);
					//testAllocator.dump();
				}*/
			}
			else if (i >= maxValues && i < testIterations - maxValues)
			{
				for (int k = 0; k < changeBlocksCount; k++)
				{
					int idx;
					while (true)
					{
						idx = rand()%maxValues;
						bool next = true;
						for (int l = 0; l < k; l++)
						{
							if (c[l] == idx)
							{
								next = false;
							}
						}
						if (next)
							break;
					}

					c[k] = idx;
				}

				for (int k = 0; k < changeBlocksCount; k++)
				{
					allocator->free(p[c[k]]);
				}				

				for (int k = 0; k < changeBlocksCount; k++)
				{
					int newSize = minSize + rand()%(maxSize - minSize);
					p[c[k]] = allocator->alloc(newSize);
					if (!p[c[k]])
					{
						printf("bad alloc\n");
						testAllocator.dump();
					}
				}
				/*int idx = rand()%maxValues;
				void* oldPtr = p[idx];
				allocator->free(p[idx]);
				int newSize = minSize + rand()%(maxSize - minSize);
				p[idx] = allocator->alloc(newSize);

				if (!p[idx])
				{
					printf("bad alloc\n");
					testAllocator.dump();
				}*/
				//printf("realloc %i: %x -> %x (%i bytes)\n", idx, oldPtr, p[idx], newSize);
				//testAllocator.dump();
			}
			else
			{
				allocator->free(p[i - testIterations + maxValues]);
			}
		}

		printf("Allocator %s - %.3f ms\n", allocator->getName(), timer.getTime()*1000.0f);
	}

	/*void* p1 = testAllocator.alloc(4);
	void* p2 = testAllocator.alloc(8);
	void* p3 = testAllocator.alloc(16);
	
	testAllocator.free(p2);
	p2 = testAllocator.alloc(6);
	testAllocator.free(p1);
	testAllocator.free(p3);
	testAllocator.free(p2);

	testAllocator.dump();*/

	app.launch();

	return 0;
}