#include "test_app.h"

#include "render_system/mesh.h"
#include "render_system/render_system.h"
#include "render_system/sprite.h"
#include "render_system/texture.h"
#include "render_system/render_target.h"
#include "render_system/camera.h"
#include "util/file_system/file_system.h"
#include "util/log.h"
#include "util/math/color.h"
#include "util/math/vector2.h"
#include "util/math/basis.h"

OPEN_O2_NAMESPACE
DECLARE_SINGLETON(TestApp);
CLOSE_O2_NAMESPACE

TestApp::TestApp():
	cApplication()
{
	setOption(o2::cApplicationOption::WND_SIZE, o2::vec2i(800, 600));
	setOption(o2::cApplicationOption::WND_CAPTION, (std::string)"bebebe");

	texture = NULL;

	o2::getFileSystem().setResourcePath("../data/");
}

TestApp::~TestApp()
{
	mRenderSystem->removeTexture(texture);
}

void TestApp::onUpdate( float dt )
{
	//llog("update %.3f", dt);
	/*o2::vec2i offs;
	if (mInputMessage.isKeyPressed(VK_LEFT))
		offs = o2::vec2i(-10, 0);
	if (mInputMessage.isKeyPressed(VK_RIGHT))
		offs = o2::vec2i(10, 0);
	if (mInputMessage.isKeyPressed(VK_UP))
		offs = o2::vec2i(0, -10);
	if (mInputMessage.isKeyPressed(VK_DOWN))
		offs = o2::vec2i(0, 10);

	if (offs != o2::vec2i(0, 0))
	{
		if (mInputMessage.isKeyDown(VK_CONTROL))
		{
			setOption(o2::cApplicationOption::WND_SIZE, mWindowedSize + offs);
		}
		else
		{
			hlog("move wnd %i %i + %i %i", mWindowedPos.x, mWindowedPos.y, offs.x, offs.y);
			setOption(o2::cApplicationOption::WND_POSITION, mWindowedPos + offs);
		}
	}

	if (mInputMessage.isKeyPressed('Z'))
		setOption(o2::cApplicationOption::RESIZIBLE, !mWindowResizible);*/
	
	if (mInputMessage.isKeyDown(VK_LEFT))
		camera->mPosition.x -= 100.0f*dt;
	
	if (mInputMessage.isKeyDown(VK_RIGHT))
		camera->mPosition.x += 100.0f*dt;
	
	if (mInputMessage.isKeyDown(VK_UP))
		camera->mPosition.y -= 100.0f*dt;
	
	if (mInputMessage.isKeyDown(VK_DOWN))
		camera->mPosition.y += 100.0f*dt;
	
	if (mInputMessage.isKeyDown(VK_NUMPAD4))
		camera->mRotation -= o2::deg2rad(100.0f*dt);
	
	if (mInputMessage.isKeyDown(VK_NUMPAD6))
		camera->mRotation += o2::deg2rad(100.0f*dt);
	
	if (mInputMessage.isKeyDown('O'))
	{
		camera->mScale.x *= 1.0f - 0.5f*dt;
		camera->mScale.y *= 1.0f - 0.5f*dt;
	}
	
	if (mInputMessage.isKeyDown('I'))
	{
		camera->mScale.x *= 1.0f + 0.5f*dt;
		camera->mScale.y *= 1.0f + 0.5f*dt;
	}
	
}

void TestApp::processMessage( o2::cApplacationMessage::type message )
{
	cApplication::processMessage(message);

	if (message == o2::cApplacationMessage::ON_STARTED)
	{
		texture = mRenderSystem->createTexture("pic");

		sprite = new o2::grSprite(mRenderSystem, texture);
		sprite->setSize(mRenderSystem->getResolution().castTo<float>());

		sprite2 = new o2::grSprite(mRenderSystem);
		sprite2->setColor(o2::color4(255, 100, 100, 100));

		renderTarget = new o2::grRenderTarget(mRenderSystem, o2::vec2f(100, 100));
		sprite3 = new o2::grSprite(mRenderSystem, renderTarget->getTexture());
		sprite3->setScale(o2::vec2f(0.3f, 0.3f));

		camera = new o2::grCamera();
		
		mRenderSystem->setLinesWidth(2);
	}
	else if (message == o2::cApplacationMessage::ON_SIZING)
	{
		draw();
	}
}

void TestApp::onDraw()
{
	mRenderSystem->clear(o2::color4(100, 100, 255, 255));

	sprite->draw();

	mRenderSystem->bindRenderTarget(renderTarget);
	mRenderSystem->clear(o2::color4(0, 0, 0, 255));
	mRenderSystem->bindCamera(camera);
	sprite->draw();
	mRenderSystem->bindCamera(NULL);
	mRenderSystem->unbindRenderTarget();

	sprite3->draw();

	sprite2->setSize(mRenderSystem->getResolution().castTo<float>().scale(camera->mScale)).
		     setAngle(o2::rad2deg(camera->mRotation)).
			 setPosition(camera->mPosition).draw();
}

void TestApp::drawBasis( o2::basis* bas )
{
	unsigned long redColor   = o2::color4::dword(255, 0, 0, 255);
	unsigned long greenColor = o2::color4::dword(0, 255, 0, 255);

	const float basisSize = 30;

	o2::vertex2 linesBuf[] = { o2::vertex2(bas->offs.x, bas->offs.y, redColor, 0, 0),
	                           o2::vertex2(bas->offs.x + bas->xv.x*basisSize, bas->offs.y + bas->xv.y*basisSize, redColor, 0, 0),

							   o2::vertex2(bas->offs.x, bas->offs.y, greenColor, 0, 0),
							   o2::vertex2(bas->offs.x + bas->yv.x*basisSize, bas->offs.y + bas->yv.y*basisSize, greenColor, 0, 0) };

	mRenderSystem->drawLines(linesBuf, 2);
}
