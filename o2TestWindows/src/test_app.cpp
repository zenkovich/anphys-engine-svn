#include "test_app.h"

#include "render_system/mesh.h"
#include "render_system/render_system.h"
#include "render_system/sprite.h"
#include "render_system/texture.h"
#include "render_system/render_target.h"
#include "render_system/font_manager.h"
#include "render_system/text.h"
#include "render_system/camera.h"
#include "util/file_system/file_system.h"
#include "util/log.h"
#include "util/time_utils.h"
#include "util/scheduler.h"
#include "util/math/color.h"
#include "util/math/vector2.h"

#include "tests/animation_test.h"
#include "tests/input_msg_test.h"
#include "tests/text_test.h"

OPEN_O2_NAMESPACE

TestApp::TestApp():
	cApplication()
{
	setOption(cApplicationOption::WND_SIZE, vec2i(800, 600));
	setOption(cApplicationOption::WND_CAPTION, (string)"o2 test");
	//setOption(cApplicationOption::RESIZIBLE, false);

	getFileSystem().setResourcePath("../data/");

	//mTest = mnew InputMsgTest(this, &mInputMessage);
	mTest = mnew cTextTest();
}

TestApp::~TestApp()
{
	safe_release(mTest);
}

void TestApp::onInitialized()
{
}

void TestApp::onUpdate( float dt )
{
	mTest->update(dt);
}

void TestApp::processMessage( cApplacationMessage::type message )
{
	cApplication::processMessage(message);

	if (message == cApplacationMessage::ON_STARTED)
	{
		onInitialized();
	}
	else if (message == cApplacationMessage::ON_SIZING)
	{
		draw();
	}
}

void TestApp::onDraw()
{
	mRenderSystem->clear(color4(100, 0, 0, 255));
	mTest->draw();
}

CLOSE_O2_NAMESPACE
