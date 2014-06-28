#include "ui_test.h"

#include "app/application.h"
#include "render_system/render_system.h"
#include "render_system/text.h"
#include "ui/ui_controller.h"
#include "ui/ui_progressbar.h"
#include "ui/ui_scroll_bar.h"
#include "ui/ui_checkbox.h"
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

	uiHost()->addWidget( uiSkin()->background() );
	uiHost()->addWidget( uiSkin()->button("Button 1", cLayout::fixedSize(vec2f(100, 20), vec2f(300, 300))) );
	uiHost()->addWidget( uiSkin()->button("Button 2", cLayout::fixedSize(vec2f(100, 20), vec2f(300, 330))) );
	uiHost()->addWidget( uiSkin()->button("Button 3", cLayout::fixedSize(vec2f(100, 20), vec2f(300, 360))) );
	uiHost()->addWidget( uiSkin()->button("Button 4", cLayout::fixedSize(vec2f(100, 20), vec2f(300, 390))) );
	uiHost()->addWidget( uiSkin()->button("Button 5", cLayout::fixedSize(vec2f(100, 20), vec2f(300, 420))) );
	
	uiHost()->addWidget( uiSkin()->checkBox("Check test", cLayout::fixedSize(vec2f(150, 20), vec2f(20, 200))) );
	uiHost()->addWidget( uiSkin()->checkBox("Check test2", cLayout::fixedSize(vec2f(150, 20), vec2f(20, 230))) );
	uiHost()->addWidget( uiSkin()->checkBox("Check test3", cLayout::fixedSize(vec2f(150, 20), vec2f(20, 260))) );
	uiHost()->addWidget( uiSkin()->checkBox("Check test4", cLayout::fixedSize(vec2f(150, 20), vec2f(20, 290))) );

	mTestBar = uiSkin()->progressBar(cLayout::fixedSize(vec2f(400, 40), vec2f(100, 100)));
	mTestScrollBar = uiSkin()->horScrollBar(cLayout::fixedSize(vec2f(400, 40), vec2f(100, 140)));
	mTestScrollBar->setValueRange(10, 100);
	mTestScrollBar->setBarSize(20);

	uiHost()->addWidget(mTestBar);
	uiHost()->addWidget(mTestScrollBar);
}

void cUITest::update(float dt)
{
	if (appInput()->isKeyDown('Y'))
		mTestBar->value += appInput()->getCursorDelta().x*0.0005f;

	mTestBar->value = (mTestScrollBar->value - mTestScrollBar->minValue)/(mTestScrollBar->maxValue - mTestScrollBar->minValue);
}

void cUITest::draw()
{
}

CLOSE_O2_NAMESPACE