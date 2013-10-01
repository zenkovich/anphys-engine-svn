#ifndef TEST_APP_H
#define TEST_APP_H

#include "app/application.h"
#include "util/singleton.h"
#include "util/math/basis.h"

OPEN_O2_NAMESPACE

class grTexture;
class grMesh;
class grSprite;
class grRenderTarget;
class grCamera;

CLOSE_O2_NAMESPACE

class TestApp:public o2::cApplication, public o2::cSingleton<TestApp>
{
public:
	o2::grTexture*      texture;
	o2::grSprite*       sprite;
	o2::grSprite*       sprite2;
	o2::grSprite*       sprite3;
	o2::grRenderTarget* renderTarget;
	o2::grCamera*       camera;
	
	TestApp();
	~TestApp();

	void onUpdate(float dt);
	void onDraw();

	void processMessage(o2::cApplacationMessage::type message);

	void drawBasis(o2::basis* bas);

	void assTest();
};

#endif //TEST_APP_H