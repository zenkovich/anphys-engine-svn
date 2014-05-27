#include "ui_test.h"

#include "ui\ui_skin.h"
#include "render_system\render_system.h"
#include "ui\ui_controller.h"
#include "ui\ui_sprite.h"
#include "ui/ui_std_skin_initializer.h"
#include "util\input\input_message.h"
#include "app\application.h"

OPEN_O2_NAMESPACE

cUITest::cUITest()
{
	uiStdSkinInitializer::initialize();

	uiHost()->addWidget( uiSkin()->createButton(vec2f(100, 30), vec2f(300, 300)) );
}

void cUITest::update(float dt)
{
}

void cUITest::draw()
{

}

CLOSE_O2_NAMESPACE