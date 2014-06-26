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

	mTestBar = uiSkin()->createProgressBar(cLayout::fixedSize(vec2f(400, 140), vec2f(100, 100)));
	uiHost()->addWidget(mTestBar);
}

void cUITest::update(float dt)
{
	if (appInput()->isKeyDown('Y'))
		mTestBar->value += appInput()->getCursorDelta().x*0.0005f;
}

void cUITest::draw()
{
}

CLOSE_O2_NAMESPACE