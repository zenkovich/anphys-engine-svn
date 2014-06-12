#include "ui_test.h"

#include "app/application.h"
#include "render_system/render_system.h"
#include "render_system/text.h"
#include "ui/ui_controller.h"
#include "ui/ui_progressbar.h"
#include "ui/ui_skin.h"
#include "ui/ui_sprite.h"
#include "ui/ui_std_skin_initializer.h"
#include "util/graphics/stretch_rect.h"
#include "util/input/input_message.h"

OPEN_O2_NAMESPACE


cStretchRect* testRT;

cUITest::cUITest()
{
	uiStdSkinInitializer::initialize();

	uiHost()->addWidget( uiSkin()->createBackground() );
	uiHost()->addWidget( uiSkin()->createButton("Button 1", cLayout::fixedSize(vec2f(100, 20), vec2f(300, 300))) );
	uiHost()->addWidget( uiSkin()->createButton("Button 2", cLayout::fixedSize(vec2f(100, 20), vec2f(300, 330))) );
	uiHost()->addWidget( uiSkin()->createButton("Button 3", cLayout::fixedSize(vec2f(100, 20), vec2f(300, 360))) );
	uiHost()->addWidget( uiSkin()->createButton("Button 4", cLayout::fixedSize(vec2f(100, 20), vec2f(300, 390))) );
	uiHost()->addWidget( uiSkin()->createButton("Button 5", cLayout::fixedSize(vec2f(100, 20), vec2f(300, 420))) );

	mTestBar = uiSkin()->createProgressBar(cLayout::fixedSize(vec2f(400, 140), vec2f(100, 300)));
	uiHost()->addWidget(mTestBar);

	spr = uiSkin()->createSprite(grTexture::createFromFile("ui_test"), 
		                         cLayout::fixedSize(vec2f(100, 100), vec2f(100, 100)));

	testRT = mnew cStretchRect(grTexture::createFromFile("ui_test"), 30, 30, 30, 30);
	testRT->rect = fRect(50, 50, 0, 0);

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

		if (appInput()->isKeyDown('V'))
			testRT->size += appInput()->getCursorDelta();
	}
	
	if (appInput()->isKeyPressed('X'))
		spr->visible = !spr->visible;

	if (appInput()->isKeyPressed('Z'))
		wdg->visible = !wdg->visible;

	if (appInput()->isKeyDown('Y'))
		mTestBar->value += appInput()->getCursorDelta().x*0.0005f;
}

void cUITest::draw()
{
	sprite->draw();
	renderSystem()->drawCross(sprite->position, 5.0f, color4::green());
	testRT->draw();
	testRT->drawDebug();
}

CLOSE_O2_NAMESPACE