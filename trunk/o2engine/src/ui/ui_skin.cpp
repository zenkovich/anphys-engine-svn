#include "ui_skin.h"

#include "render_system/render_system.h"
#include "render_system/texture.h"

#include "ui_widget.h"
#include "ui_sprite.h"
#include "ui_transition_state.h"
#include "ui_rect.h"
#include "ui_button.h"
#include "ui_progressbar.h"

OPEN_O2_NAMESPACE

DECLARE_SINGLETON(uiSkinManager);

shared<uiWidget> uiSkinManager::createWidget(const vec2f& size, const vec2f& position /*= vec2f()*/, const string& id /*= ""*/)
{
	shared<uiWidget> widget = mnew uiWidget(cLayout::fixedSize(position, size), id);
	return widget;
}

shared<uiButton> uiSkinManager::createButton(const string& caption, const cLayout& layout, const string& id /*= ""*/)
{
	shared<uiButton> button = mButtonSample->clone();
	button->setLayout(layout);
	button->setId(id);
	button->setCCaption(caption);

	return button;
}

void uiSkinManager::setButtonSample(const shared<uiButton>& buttonSample)
{
	mButtonSample = buttonSample;
}

shared<uiRect> uiSkinManager::createBackground(const cLayout& layout /*= uiBothLayout()*/, const string& id /*= ""*/)
{
	shared<uiRect> background = mBackgroundSample->clone();
	background->setLayout(layout);
	background->setId(id);

	return background;
}

void uiSkinManager::setBackgroundSample(const shared<uiRect>& backgroundSample)
{
	mBackgroundSample = backgroundSample;
}

shared<uiSprite> uiSkinManager::createSprite( const grTexture& texture, const cLayout& layout /*= cLayout::both()*/, const string& id /*= ""*/ )
{
	shared<uiSprite> spriteWidget = mnew uiSprite(layout, id);
	spriteWidget->mSprite.setTexture(texture);
	spriteWidget->mSprite.setTextureSrcRect(fRect(vec2f(), texture.getSize()));

	addVisibleState(spriteWidget);

	return spriteWidget;
}

void uiSkinManager::addVisibleState( const shared<uiWidget>& widget )
{
	shared<uiTransitionState> state = mnew uiTransitionState("visible");
	state->addProperty(widget->transparency, 0.0f, 1.0f, 0.5f);
	widget->addState(state);
}

void uiSkinManager::setProgressbarSample( const shared<uiProgressBar>& progressbarSample )
{
	mProgressBarSample = progressbarSample;
}

shared<uiProgressBar> uiSkinManager::createProgressBar( const cLayout& layout /*= cLayout::both()*/, 
	                                                    const string& id /*= ""*/, float value /*= 0*/, 
														float minValue /*= 0*/, float maxValue /*= 1*/ )
{
	shared<uiProgressBar> progressbar = mProgressBarSample->clone();
	progressbar->setLayout(layout);
	progressbar->setId(id);
	progressbar->setValueRange(minValue, maxValue);
	progressbar->setValue(value);
	return progressbar;
}

CLOSE_O2_NAMESPACE