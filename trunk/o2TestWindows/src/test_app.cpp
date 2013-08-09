#include "test_app.h"

#include "render_system/mesh.h"
#include "render_system/render_system.h"
#include "render_system/texture.h"
#include "util/file_system/file_system.h"
#include "util/log.h"
#include "util/math/color.h"
#include "util/math/vector2.h"

OPEN_O2_NAMESPACE
DECLARE_SINGLETON(TestApp);
CLOSE_O2_NAMESPACE

TestApp::TestApp():
	cApplication()
{
	setOption(o2::cApplicationOption::WND_SIZE, o2::vec2i(800, 600));
	setOption(o2::cApplicationOption::WND_CAPTION, (std::string)"bebebe");

	texture = NULL;
	mesh = NULL;

	o2::getFileSystem().setResourcePath("../data/");
}

TestApp::~TestApp()
{
	safe_release(mesh);
	mRenderSystem->removeTexture(texture);
}

void TestApp::onUpdate( float dt )
{
	//llog("update %.3f", dt);
	o2::vec2i offs;
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
		setOption(o2::cApplicationOption::RESIZIBLE, !mWindowResizible);
}

void TestApp::processMessage( o2::cApplacationMessage::type message )
{
	cApplication::processMessage(message);

	if (message == o2::cApplacationMessage::ON_STARTED)
	{
		texture = mRenderSystem->createTexture("test");
		mesh = new o2::grMesh(mRenderSystem, NULL);

		o2::color4 colr(0, 255, 0, 255);
		
		mesh->mVerticies[0] = o2::vertex2(0, 0, colr.ABGR(), 0, 0);
		mesh->mVerticies[1] = o2::vertex2(100, 0, colr.ABGR(), 1, 0);
		mesh->mVerticies[2] = o2::vertex2(100, 100, colr.ABGR(), 1, 1);
		mesh->mVerticies[3] = o2::vertex2(0, 100, colr.ABGR(), 0, 1);
		
		mesh->mIndexes[0] = 0;
		mesh->mIndexes[1] = 1;
		mesh->mIndexes[2] = 2;
		
		mesh->mIndexes[3] = 0;
		mesh->mIndexes[4] = 2;
		mesh->mIndexes[5] = 3;

		mesh->mVertexCount = 4;
		mesh->mPolyCount = 2;
	}
}

void TestApp::onDraw()
{
	mesh->draw();
}
