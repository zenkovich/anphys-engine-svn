// win32_app.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#include "engine/engine_incl.h"
#include "../physics_test/src/box_collision_test_frame.h"
#include "../physics_test/src/physics_test_frame.h"

void outVec(const pvec<int>& vec)
{
	for (int i = 0; i < vec.size(); i++)
		printf("value %i = %i\n", i, vec[i]);
}

void pvecTest()
{
	pvec<int> testVec(3);

	printf("-------------1\n");
	outVec(testVec);

	for (int i = 0; i < 10; i++)
		*testVec.addValue() = i;
	
	for (int i = 0; i < 10; i++)
		testVec[i] = 10 - i;

	printf("-------------2\n");
	outVec(testVec);

	testVec.removeValue(5);

	printf("-------------3\n");
	outVec(testVec);

	testVec.resize(5);

	printf("-------------4\n");
	outVec(testVec);
	
	pvec<int> testVec2(testVec);

	printf("-------------5\n");
	outVec(testVec2);
	
	pvec<int> testVec3;
	testVec3 = testVec2;

	printf("-------------6\n");
	outVec(testVec3);
}

int _tmain(int argc, _TCHAR* argv[])
{
	testDataObject();

	pvecTest();


	printf("%s\n", argv[0]);

	gLogSystem = createLogSystem("log.txt");

	cEngine::initializeSingleton();
	getEngine().initialize();
	getFileSystem().mResourcesDirectory = "../data/";

	//engine.addFrame(new apBoxCollisionTestFrame("anPhys box collision test", fRect(0,0,800,600)));
	getEngine().addFrame(new apPhysicsTestFrame("anPhys physics test", fRect(200,0,1224,730)));
	//engine.addFrame(new apTestFrame("anPhys2 test", fRect(0,0,800,600)));

	getEngine().run();

	return 0;
}

