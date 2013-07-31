#include "test_app.h"

#include "util/math/vector2.h"
#include "util/log.h"

OPEN_O2_NAMESPACE
DECLARE_SINGLETON(TestApp);
CLOSE_O2_NAMESPACE

TestApp::TestApp():
	cApplication()
{
	setOption(o2::cApplicationOption::WND_SIZE, o2::vec2i(800, 600));
	setOption(o2::cApplicationOption::WND_CAPTION, (std::string)"bebebe");
}

TestApp::~TestApp()
{

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
