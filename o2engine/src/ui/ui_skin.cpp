#include "ui_skin.h"

#include "render_system/render_system.h"
#include "render_system/texture.h"

#include "ui_widget.h"
#include "ui_sprite.h"
#include "ui_transition_state.h"
#include "ui_rect.h"
#include "ui_button.h"
#include "ui_progressbar.h"
#include "ui_checkbox.h"
#include "ui_scroll_bar.h"

OPEN_O2_NAMESPACE

DECLARE_SINGLETON(uiSkinManager);

uiWidget* uiSkinManager::widget(const vec2f& size, const vec2f& position /*= vec2f()*/, const string& id /*= ""*/)
{
	uiWidget* widget = mnew uiWidget(cLayout::fixedSize(position, size), id);
	return widget;
}

uiButton* uiSkinManager::button(const string& caption, const cLayout& layout, const string& id /*= ""*/)
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

uiRect* uiSkinManager::background(const cLayout& layout /*= uiBothLayout()*/, const string& id /*= ""*/)
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

uiSprite* uiSkinManager::sprite( const grTexture& texture, const cLayout& layout /*= cLayout::both()*/, 
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

uiProgressBar* uiSkinManager::progressBar( const cLayout& layout /*= cLayout::both()*/, 
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

void uiSkinManager::setCheckBoxSample( uiCheckBox* checkbox )
{
	mCheckBoxSample = checkbox;
}

uiCheckBox* uiSkinManager::checkBox( const string& caption, const cLayout& layout, const string& id /*= ""*/, 
	                                       bool checked /*= false*/ )
{
	uiCheckBox* checkbox = static_cast<uiCheckBox*>(mCheckBoxSample->clone());
	checkbox->setLayout(layout);
	checkbox->setCCaption(caption);
	checkbox->setId(id);
	checkbox->setChecked(checked);
	return checkbox;
}

uiScrollBar* uiSkinManager::horScrollBar( const cLayout& layout /*= cLayout::both()*/, const string& id /*= ""*/, 
	                                            float value /*= 0*/, float minValue /*= 0*/, float maxValue /*= 1*/,
												float barSize /*= 0.1f*/ )
{
	uiScrollBar* scrollbar = static_cast<uiScrollBar*>(mHorScrollbarSample->clone());
	scrollbar->setLayout(layout);
	scrollbar->setId(id);
	scrollbar->setValueRange(minValue, maxValue);
	scrollbar->setValue(value);
	scrollbar->setBarSize(barSize);
	return scrollbar;
}

void uiSkinManager::setHorScrollbarSample( uiScrollBar* scrollbar )
{
	mHorScrollbarSample = scrollbar;
}

CLOSE_O2_NAMESPACE