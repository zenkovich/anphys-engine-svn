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

#include "man_field.h"

OPEN_O2_NAMESPACE

DECLARE_SINGLETON(TestApp);

TestApp::TestApp():
	cApplication(), mManField(NULL)
{
	setOption(cApplicationOption::WND_SIZE, vec2i(800, 600));
	setOption(cApplicationOption::WND_CAPTION, (std::string)"Man field test");
	setOption(cApplicationOption::RESIZIBLE, false);

	getFileSystem().setResourcePath("../data/");
}

TestApp::~TestApp()
{
	safe_release(mManField);
}

void TestApp::onInitialized()
{
	mManField = mnew cManField(this);
}

void TestApp::onUpdate( float dt )
{
	mManField->update(dt);
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
	mRenderSystem->clear(color4(0, 100, 0, 255));

	mManField->draw();
}

CLOSE_O2_NAMESPACE
