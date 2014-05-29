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

#include "tests/animation_test.h"
#include "tests/input_msg_test.h"
#include "tests/text_test.h"
#include "tests/ui_test.h"

OPEN_O2_NAMESPACE


TestApp::TestApp():
	cApplication()
{
	setContentSize(vec2i(800, 600));
	setWindowCaption("o2 test");
	//setOption(cApplicationOption::RESIZIBLE, false);

	getFileSystem().setResourcePath("../data/");

	//mTest = mnew InputMsgTest(this, &mInputMessage);
	//mTest = mnew cTextTest();
	mTest = mnew cUITest();
}

TestApp::~TestApp()
{
	safe_release(mTest);
}

void TestApp::onStarted()
{
}

void TestApp::onUpdate( float dt )
{
	mTest->update(dt);
}

void TestApp::onDraw()
{
	mRenderSystem->clear(color4::white());
	mTest->draw();
}

CLOSE_O2_NAMESPACE
