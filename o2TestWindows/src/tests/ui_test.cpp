#include "ui_test.h"

#include "app/application.h"
#include "render_system/render_system.h"
#include "render_system/text.h"
#include "ui/ui_controller.h"
#include "ui/ui_skin.h"
#include "ui/ui_sprite.h"
#include "ui/ui_std_skin_initializer.h"
#include "util/graphics/stretch_rect.h"
#include "util/input/input_message.h"

OPEN_O2_NAMESPACE

cUITest::cUITest()
{
	uiStdSkinInitializer::initialize();

	uiHost()->addWidget( uiSkin()->createBackground() );
	uiHost()->addWidget( uiSkin()->createButton("Button 1", uiStraightPixelLayout(vec2f(300, 300), vec2f(100, 20))) );
	uiHost()->addWidget( uiSkin()->createButton("Button 2", uiStraightPixelLayout(vec2f(300, 330), vec2f(100, 20))) );
	uiHost()->addWidget( uiSkin()->createButton("Button 3", uiStraightPixelLayout(vec2f(300, 360), vec2f(100, 20))) );
	uiHost()->addWidget( uiSkin()->createButton("Button 4", uiStraightPixelLayout(vec2f(300, 390), vec2f(100, 20))) );
	uiHost()->addWidget( uiSkin()->createButton("Button 5", uiStraightPixelLayout(vec2f(300, 420), vec2f(100, 20))) );

	spr = uiSkin()->createSprite(grTexture::createFromFile("ui_test"), 
		                         uiStraightPixelLayout(vec2f(100, 100), vec2f(100, 100)));

	wdg = spr->clone();
	wdg->position = vec2f(200, 200);
	
	uiHost()->addWidget( spr );
	uiHost()->addWidget( wdg );

	//sprite = mnew grSprite(grTexture::createFromFile("test"));
	//sprite = mnew cStretchRect( grTexture::createFromFile("ui_skin/btn_regular"), 10, 10, 14, 14 );
	
	shared<grText> txt = mnew grText(renderSystem()->getFontManager()->loadBMFont("o2_font/myriad"));
	txt->ctext = "Prived, i'm a text";
	sprite = txt;
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
	
	if (appInput()->isKeyPressed('X'))
		spr->visible = !spr->visible;

	if (appInput()->isKeyPressed('Z'))
		wdg->visible = !wdg->visible;
}

void cUITest::draw()
{
	sprite->draw();
	renderSystem()->drawCross(sprite->position, 5.0f, color4::green());
}

CLOSE_O2_NAMESPACE