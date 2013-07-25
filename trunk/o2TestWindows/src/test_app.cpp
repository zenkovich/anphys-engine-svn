#include "test_app.h"

#include "util/math/vector2.h"
#include "util/log.h"

OPEN_O2_NAMESPACE
DECLARE_SINGLETON(TestApp);
CLOSE_O2_NAMESPACE

TestApp::TestApp():
	cApplication()
{
	setOptions(o2::cApplicationOption::WND_SIZE, o2::vec2i(800, 600));
	setOptions(o2::cApplicationOption::WND_CAPTION, (std::string)"bebebe");
}

TestApp::~TestApp()
{

}

void TestApp::onUpdate( float dt )
{
	llog("update %.3f", dt);
}
