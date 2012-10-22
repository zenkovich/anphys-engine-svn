// win32_app.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#include "../src/engine/engine_incl.h"
#include "../physics_test/src/test_frame.h"


int _tmain(int argc, _TCHAR* argv[])
{
	gLogSystem = createLogSystem("log.txt");

	cDataObject obj;
	obj = vec3(1, 0, 3);
	std::string str = obj.getString();
	cDataObject obj2 = "123.321";
	float vecf = obj2.getFloat();

	cEngine engine;
	engine.initialize();
	engine.addFrame(new apTestFrame("anPhys test", fRect(0,0,800,600)));
	//engine.addFrame(new apTestFrame("anPhys2 test", fRect(0,0,800,600)));

	engine.run();

	return 0;
}

