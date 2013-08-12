#ifndef TEST_APP_H
#define TEST_APP_H

#include "app/application.h"
#include "util/singleton.h"

OPEN_O2_NAMESPACE

class grTexture;
class grMesh;
class grSprite;

CLOSE_O2_NAMESPACE

class TestApp:public o2::cApplication, public o2::cSingleton<TestApp>
{
public:
	o2::grTexture* texture;
	o2::grMesh*    mesh;
	o2::grSprite*  sprite;

	TestApp();
	~TestApp();

	void onUpdate(float dt);
	void onDraw();

	void processMessage(o2::cApplacationMessage::type message);
};

#endif //TEST_APP_H