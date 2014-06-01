#include "ui_std_skin_initializer.h"

#include "ui_button.h"
#include "ui_rect.h"
#include "ui_transition_state.h"
#include "util\string.h"
#include "util\geometry\geometry.h"

OPEN_O2_NAMESPACE

DECLARE_SINGLETON(uiStdSkinInitializer);

shared<uiSkinManager> uiStdSkinInitializer::mSkinManager = NULL;


void uiStdSkinInitializer::initialize()
{
	mSkinManager = mnew uiSkinManager();

	initButton();
	initBackground();
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

	shared<uiButton> button = mnew uiButton(uiStaightPixelLayout(vec2f(), vec2f(50, 50)));

	//drawables
	shared<cStretchRect> regDrawable = mnew cStretchRect(
		grTexture::createFromFile(regDrawableTexture), rectBorderLeft, rectBorderTop, rectBorderRight, rectBorderBottom);
	
	shared<cStretchRect> shadowDrawable = mnew cStretchRect(
		grTexture::createFromFile(shadowDrawableTexture), rectBorderLeft, rectBorderTop, rectBorderRight, rectBorderBottom);
	
	shared<cStretchRect> hoverDrawable = mnew cStretchRect(
		grTexture::createFromFile(hoverDrawableTexture), rectBorderLeft, rectBorderTop, rectBorderRight, rectBorderBottom);
	
	shared<cStretchRect> focusDrawable = mnew cStretchRect(
		grTexture::createFromFile(focusDrawableTexture), rectBorderLeft, rectBorderTop, rectBorderRight, rectBorderBottom);
	
	shared<cStretchRect> pressedDrawable = mnew cStretchRect(
		grTexture::createFromFile(pressedDrawableTexture), rectBorderLeft, rectBorderTop, rectBorderRight, rectBorderBottom);

	//adding drawables
	int shadowDrawableIdx = button->addDrawable(shadowDrawable);
	int focusDrawableIdx = button->addDrawable(focusDrawable);
	int regDrawableIdx = button->addDrawable(regDrawable);
	int hoverDrawableIdx = button->addDrawable(hoverDrawable);
	int pressedDrawableIdx = button->addDrawable(pressedDrawable);

	//hover state
	shared<uiTransitionState> hoverState = mnew uiTransitionState("hover");
	hoverState->addProperty(
		format("drawable_%i_transparency", hoverDrawableIdx), 
		cAnimFrame<float>(0.0f, hoverDelayOff, false, IT_LINEAR),
		cAnimFrame<float>(1.0f, hoverDelayOn, false, IT_LINEAR));

	//pressed state
	shared<uiTransitionState> pressedState = mnew uiTransitionState("pressed");
	pressedState->addProperty(
		format("drawable_%i_transparency", pressedDrawableIdx), 
		cAnimFrame<float>(0.0f, pressingDelayOff, false, IT_LINEAR),
		cAnimFrame<float>(1.0f, pressingDelayOn, false, IT_LINEAR));

	pressedState->addProperty(
		format("drawable_%i_transparency", shadowDrawableIdx), 
		cAnimFrame<float>(1.0f, pressingDelayOff, false, IT_LINEAR),
		cAnimFrame<float>(0.0f, pressingDelayOn, false, IT_LINEAR));

	//focus state
	shared<uiTransitionState> focusState = mnew uiTransitionState("focus");
	focusState->addProperty(
		format("drawable_%i_transparency", focusDrawableIdx), 
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

	shared<uiRect> background = mnew uiRect(uiStaightPixelLayout(vec2f(), vec2f(50, 50)), "bgSample");
	shared<uiRect> shadow = mnew uiRect(uiBothLayout(vec2f()), "shadow"); 


	grTexture basicBackgroundTex = grTexture::createFromFile(basicBackgroundTexName);
	grTexture shadowTex = grTexture::createFromFile(shadowTexName);

	shadow->mStretchRect = cStretchRect(1, shadowTex);
	shadow->mStretchRect.addPart(vec2f(0, 0), vec2f(0, 0), vec2f(1, 1), vec2f(0, 0), 
		                         fRect(vec2f(), shadowTex.getSize()));

	background->mStretchRect = cStretchRect(1, basicBackgroundTex);
	background->mStretchRect.addPart(vec2f(0, 0), vec2f(0, 0), vec2f(1, 1), vec2f(0, 0), 
		                             fRect(vec2f(), basicBackgroundTex.getSize()), true);

	background->addChild(shadow);

	mSkinManager->setBackgroundSamble(background);
}

CLOSE_O2_NAMESPACE