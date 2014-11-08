#include "ui_test.h"

#include "app/application.h"
#include "render_system/render_system.h"
#include "render_system/text.h"
#include "ui/ui.h"
#include "util/graphics/stretch_rect.h"
#include "util/input/input_message.h"
#include "../../../o2engine/src/util/file_system/file_system.h"

OPEN_O2_NAMESPACE

uiWidget* testStretchingWdg;

void AddTestStretchButton() 
{
	float height = random(5.0f, 25.0f);
	testStretchingWdg->addChild( uiSkin()->button("Some button", cLayout::both().fixHeight(height).minHeight(height)) );
}

cUITest::cUITest()
{
	uiStdSkinInitializer::initialize();

	uiHost()->addWidget( uiSkin()->background() );

	uiHorLayout* horLayout = uiHost()->addTWidget( uiSkin()->horLayout(cLayout::both().maxHeight(200.0f), "hor", 10.0f) );
	horLayout->setChildsLayout(cLayout::both( fRect(10.0f, 10.0f, 10.0f, 10.0f) ));

	uiVerLayout* verLayout1 = horLayout->addTChild( uiSkin()->verLayout(cLayout::both(), "ver1") );
	verLayout1->addChild( uiSkin()->button("Button 1", cLayout::both().maxWidth(100.0f).minWidth(100.0f)) );
	verLayout1->addChild( uiSkin()->button("Button 1", cLayout::both()) );
	verLayout1->addChild( uiSkin()->button("Button 1", cLayout::both()) );
	verLayout1->addChild( uiSkin()->button("Button 1", cLayout::both()) );

	uiVerLayout* verLayout2 = horLayout->addTChild( uiSkin()->verLayout(cLayout::both(), "ver2") );
	verLayout2->addChild( uiSkin()->button("Button 1", cLayout::both()) );
	verLayout2->addChild( uiSkin()->button("Button 1", cLayout::both()) );
	verLayout2->addChild( uiSkin()->button("Button 1", cLayout::both()) );
	verLayout2->addChild( uiSkin()->button("Button 1", cLayout::both()) );

	uiWidget* pad = uiHost()->addWidget( uiSkin()->rectPad( cLayout::fixed(vec2f(200.0f, 200.0f), vec2f(100.0f, 210.0f)) ) );
	pad->setRisizeByChilds(true);
	testStretchingWdg = pad->addChild( uiSkin()->verLayout() );
	testStretchingWdg->setRisizeByChilds(true);
	uiButton* btn = testStretchingWdg->addTChild( uiSkin()->button("Add button", cLayout::both().fixHeight(25.0f)) );
	btn->onClickEvent.add( callback(&AddTestStretchButton) );

	cPathInfo rePathInfo = fileSystem()->getPathInfo(fileSystem()->getResourcePath());
	/*uiHost()->addWidget( uiSkin()->button("Button 1", cLayout::fixedSize(vec2f(100, 20), vec2f(300, 300))) );
	uiHost()->addWidget( uiSkin()->button("Button 2", cLayout::fixedSize(vec2f(100, 20), vec2f(300, 330))) );
	uiHost()->addWidget( uiSkin()->button("Button 3", cLayout::fixedSize(vec2f(100, 20), vec2f(300, 360))) );
	uiHost()->addWidget( uiSkin()->button("Button 4", cLayout::fixedSize(vec2f(100, 20), vec2f(300, 390))) );
	uiHost()->addWidget( uiSkin()->button("Button 5", cLayout::fixedSize(vec2f(100, 20), vec2f(300, 420))) );
	
	uiHost()->addWidget( uiSkin()->checkBox("Check test", cLayout::fixedSize(vec2f(150, 20), vec2f(20, 200))) );
	uiHost()->addWidget( uiSkin()->checkBox("Check test2", cLayout::fixedSize(vec2f(150, 20), vec2f(20, 220))) );
	uiHost()->addWidget( uiSkin()->checkBox("Check test3", cLayout::fixedSize(vec2f(150, 20), vec2f(20, 240))) );
	uiHost()->addWidget( uiSkin()->checkBox("Check test4", cLayout::fixedSize(vec2f(150, 20), vec2f(20, 260))) );
	
	uiHost()->addWidget( uiSkin()->editbox(cLayout::fixedSize(vec2f(80, 23), vec2f(440, 50)), "editbox", "Some text") );
	uiHost()->addWidget( uiSkin()->editbox(cLayout::fixedSize(vec2f(150, 150), vec2f(440, 240)), "editbox", "Some text\nDAffaS", true) );

	uiHost()->addWidget( uiSkin()->label("I'm label", cLayout::fixedSize(vec2f(100, 30), vec2f(130, 50)), "label") );
	uiHost()->addWidget( uiSkin()->horThinScrollBar(cLayout::fixedSize(vec2f(400, 40), vec2f(100, 120))) );
	uiHost()->addWidget( uiSkin()->verThinScrollBar(cLayout::fixedSize(vec2f(40, 400), vec2f(600, 30))) );

	mTestBar = uiSkin()->progressBar(cLayout::fixedSize(vec2f(400, 40), vec2f(100, 100)));
	mTestScrollBar = uiSkin()->horScrollBar(cLayout::fixedSize(vec2f(400, 40), vec2f(100, 140)));
	mTestScrollBar->setValueRange(10, 100);
	mTestScrollBar->setBarSize(20);

	uiHost()->addWidget(mTestBar);
	uiHost()->addWidget(mTestScrollBar);

	mTestScrollArea = uiSkin()->scrollArea(cLayout::fixedSize(vec2f(200, 200), vec2f(30, 300)), "scrollArea");
	uiHost()->addWidget(mTestScrollArea);
	
	mTestScrollArea->addChild( uiSkin()->button("Button 1", cLayout::fixedSize(vec2f(100, 20), vec2f(20, 20))) );
	mTestScrollArea->addChild( uiSkin()->button("Button 2", cLayout::fixedSize(vec2f(100, 20), vec2f(20, 50))) );
	mTestScrollArea->addChild( uiSkin()->button("Button 3", cLayout::fixedSize(vec2f(100, 20), vec2f(20, 80))) );
	mTestScrollArea->addChild( uiSkin()->button("Button 4", cLayout::fixedSize(vec2f(100, 20), vec2f(20, 110))) );
	mTestScrollArea->addChild( uiSkin()->button("Button 5", cLayout::fixedSize(vec2f(100, 20), vec2f(20, 140))) );
	mTestScrollArea->addChild( uiSkin()->button("Button 5", cLayout::fixedSize(vec2f(100, 20), vec2f(20, 170))) );
	mTestScrollArea->addChild( uiSkin()->button("Button 5", cLayout::fixedSize(vec2f(100, 20), vec2f(20, 200))) );
	mTestScrollArea->addChild( uiSkin()->button("Button 5", cLayout::fixedSize(vec2f(100, 20), vec2f(20, 230))) );
	mTestScrollArea->addChild( uiSkin()->button("Button 5", cLayout::fixedSize(vec2f(100, 20), vec2f(20, 260))) );
	mTestScrollArea->addChild( uiSkin()->button("Button 5", cLayout::fixedSize(vec2f(100, 20), vec2f(20, 290))) );

	mTestScrollArea->addChild( uiSkin()->horScrollBar(cLayout::fixedSize(vec2f(300, 20), vec2f(130, 20)), "scrollBar" ) );*/
}

void cUITest::update(float dt)
{
	/*if (appInput()->isKeyDown('Y'))
		mTestBar->value += appInput()->getCursorDelta().x*0.0005f;

	mTestBar->value = (mTestScrollBar->value - mTestScrollBar->minValue)/(mTestScrollBar->maxValue - mTestScrollBar->minValue);
	*/
	/*if (appInput()->isCursorPressed())
		uiHost()->showHint("Hint some text\nWith two lines (Ctrl + H)", appInput()->getCursorPos());*/

	//hlog("Cursor %.1f %.1f", appInput()->getCursorPos().x, appInput()->getCursorPos().y);
}

void cUITest::draw()
{
}

CLOSE_O2_NAMESPACE