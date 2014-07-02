#include "ui_std_skin_initializer.h"

#include "render_system/render_system.h"
#include "render_system/text.h"
#include "render_system/sprite.h"
#include "ui_button.h"
#include "ui_checkbox.h"
#include "ui_progressbar.h"
#include "ui_scroll_bar.h"
#include "ui_rect.h"
#include "ui_transition_state.h"
#include "util/geometry/geometry.h"
#include "util/string.h"

OPEN_O2_NAMESPACE

DECLARE_SINGLETON(uiStdSkinInitializer);

uiSkinManager* uiStdSkinInitializer::mSkinManager = NULL;
grFont*        uiStdSkinInitializer::mStdFont = NULL;


void uiStdSkinInitializer::initialize()
{
	mSkinManager = mnew uiSkinManager();

	mStdFont = fontsManager()->loadBMFont("o2_font/myriad");

	initButton();
	initBackground();
	initProgressBar();
	initCheckBox();
	initHorScrollBar();
}

void uiStdSkinInitializer::deinitialize()
{
	safe_release(mSkinManager);
}

void uiStdSkinInitializer::initButton()
{
	const string regDrawableTexture = "ui_skin/btn_regular";
	const string shadowDrawableTexture = "ui_skin/btn_shadow";
	const string hoverDrawableTexture = "ui_skin/btn_hover";
	const string focusDrawableTexture = "ui_skin/btn_glow";
	const string pressedDrawableTexture = "ui_skin/btn_pressed";
	const int rectBorderLeft = 10;
	const int rectBorderTop = 10;
	const int rectBorderRight = 14;
	const int rectBorderBottom = 14;

	const float hoverDelayOn = 0.1f;
	const float hoverDelayOff = 0.3f;
	const float pressingDelayOn = 0.01f;
	const float pressingDelayOff= 0.1f;
	const float focusingDelayOn = 0.1f;
	const float focusingDelayOff= 0.6f;

	uiButton* button = mnew uiButton(cLayout::fixedSize(vec2f(50, 50), vec2f()));

	//drawables
	cStretchRect* regDrawable = mnew cStretchRect(
		grTexture::createFromFile(regDrawableTexture), rectBorderLeft, rectBorderTop, rectBorderRight, rectBorderBottom);
	
	cStretchRect* shadowDrawable = mnew cStretchRect(
		grTexture::createFromFile(shadowDrawableTexture), rectBorderLeft, rectBorderTop, rectBorderRight, rectBorderBottom);
	
	cStretchRect* hoverDrawable = mnew cStretchRect(
		grTexture::createFromFile(hoverDrawableTexture), rectBorderLeft, rectBorderTop, rectBorderRight, rectBorderBottom);
	
	cStretchRect* focusDrawable = mnew cStretchRect(
		grTexture::createFromFile(focusDrawableTexture), rectBorderLeft, rectBorderTop, rectBorderRight, rectBorderBottom);
	
	cStretchRect* pressedDrawable = mnew cStretchRect(
		grTexture::createFromFile(pressedDrawableTexture), rectBorderLeft, rectBorderTop, rectBorderRight, rectBorderBottom);

	grText* captionDrawable = mnew grText(mStdFont);
	captionDrawable->horAlign = grText::HA_CENTER;
	captionDrawable->verAlign = grText::VA_CENTER;

	//adding drawables
	cLayout drawablesLayout = cLayout::both(fRect(-5, -5, -6, -7));
	button->addDrawable(shadowDrawable, "shadow", drawablesLayout);
	button->addDrawable(focusDrawable, "focus", drawablesLayout);
	button->addDrawable(regDrawable, "regular", drawablesLayout);
	button->addDrawable(hoverDrawable, "hover", drawablesLayout);
	button->addDrawable(pressedDrawable, "presed", drawablesLayout);
	button->addDrawable(captionDrawable, "caption", cLayout::both(fRect(-5, -5, -9, -14)));

	//hover state
	uiTransitionState* hoverState = mnew uiTransitionState("hover");
	hoverState->addProperty(&hoverDrawable->transparency,
		cAnimFrame<float>(0.0f, hoverDelayOff, false, IT_LINEAR),
		cAnimFrame<float>(1.0f, hoverDelayOn, false, IT_LINEAR));

	//pressed state
	uiTransitionState* pressedState = mnew uiTransitionState("pressed");
	pressedState->addProperty(&pressedDrawable->transparency, 
		cAnimFrame<float>(0.0f, pressingDelayOff, false, IT_LINEAR),
		cAnimFrame<float>(1.0f, pressingDelayOn, false, IT_LINEAR));

	pressedState->addProperty(&shadowDrawable->transparency, 
		cAnimFrame<float>(1.0f, pressingDelayOff, false, IT_LINEAR),
		cAnimFrame<float>(0.0f, pressingDelayOn, false, IT_LINEAR));

	//focus state
	uiTransitionState* focusState = mnew uiTransitionState("focus");
	focusState->addProperty(&focusDrawable->transparency, 
		cAnimFrame<float>(0.0f, focusingDelayOff, false, IT_LINEAR),
		cAnimFrame<float>(1.0f, focusingDelayOn, false, IT_LINEAR));
	
	//adding states
	button->addState(hoverState);
	button->addState(pressedState);
	button->addState(focusState);

	//set as sample in skin manager
	mSkinManager->setButtonSample(button);
}

void uiStdSkinInitializer::initBackground()
{
	const string basicBackgroundTexName = "ui_skin/background";
	const string shadowTexName = "ui_skin/background_shadow";

	uiRect* background = mnew uiRect(cLayout::fixedSize(vec2f(50, 50), vec2f()), "bgSample");
	uiRect* shadow = mnew uiRect(cLayout::both(), "shadow"); 


	grTexture basicBackgroundTex = grTexture::createFromFile(basicBackgroundTexName);
	grTexture shadowTex = grTexture::createFromFile(shadowTexName);

	shadow->mStretchRect = cStretchRect(1, shadowTex);
	shadow->mStretchRect.addPart(vec2f(0, 0), vec2f(0, 0), vec2f(1, 1), vec2f(0, 0), 
		                         fRect(vec2f(), shadowTex.getSize()));

	background->mStretchRect = cStretchRect(1, basicBackgroundTex);
	background->mStretchRect.addPart(vec2f(0, 0), vec2f(0, 0), vec2f(1, 1), vec2f(0, 0), 
		                             fRect(vec2f(), basicBackgroundTex.getSize()), true);

	background->addChild(shadow);

	mSkinManager->setBackgroundSample(background);
}

void uiStdSkinInitializer::initProgressBar()
{
	const string backgroundTexName = "ui_skin/bar_bk";
	const string barTexName = "ui_skin/bar_bar";

	grTexture backgroundTex = grTexture::createFromFile(backgroundTexName);
	grTexture barTex = grTexture::createFromFile(barTexName);

	//drawables
	const float backgroundLeftBorder = 11;
	const float backgroundRightBorder = 12;
	cStretchRect* backgroundDrawable = mnew cStretchRect(backgroundTex, (int)backgroundLeftBorder, 0, (int)backgroundRightBorder, 0);

	const float barLeftOffset = 6, barRightOffset = 8;
	const float barLeftBorder = 11, barRightBorder = 12;
	cStretchRect* barDrawable = mnew cStretchRect(barTex, (int)barLeftBorder, 0, (int)barRightBorder, 0);

	//progerss bar
	uiProgressBar* progressbar = mnew uiProgressBar(cLayout::both());
	
	// adding drawables
	cLayout backgroundLayout(vec2f(0.0f, 0.5f), vec2f(0.0f, -backgroundTex.getSize().y*0.5f), 
		                     vec2f(1.0f, 0.5f), vec2f(0.0f, backgroundTex.getSize().y*0.5f));
	progressbar->addDrawable(backgroundDrawable, "background", backgroundLayout);

	uiProgressBar::Drawable* barBasicDrw = progressbar->addDrawable(NULL, "barBasic", 
		cLayout(vec2f(0.0f, 0.0f), vec2f(barLeftOffset, 0.0f), vec2f(1.0f, 1.0f), vec2f(-barRightOffset, 0.0)) );

	uiProgressBar::Drawable* barDrw = barBasicDrw->addChildDrawable("bar", NULL, cLayout::both() );

	float hs = barTex.getSize().y*0.5f;
	barDrw->addChildDrawable("bar", barDrawable, cLayout( vec2f(0.0f, 0.5f), vec2f(-barLeftOffset, -hs),
		                                                  vec2f(1.0f, 0.5f), vec2f(barRightOffset, hs) ) );

	progressbar->setBarDrawable(barDrw);


	mSkinManager->setProgressbarSample(progressbar);
}

void uiStdSkinInitializer::initCheckBox()
{
	const string bkTexName = "ui_skin/checkbox_bk";
	const string glowTexName = "ui_skin/checkbox_glow";
	const string checkTexName = "ui_skin/checkbox_check";
	const string hoverTexName = "ui_skin/checkbox_hover";
	const string pressedTexName = "ui_skin/checkbox_pressed";

	const float hoverDelayOn = 0.1f;
	const float hoverDelayOff = 0.3f;
	const float pressingDelayOn = 0.01f;
	const float pressingDelayOff= 0.1f;
	const float focusingDelayOn = 0.1f;
	const float focusingDelayOff= 0.6f;

	const float boxBorder = 3.0f;

	uiCheckBox* checkBox = mnew uiCheckBox(cLayout::both());

	//drawables
	grSprite* bkDrawable = mnew grSprite(grTexture::createFromFile(bkTexName));
	grSprite* glowDrawable = mnew grSprite(grTexture::createFromFile(glowTexName));
	grSprite* checkDrawable = mnew grSprite(grTexture::createFromFile(checkTexName));
	grSprite* hoverDrawable = mnew grSprite(grTexture::createFromFile(hoverTexName));
	grSprite* pressedDrawable = mnew grSprite(grTexture::createFromFile(pressedTexName));

	grText* captionDrawable = mnew grText(mStdFont);
	captionDrawable->horAlign = grText::HA_LEFT;
	captionDrawable->verAlign = grText::VA_CENTER;

	//adding drawables
	vec2f bkSize = bkDrawable->getSize();
	cLayout bkLayout = cLayout(vec2f(0.0f, 0.5f), vec2f(boxBorder, -bkSize.y*0.5f), 
		                       vec2f(0.0f, 0.5f), vec2f(bkSize.x + boxBorder, bkSize.y*0.5f));

	cLayout textLayout = cLayout(vec2f(0.0f, 0.0f), vec2f(bkSize.x + boxBorder*2.0f, 0.0f),
		                         vec2f(1.0f, 1.0f), vec2f(0.0f, 8.0f));
	
	checkBox->addDrawable(glowDrawable, "glow", bkLayout);
	checkBox->addDrawable(bkDrawable, "back", bkLayout);
	checkBox->addDrawable(hoverDrawable, "hover", bkLayout);
	checkBox->addDrawable(pressedDrawable, "pressed", bkLayout);
	checkBox->addDrawable(checkDrawable, "check", bkLayout);
	checkBox->addDrawable(captionDrawable, "caption", textLayout);

	//hover state
	uiTransitionState* hoverState = mnew uiTransitionState("hover");
	hoverState->addProperty(&hoverDrawable->transparency,
		cAnimFrame<float>(0.0f, hoverDelayOff, false, IT_LINEAR),
		cAnimFrame<float>(1.0f, hoverDelayOn, false, IT_LINEAR));

	//pressed state
	uiTransitionState* pressedState = mnew uiTransitionState("pressed");
	pressedState->addProperty(&pressedDrawable->transparency, 
		cAnimFrame<float>(0.0f, pressingDelayOff, false, IT_LINEAR),
		cAnimFrame<float>(1.0f, pressingDelayOn, false, IT_LINEAR));

	//focus state
	uiTransitionState* focusState = mnew uiTransitionState("focus");
	focusState->addProperty(&glowDrawable->transparency, 
		cAnimFrame<float>(0.0f, focusingDelayOff, false, IT_LINEAR),
		cAnimFrame<float>(1.0f, focusingDelayOn, false, IT_LINEAR));

	//check state
	uiTransitionState* checkState = mnew uiTransitionState("checked");
	checkState->addProperty(&checkDrawable->transparency,
		cAnimFrame<float>(0.0f, pressingDelayOff, false, IT_LINEAR),
		cAnimFrame<float>(1.0f, pressingDelayOn, false, IT_LINEAR));
	
	//adding states
	checkBox->addState(hoverState);
	checkBox->addState(pressedState);
	checkBox->addState(focusState);
	checkBox->addState(checkState);

	//set as sample in skin manager
	mSkinManager->setCheckBoxSample(checkBox);
}

void uiStdSkinInitializer::initHorScrollBar()
{
	const string backgroundTexName = "ui_skin/bar_bk";
	const string barTexName = "ui_skin/bar_bar";
	const string barHoverTexName = "ui_skin/bar_bar_hover";
	const string barPressedTexName = "ui_skin/bar_bar_pressed";
	const string barIconTexName = "ui_skin/bar_drag_icn";

	grTexture backgroundTex = grTexture::createFromFile(backgroundTexName);
	grTexture barTex = grTexture::createFromFile(barTexName);
	grTexture barHoverTex = grTexture::createFromFile(barHoverTexName);
	grTexture barPressedTex = grTexture::createFromFile(barPressedTexName);

	const float hoverDelayOn = 0.1f;
	const float hoverDelayOff = 0.3f;
	const float pressingDelayOn = 0.01f;
	const float pressingDelayOff= 0.1f;
	const float barHeight = 13.0f;

	//drawables
	const float backgroundLeftBorder = 11;
	const float backgroundRightBorder = 12;
	cStretchRect* backgroundDrawable = mnew cStretchRect(backgroundTex, (int)backgroundLeftBorder, 0, (int)backgroundRightBorder, 0);

	const float barLeftOffset = 6, barRightOffset = 8;
	const float barLeftBorder = 11, barRightBorder = 12;
	cStretchRect* barDrawable = mnew cStretchRect(barTex, (int)barLeftBorder, 0, (int)barRightBorder, 0);
	cStretchRect* barHoverDrawable = mnew cStretchRect(barHoverTex, (int)barLeftBorder, 0, (int)barRightBorder, 0);
	cStretchRect* barPresedDrawable = mnew cStretchRect(barPressedTex, (int)barLeftBorder, 0, (int)barRightBorder, 0);

	grSprite* barIconDrawable = new grSprite(grTexture::createFromFile(barIconTexName));

	//scrollbar
	uiScrollBar* scrollbar = mnew uiScrollBar(cLayout::both());

	// adding drawables
	cLayout backgroundLayout(vec2f(0.0f, 0.5f), vec2f(0.0f, -backgroundTex.getSize().y*0.5f), 
		                     vec2f(1.0f, 0.5f), vec2f(0.0f, backgroundTex.getSize().y*0.5f));
	scrollbar->addDrawable(backgroundDrawable, "background", backgroundLayout);

	uiScrollBar::Drawable* barDrw = scrollbar->addDrawable(NULL, "bar");
	scrollbar->setBarDrawable(barDrw);
	
	float hs = barTex.getSize().y*0.5f;
	cLayout barLayout(vec2f(0.0f, 0.5f), vec2f(0.0f, -hs), vec2f(1.0f, 0.5f), vec2f(0.0f, hs));
	barDrw->addChildDrawable("bar", barDrawable, barLayout);
	barDrw->addChildDrawable("hover", barHoverDrawable, barLayout);
	barDrw->addChildDrawable("Pressed", barPresedDrawable, barLayout);

	vec2f icnSize = barIconDrawable->getSize();
	vec2f icnOffs(0, 0.0f);
	barDrw->addChildDrawable("icon", barIconDrawable, 
		cLayout(vec2f(0.5f, 0.5f), icnSize*(-0.5f) + icnOffs, vec2f(0.5f, 0.5f), icnSize*0.5f + icnOffs));

	//states
	uiTransitionState* hoverState = mnew uiTransitionState("hover");
	hoverState->addProperty(&barHoverDrawable->transparency,
		cAnimFrame<float>(0.0f, hoverDelayOff, false, IT_LINEAR),
		cAnimFrame<float>(1.0f, hoverDelayOn, false, IT_LINEAR));

	//pressed state
	uiTransitionState* pressedState = mnew uiTransitionState("pressed");
	pressedState->addProperty(&barPresedDrawable->transparency, 
		cAnimFrame<float>(0.0f, pressingDelayOff, false, IT_LINEAR),
		cAnimFrame<float>(1.0f, pressingDelayOn, false, IT_LINEAR));

	scrollbar->addState(hoverState);
	scrollbar->addState(pressedState);

	//geometry
	scrollbar->setBackgroundGeometryLayout(
		cLayout(vec2f(0.0f, 0.5f), vec2f(barLeftOffset, -barHeight*0.5f), vec2f(1.0f, 0.5f), vec2f(-barRightOffset, barHeight*0.5f)));

	scrollbar->setBarGeometryLayout(
		cLayout(vec2f(0.0f, 0.5f), vec2f(barLeftOffset, -barHeight*0.5f), vec2f(1.0f, 0.5f), vec2f(-barRightOffset, barHeight*0.5f)));

	mSkinManager->setHorScrollbarSample(scrollbar);
}


CLOSE_O2_NAMESPACE