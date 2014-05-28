#include "ui_std_skin_initializer.h"

#include "ui_button.h"
#include "ui_rect.h"

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
	button->addDrawable(shadowDrawable);
	button->addDrawable(focusDrawable);
	button->addDrawable(regDrawable);
	button->addDrawable(hoverDrawable);
	button->addDrawable(pressedDrawable);

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