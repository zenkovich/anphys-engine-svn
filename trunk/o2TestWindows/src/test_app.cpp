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
#include "util/math/color.h"
#include "util/math/vector2.h"
#include "tests/animation_test.h"

OPEN_O2_NAMESPACE

DECLARE_SINGLETON(TestApp);

TestApp::TestApp():
	cApplication()
{
	setOption(cApplicationOption::WND_SIZE, vec2i(800, 600));
	setOption(cApplicationOption::WND_CAPTION, (std::string)"o2 test");
	//setOption(cApplicationOption::RESIZIBLE, false);

	getFileSystem().setResourcePath("../data/");

	mTest = mnew AnimationTest(this, &mInputMessage);
	mRenderSystem->getFontManager()->loadBMFont("arial");
	mText = mnew grText(mRenderSystem, mRenderSystem->getFontManager()->getFont("arial"));
	mText->setText("Prived text\nPrived text");
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
	//mTest->update(dt);

	if (mInputMessage.isKeyDown('M'))
		mText->position = mInputMessage.getCursorPos();

	if (mInputMessage.isKeyPressed('B'))
		mText->shadow = !mText->shadow;

	llog("scale (%.3f %.3f)", mText->getScale().x, mText->getScale().y);
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
	mText->draw();
}

CLOSE_O2_NAMESPACE
