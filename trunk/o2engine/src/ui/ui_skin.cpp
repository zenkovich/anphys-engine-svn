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

uiWidget* uiSkinManager::createWidget(const vec2f& size, const vec2f& position /*= vec2f()*/, const string& id /*= ""*/)
{
	uiWidget* widget = mnew uiWidget(cLayout::fixedSize(position, size), id);
	return widget;
}

uiButton* uiSkinManager::createButton(const string& caption, const cLayout& layout, const string& id /*= ""*/)
{
	uiButton* button = static_cast<uiButton*>(mButtonSample->clone());
	button->setLayout(layout);
	button->setId(id);
	button->setCCaption(caption);

	return button;
}

void uiSkinManager::setButtonSample(uiButton* buttonSample)
{
	mButtonSample = buttonSample;
}

uiRect* uiSkinManager::createBackground(const cLayout& layout /*= uiBothLayout()*/, const string& id /*= ""*/)
{
	uiRect* background = static_cast<uiRect*>(mBackgroundSample->clone());
	background->setLayout(layout);
	background->setId(id);

	return background;
}

void uiSkinManager::setBackgroundSample(uiRect* backgroundSample)
{
	mBackgroundSample = backgroundSample;
}

uiSprite* uiSkinManager::createSprite( const grTexture& texture, const cLayout& layout /*= cLayout::both()*/, 
	                                   const string& id /*= ""*/ )
{
	uiSprite* spriteWidget = mnew uiSprite(layout, id);
	spriteWidget->mSprite.setTexture(texture);
	spriteWidget->mSprite.setTextureSrcRect(fRect(vec2f(), texture.getSize()));

	addVisibleState(spriteWidget);

	return spriteWidget;
}

void uiSkinManager::addVisibleState( uiWidget* widget )
{
	uiTransitionState* state = mnew uiTransitionState("visible");
	state->addProperty(&widget->transparency, 0.0f, 1.0f, 0.5f);
	widget->addState(state);
}

void uiSkinManager::setProgressbarSample( uiProgressBar* progressbarSample )
{
	mProgressBarSample = progressbarSample;
}

uiProgressBar* uiSkinManager::createProgressBar( const cLayout& layout /*= cLayout::both()*/, 
	                                                    const string& id /*= ""*/, float value /*= 0*/, 
														float minValue /*= 0*/, float maxValue /*= 1*/ )
{
	uiProgressBar* progressbar = static_cast<uiProgressBar*>(mProgressBarSample->clone());
	progressbar->setLayout(layout);
	progressbar->setId(id);
	progressbar->setValueRange(minValue, maxValue);
	progressbar->setValue(value);
	return progressbar;
}

CLOSE_O2_NAMESPACE