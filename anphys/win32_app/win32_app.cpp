// win32_app.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#include "engine/engine_incl.h"
#include "../physics_test/src/box_collision_test_frame.h"
#include "../physics_test/src/physics_test_frame.h"
#include "../mesh_test/mesh_test_frame.h"

int _tmain(int argc, _TCHAR* argv[])
{
	testDataObject();

	gLogSystem = createLogSystem("log.txt");

	cEngine::initializeSingleton();
	getEngine().initialize();
	getFileSystem().mResourcesDirectory = "../data/";

	//engine.addFrame(new apBoxCollisionTestFrame("anPhys box collision test", fRect(0,0,800,600)));
	getEngine().addFrame(new apPhysicsTestFrame("anPhys physics test", fRect(200,0,1224,730)));
	//getEngine().addFrame(new apMeshTestFrame("mesh test", fRect(200,0,1224,730)));
	//engine.addFrame(new apTestFrame("anPhys2 test", fRect(0,0,800,600)));

	getEngine().run();

	return 0;
}

