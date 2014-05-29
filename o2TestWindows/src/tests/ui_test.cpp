#include "ui_test.h"

#include "ui/ui_skin.h"
#include "render_system/render_system.h"
#include "ui/ui_controller.h"
#include "ui/ui_sprite.h"
#include "util/graphics/stretch_rect.h"
#include "ui/ui_std_skin_initializer.h"
#include "util/input/input_message.h"
#include "app/application.h"

OPEN_O2_NAMESPACE

cUITest::cUITest()
{
	uiStdSkinInitializer::initialize();

	uiHost()->addWidget( uiSkin()->createBackground() );
	uiHost()->addWidget( uiSkin()->createButton(vec2f(100, 30), vec2f(300, 300)) );
	/*uiHost()->addWidget( uiSkin()->createButton(vec2f(100, 30), vec2f(300, 330)) );
	uiHost()->addWidget( uiSkin()->createButton(vec2f(100, 30), vec2f(300, 360)) );
	uiHost()->addWidget( uiSkin()->createButton(vec2f(100, 30), vec2f(300, 390)) );
	uiHost()->addWidget( uiSkin()->createButton(vec2f(100, 30), vec2f(300, 420)) );*/

	//sprite = mnew grSprite(grTexture::createFromFile("test"));
	sprite = mnew cStretchRect( grTexture::createFromFile("ui_skin/btn_regular"), 10, 10, 14, 14 );
}

void cUITest::update(float dt)
{
	if (appInput()->isCursorDown())
	{
		if (appInput()->isKeyDown('A'))
			sprite->pivot += appInput()->getCursorDelta();
		else if (appInput()->isKeyDown('S'))
			sprite->size += appInput()->getCursorDelta();
		else
			sprite->position += appInput()->getCursorDelta();
	}
}

void cUITest::draw()
{
	sprite->draw();
	renderSystem()->drawCross(sprite->position, 5.0f, color4::green());
}

CLOSE_O2_NAMESPACE