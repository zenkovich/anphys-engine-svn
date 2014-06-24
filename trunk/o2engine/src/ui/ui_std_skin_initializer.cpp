#include "ui_std_skin_initializer.h"

#include "render_system\render_system.h"
#include "render_system\text.h"
#include "ui_button.h"
#include "ui_rect.h"
#include "ui_progressbar.h"
#include "ui_transition_state.h"
#include "util\geometry\geometry.h"
#include "util\string.h"

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
	button->addDrawable(captionDrawable, "caption", cLayout::both(fRect(-5, -5, -5, -12)));

	//hover state
	uiTransitionState* hoverState = mnew uiTransitionState("hover");
	hoverState->addProperty(static_cast< cPropertyList::Property<float>* >(&hoverDrawable->transparency),
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
	cStretchRect* barDrawable = 
		mnew cStretchRect(barTex, (int)(barLeftBorder - barLeftOffset), 0, (int)(barRightBorder - barRightOffset), 0,  
		                  fRect(barLeftOffset, 0.0f, barTex.getSize().x - barRightOffset, barTex.getSize().y));

	//progerssbar
	uiProgressBar* progressbar = mnew uiProgressBar(cLayout::both());
	cLayout backgroundLayout(vec2f(0.0f, 0.5f), vec2f(0.0f, -backgroundTex.getSize().y*0.5f), 
		                     vec2f(1.0f, 0.5f), vec2f(0.0f, backgroundTex.getSize().y*0.5f));
	progressbar->getBackgroundDrawable()->addChildDrawable("bg", backgroundDrawable, backgroundLayout);

	// adding drawables
	cLayout insideBarLayout(vec2f(0.0f, 0.5f), vec2f(barLeftOffset, -barTex.getSize().y*0.5f), 
		                    vec2f(1.0f, 0.5f), vec2f(-barRightOffset, barTex.getSize().y*0.5f));
	uiProgressBar::Drawable* barInsideDrawable = 
		progressbar->getBarDrawable()->addChildDrawable("barInside",NULL, insideBarLayout);

	barInsideDrawable->addChildDrawable("barrr", barDrawable, cLayout::both());

	mSkinManager->setProgressbarSample(progressbar);
}


CLOSE_O2_NAMESPACE