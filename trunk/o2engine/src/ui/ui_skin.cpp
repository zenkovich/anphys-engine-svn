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
#include "ui_editbox.h"
#include "ui_label.h"
#include "ui_scrollarea.h"
#include "ui_hor_layout.h"
#include "ui_ver_layout.h"

OPEN_O2_NAMESPACE

DECLARE_SINGLETON(uiSkinManager);

uiSkinManager::uiSkinManager()
{
	mWidgetIdx = 0;
}

uiWidget* uiSkinManager::widget(const vec2f& size, const vec2f& position /*= vec2f()*/, const string& id /*= ""*/)
{
	uiWidget* widget = mnew uiWidget(layout::fixed(position, size), checkEmptyId(id, "widget"));
	return widget;
}

uiButton* uiSkinManager::button(const string& caption, const layout& layout, const string& id /*= ""*/)
{
	uiButton* button = static_cast<uiButton*>(mButtonSample->clone());
	button->setLayout(layout);
	button->setId(checkEmptyId(id, "button"));
	button->setCCaption(caption);

	return button;
}

void uiSkinManager::setButtonSample(uiButton* buttonSample)
{
	mButtonSample = buttonSample;
}

uiRect* uiSkinManager::background(const layout& layout /*= uiBothLayout()*/, const string& id /*= ""*/)
{
	uiRect* background = static_cast<uiRect*>(mBackgroundSample->clone());
	background->setLayout(layout);
	background->setId(checkEmptyId(id, "background"));

	return background;
}

void uiSkinManager::setBackgroundSample(uiRect* backgroundSample)
{
	mBackgroundSample = backgroundSample;
}

uiSprite* uiSkinManager::sprite( const grTexture& texture, const layout& layout /*= cLayout::both()*/, 
	                                   const string& id /*= ""*/ )
{
	uiSprite* spriteWidget = mnew uiSprite(layout, checkEmptyId(id, "sprite"));
	spriteWidget->mSprite.setTexture(texture);
	spriteWidget->mSprite.setTextureSrcRect(fRect(vec2f(), texture.getSize()));

	addVisibleState(spriteWidget);

	return spriteWidget;
}

void uiSkinManager::addVisibleState( uiWidget* widget, float duration /*= 0.5f*/ )
{
	uiTransitionState* state = mnew uiTransitionState("visible");
	state->addProperty(&widget->transparency, 0.0f, 1.0f, duration);
	widget->addState(state);
}

void uiSkinManager::setProgressbarSample( uiProgressBar* progressbarSample )
{
	mProgressBarSample = progressbarSample;
}

uiProgressBar* uiSkinManager::progressBar( const layout& layout /*= cLayout::both()*/, 
	                                       const string& id /*= ""*/, float value /*= 0*/, 
										   float minValue /*= 0*/, float maxValue /*= 1*/ )
{
	uiProgressBar* progressbar = static_cast<uiProgressBar*>(mProgressBarSample->clone());
	progressbar->setLayout(layout);
	progressbar->setId(checkEmptyId(id, "progressBar"));
	progressbar->setValueRange(minValue, maxValue);
	progressbar->setValue(value);
	return progressbar;
}

void uiSkinManager::setCheckBoxSample( uiCheckBox* checkbox )
{
	mCheckBoxSample = checkbox;
}

uiCheckBox* uiSkinManager::checkBox( const string& caption, const layout& layout, const string& id /*= ""*/, 
	                                       bool checked /*= false*/ )
{
	uiCheckBox* checkbox = static_cast<uiCheckBox*>(mCheckBoxSample->clone());
	checkbox->setLayout(layout);
	checkbox->setCCaption(caption);
	checkbox->setId(checkEmptyId(id, "checkbox"));
	checkbox->setChecked(checked);
	return checkbox;
}

uiScrollBar* uiSkinManager::horScrollBar( const layout& layout /*= cLayout::both()*/, const string& id /*= ""*/, 
	                                      float value /*= 0*/, float minValue /*= 0*/, float maxValue /*= 1*/,
										  float barSize /*= 0.1f*/ )
{
	uiScrollBar* scrollbar = static_cast<uiScrollBar*>(mHorScrollbarSample->clone());
	scrollbar->setLayout(layout);
	scrollbar->setId(checkEmptyId(id, "horScrollbar"));
	scrollbar->setValueRange(minValue, maxValue);
	scrollbar->setValue(value);
	scrollbar->setBarSize(barSize);
	return scrollbar;
}

uiScrollBar* uiSkinManager::verThinScrollBar(const layout& layout /*= cLayout::both()*/, const string& id /*= ""*/, 
                                             float value /*= 0*/, float minValue /*= 0*/, float maxValue /*= 1*/, 
										     float barSize /*= 0.1f*/)
{
	uiScrollBar* scrollbar = static_cast<uiScrollBar*>(mVerThinScrollbarSample->clone());
	scrollbar->setLayout(layout);
	scrollbar->setId(checkEmptyId(id, "verThisScrollBar"));
	scrollbar->setValueRange(minValue, maxValue);
	scrollbar->setValue(value);
	scrollbar->setBarSize(barSize);
	return scrollbar;
}

uiScrollBar* uiSkinManager::horThinScrollBar(const layout& layout /*= cLayout::both()*/, const string& id /*= ""*/, 
	                                         float value /*= 0*/, float minValue /*= 0*/, float maxValue /*= 1*/, 
											 float barSize /*= 0.1f*/)
{
	uiScrollBar* scrollbar = static_cast<uiScrollBar*>(mHorThinScrollbarSample->clone());
	scrollbar->setLayout(layout);
	scrollbar->setId(checkEmptyId(id, "horThisScrollBar"));
	scrollbar->setValueRange(minValue, maxValue);
	scrollbar->setValue(value);
	scrollbar->setBarSize(barSize);
	return scrollbar;
}

void uiSkinManager::setHorScrollbarSample( uiScrollBar* scrollbar )
{
	mHorScrollbarSample = scrollbar;
}

uiEditBox* uiSkinManager::editbox( const layout& layout /*= cLayout::both()*/, const string& id /*= ""*/, 
	                               const string& text /*= ""*/, bool multiLine /*= false*/ )
{
	uiEditBox* src = multiLine ? mMultilineEditBox:mSingleLineEditBoxSample;
	uiEditBox* res = static_cast<uiEditBox*>(src->clone());
	res->setLayout(layout);
	res->setId(checkEmptyId(id, "editbox"));
	res->setCText(text);
	return res;
}

void uiSkinManager::setSingleLineEditBoxSample( uiEditBox* editboxSampl )
{
	mSingleLineEditBoxSample = editboxSampl;
}

uiLabel* uiSkinManager::label(const string& text, const layout& layout /*= cLayout::both()*/, const string& id /*= ""*/)
{
	uiLabel* res = static_cast<uiLabel*>(mLabelSample->clone());
	res->setLayout(layout);
	res->setId(checkEmptyId(id, "label"));
	res->setCText(text);
	return res;
}

void uiSkinManager::setLabelSample(uiLabel* labelSample)
{
	mLabelSample = labelSample;
}

void uiSkinManager::setMultilineEditBoxSample(uiEditBox* editboxSample)
{
	mMultilineEditBox = editboxSample;
}

void uiSkinManager::setHorThinScrollbarSample(uiScrollBar* scrollbar)
{
	mHorThinScrollbarSample = scrollbar;
}

void uiSkinManager::setVerThinScrollbarSample(uiScrollBar* scrollbar)
{
	mVerThinScrollbarSample = scrollbar;
}

void uiSkinManager::setScrollAreaSample( uiScrollArea* scrollAreaSample )
{
	mScrollAreaSample = scrollAreaSample;
}

uiScrollArea* uiSkinManager::scrollArea( const layout& layout /*= cLayout::both()*/, const string& id /*= ""*/ )
{
	uiScrollArea* res = static_cast<uiScrollArea*>(mScrollAreaSample->clone());
	res->setLayout(layout);
	res->setId(checkEmptyId(id, "scrollArea"));
	return res;
}

uiHorLayout* uiSkinManager::horLayout(const layout& layout /*= cLayout::both()*/, const string& id/*= ""*/, 
	                                  float widgetsDistance /*= 10.0f*/)
{
	return mnew uiHorLayout(layout, widgetsDistance, checkEmptyId(id, "horlayout"));
}

uiVerLayout* uiSkinManager::verLayout(const layout& layout /*= cLayout::both()*/, const string& id/*= ""*/, float widgetsDistance /*= 10.0f*/)
{
	return mnew uiVerLayout(layout, widgetsDistance, checkEmptyId(id, "verlayout"));
}

string uiSkinManager::checkEmptyId(const string& id, const string& prefix)
{
	if (id.empty()) 
		return prefix + toString(mWidgetIdx++);

	return id;
}

void uiSkinManager::setRectPadSample(uiRect* padSample)
{
	mRectPadSample = padSample;
}

uiRect* uiSkinManager::rectPad(const layout& layout /*= cLayout::both()*/, const string& id /*= ""*/)
{
	uiRect* res = mnew uiRect(*mRectPadSample);
	res->setLayout(layout);
	res->setId(checkEmptyId(id, "pad"));
	return res;
}

CLOSE_O2_NAMESPACE