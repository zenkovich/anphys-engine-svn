#include "ui_test.h"

#include "ui\ui_skin.h"
#include "render_system\render_system.h"
#include "ui\ui_controller.h"
#include "ui\ui_sprite.h"
#include "util\input\input_message.h"
#include "app\application.h"

OPEN_O2_NAMESPACE

cUITest::cUITest()
{
	spr = uiSkin::createSprite(renderSystem()->getTextureFromFile("ui_test"), vec2f(100, 100));
	uiHost()->addWidget(spr);

	wdg = spr->clone();
	wdg->position = vec2f(100, 200);
	uiHost()->addWidget(wdg);

	rect = uiSkin::createRectangle(renderSystem()->getTextureFromFile("ui_test"), fRect(), 30, 30, 30, 30, vec2f(300, 300),
		                           vec2f(200, 100));
	uiHost()->addWidget(rect);
}

void cUITest::update(float dt)
{
	if (appInput()->isKeyPressed('Q'))
		spr->visible = !spr->visible;
	
	if (appInput()->isKeyPressed('W'))
		wdg->visible = !wdg->visible;
	
	if (appInput()->isKeyPressed('E'))
		rect->visible = !rect->visible;
}

void cUITest::draw()
{

}

CLOSE_O2_NAMESPACE