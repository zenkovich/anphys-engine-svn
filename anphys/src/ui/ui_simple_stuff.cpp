#include "ui_simple_stuff.h"

#include "ui_sprite.h"
#include "ui_manager.h"
#include "render/render_objects/2d/sprite.h"
#include "ui_label.h"
#include "ui_button.h"
#include "ui_state.h"
#include "ui_property.h"
#include "ui_font.h"
#include "ui_scrollbar.h"
#include "ui_scroll_area.h"
#include "ui_text_edit.h"
#include "ui_border.h"
#include "ui_solid_rect.h"
#include "ui_window.h"
#include "ui_lines_geometry.h"
#include "ui_checkbox.h"

uiSpriteWidget* uiSimpleStuff::createSpriteWidget( uiWidgetsManager* widgetsManager, 
	                   const color4& color, const vec2& pos, const vec2& size, const std::string& id )
{
	grSprite* sprite = new grSprite(widgetsManager->mRender);
	sprite->setColor(color);

	uiSpriteWidget* spriteWidget = new uiSpriteWidget(widgetsManager, sprite, true, id);
	spriteWidget->setPosition(pos);
	spriteWidget->setSize(size);
	
	return spriteWidget;
}

uiLabel* uiSimpleStuff::createLabel( uiWidgetsManager* widgetsManager, const vec2& pos, const vec2& size, 
	const std::string& id, const std::string& text )
{
	uiLabel* label = new uiLabel(widgetsManager, "fonts/rfont", "font", id);
	label->setText(text);
	label->setPosition(pos);
	label->setSize(size);
	label->setColor(mColor6);
	label->setDistCoef(vec2(-2, 0));
	label->setHorAlign(uiLabel::AL_LEFT);

	return label;
}

uiButton* uiSimpleStuff::createButton( uiWidgetsManager* widgetManager, const vec2& pos, const vec2& size, 
	                                   const std::string& id, const std::string& caption, 
									   cCallbackInterface* callback )
{
	uiLabel* label = createLabel(widgetManager, vec2(0, 0), size , "label", caption);
	label->mFont->setClipping(true);

	return createButton(widgetManager, pos, size, id, label, callback);
}

uiButton* uiSimpleStuff::createButton( uiWidgetsManager* widgetManager, const vec2& pos, const vec2& size, 
	                                   const std::string& id, uiWidget* content, cCallbackInterface* callback )
{	
	uiButton* button = new uiButton(widgetManager, id, callback);
	button->setPosition(pos);

	uiSolidRect* panelSprite = new uiSolidRect(widgetManager, "panel", mColor5, mColor3);
	panelSprite->setSize(size);

	button->mSelectedState->addProperty(
		new uiParameterProperty<color4>(&panelSprite->mResInColor, color4(0, 0, 0, 0), color4(0.05f, 0.05f, 0.05f, 0.0f), 
		                                uiProperty::IT_SMOOTH, 0.15f, uiParameterProperty<color4>::OP_ADDITION, 1.5f), panelSprite);

	button->mPressedState->addProperty(
		new uiParameterProperty<color4>(&panelSprite->mResInColor, color4(0, 0, 0, 0), color4(0.25f, 0.25f, 0.25f, 0.0f), 
		                                uiProperty::IT_FORCIBLE, 0.01f, uiParameterProperty<color4>::OP_SUBSTRACT, 1.5f), panelSprite);

	button->addChild(panelSprite);
	button->addChild(content);

	button->setSize(size);

	return button;
}

void uiSimpleStuff::createSizeEffect( uiWidget* widget, float duration /*= 0.5f*/ )
{
	widget->mVisibleState->removeAllProperties();

	widget->setClipping(true);

	widget->mVisibleState->addProperty(
		new uiParameterProperty<float>(&widget->mTransparency, 0.0f, 1.0f, uiProperty::IT_SMOOTH, 
		duration*0.3f, duration, duration*0.8f,
		uiProperty::OP_MULTIPLICATION, 1.5f));

	widget->mVisibleState->addProperty(
		new uiParameterProperty<float>(&widget->mResSize.x, 0.0f, 1.0f, uiProperty::IT_SMOOTH, 
		duration*0.3f, duration, duration*0.6f,
		uiProperty::OP_MULTIPLICATION, 1.5f));

	widget->mVisibleState->addProperty(
		new uiParameterProperty<float>(&widget->mResSize.y, 0.1f, 1.0f, uiProperty::IT_SMOOTH, 
		duration*0.6f, duration, duration,
		uiProperty::OP_MULTIPLICATION, 1.5f));
}

uiScrollbar* uiSimpleStuff::createScrollbar( uiWidgetsManager* widgetManager, const vec2& pos, const vec2& size, 
	const std::string& id, int type, float minv /*= 0.0f*/, float maxv /*= 1.0f*/ )
{
	uiSolidRect* bkWidget = new uiSolidRect(widgetManager, "bk", mColor5, mColor1);
	uiSolidRect* scrollerWidget = new uiSolidRect(widgetManager, "csroller", mColor5, mColor2);

	/*grSprite* bkSprite = new grSprite(widgetManager->mRender, NULL);
	grSprite* scrollerSprite = new grSprite(widgetManager->mRender, NULL);

	bkSprite->setColor(mColor1);
	scrollerSprite->setColor(mColor2);*/

	uiScrollbar* scrollbar = new uiScrollbar(widgetManager, id, (uiScrollbar::ScrollbarType)type, size, bkWidget,
		scrollerWidget, minv, maxv,minv, -1.0f);

	scrollbar->setPosition(pos);

	scrollbar->mSelectedState->addProperty(
		new uiParameterProperty<color4>(&scrollerWidget->mResInColor, color4(0, 0, 0, 0), color4(0.05f, 0.05f, 0.05f, 0.0f), 
		uiProperty::IT_SMOOTH, 0.15f, uiParameterProperty<color4>::OP_ADDITION, 1.5f), scrollerWidget);

	scrollbar->mPressedState->addProperty(
		new uiParameterProperty<color4>(&scrollerWidget->mResInColor, color4(0, 0, 0, 0), color4(0.25f, 0.25f, 0.25f, 0.0f), 
		uiProperty::IT_FORCIBLE, 0.01f, uiParameterProperty<color4>::OP_SUBSTRACT, 1.5f), scrollerWidget);

	return scrollbar;
}

uiScrollArea* uiSimpleStuff::createScrollarea( uiWidgetsManager* widgetsManager, const vec2& pos, const vec2& size, const std::string& id )
{
	uiScrollbar* horScrollbar = createScrollbar(widgetsManager, vec2(0.0f, 0.0f), vec2(15.0f, 15.0f), "horScrollbar",
		(int)uiScrollbar::ST_HORISONTAL);

	uiScrollbar* verScrollbar = createScrollbar(widgetsManager, vec2(0.0f, 0.0f), vec2(15.0f, 15.0f), "verScrollbar",
		(int)uiScrollbar::ST_VERTICAL);

	uiSolidRect* contentWidget = new uiSolidRect(widgetsManager, "content", mColor5, mColor1);

	uiScrollArea* scrollarea = new uiScrollArea(widgetsManager, id, size, contentWidget);
	scrollarea->setPosition(pos);
	scrollarea->setHorScrollbar(horScrollbar);
	scrollarea->setVerScrollbar(verScrollbar);

	return scrollarea;
}

uiTextEdit* uiSimpleStuff::createTextEdit( uiWidgetsManager* widgetsManager, const vec2& pos, const vec2& size, const std::string& id )
{
	uiFont* font = new uiFont(widgetsManager->mRender);
	font->load("fonts/rfont", "font");
	//font->setWordWrap(true);
	font->setColor(mColor6);
	font->setDistCoef(vec2(-2, 0));

	//uiSpriteWidget* backWidget = createSpriteWidget(widgetsManager, mColor1, vec2(0, 0), size, "bk");
	uiSolidRect* backWidget = new uiSolidRect(widgetsManager, "bk", mColor5, mColor1);

	uiTextEdit* textEdit = new uiTextEdit(widgetsManager, id, font, backWidget);
	textEdit->setPosition(pos);
	//textEdit->setText("000 111 222 333 444 555 666 777\n\n\n000 123");
	textEdit->setSize(size);
	textEdit->setTextOffset(vec2(2, 1));

	textEdit->mSelectedState->addProperty(
		new uiParameterProperty<color4>(&backWidget->mResInColor, color4(0, 0, 0, 0), color4(0.05f, 0.05f, 0.05f, 0.0f), 
		uiProperty::IT_SMOOTH, 0.15f, uiParameterProperty<color4>::OP_ADDITION, 1.5f), backWidget);

	textEdit->mFocusedState->addProperty(
		new uiParameterProperty<color4>(&backWidget->mResInColor, color4(0, 0, 0, 0), color4(0.05f, 0.05f, 0.05f, 0.0f), 
		uiProperty::IT_FORCIBLE, 0.01f, uiParameterProperty<color4>::OP_ADDITION, 1.5f), backWidget);

	return textEdit;
}

uiBorder* uiSimpleStuff::createBorder( uiWidgetsManager* widgetsManager, const std::string& id, const vec2& pos, 
	                                   const vec2& size, int type, const std::string& caption )
{
	uiFont* font = new uiFont(widgetsManager->mRender);
	font->load("fonts/rfont", "font");
	font->setColor(mColor6);
	font->setDistCoef(vec2(-2, 0));
	font->setText(caption);

	uiBorder* border = new uiBorder(widgetsManager, id, font, 
		fRect(5.0f, (caption.length() == 0) ? 5.0f:20.0f, 5.0f, 5.0f), (uiBorder::LineType)type, mColor5);

	border->setPosition(pos);
	border->setSize(size - vec2(5, 5));

	return border;
}

uiWindow* uiSimpleStuff::createWindow( uiWidgetsManager* widgetsManager, const std::string& id, const vec2& pos, 
	                                   const vec2& size, const std::string& caption )
{
	float headSize = 22;
	float borders = 2;
	float closeBtnSize = 15;

	uiSolidRect* windowHead = new uiSolidRect(widgetsManager, "windowHead", mColor5, mColor2);
	windowHead->setPosition(vec2(borders, borders));
	windowHead->setSize(vec2(size.x - borders*2.0f, headSize));

	uiLabel* windowCaption = createLabel(widgetsManager, vec2(2, 1), 
		vec2(size.x - borders*2.0f - 4.0f - closeBtnSize, headSize - 2), "windowCaption", caption);
	windowCaption->setHorAlign(uiLabel::AL_LEFT);
	windowCaption->setDistCoef(vec2(-2, 0));

	uiLinesGeometry* buttonCross = new uiLinesGeometry(widgetsManager, "cross");
	buttonCross->addLine(vec2(borders, borders), vec2(closeBtnSize - borders, closeBtnSize - borders), mColor5);
	buttonCross->addLine(vec2(closeBtnSize - borders, borders), vec2(borders, closeBtnSize - borders), mColor5);

	uiButton* closeBtn = createButton(widgetsManager, vec2(size.x - borders*3.0f - closeBtnSize, borders*2),
		vec2(closeBtnSize, closeBtnSize), "closeBtn", buttonCross, NULL);

	windowHead->addChild(windowCaption);
	windowHead->addChild(closeBtn);
	//windowHead->setClipping(true);

	uiScrollArea* contentScrollarea = createScrollarea(widgetsManager, vec2(borders, borders*2.0f + headSize), 
		vec2(size.x - borders*2.0f, size.y - borders*3.0f - headSize), "content"); 

	uiSolidRect* backWidget = new uiSolidRect(widgetsManager, "back", mColor5, mColor2);
	backWidget->setSize(size);

	uiWindow* window = new uiWindow(widgetsManager, id, size, pos, windowHead, contentScrollarea, backWidget, closeBtn, 
		                            windowCaption);	

	window->mHeadSelectingState->addProperty(
		new uiParameterProperty<color4>(&windowHead->mResInColor, color4(0, 0, 0, 0), color4(0.05f, 0.05f, 0.05f, 0.0f), 
		                                uiProperty::IT_SMOOTH, 0.15f, uiParameterProperty<color4>::OP_ADDITION, 1.5f), windowHead);

	window->mHeadPressedState->addProperty(
		new uiParameterProperty<color4>(&windowHead->mResInColor, color4(0, 0, 0, 0), color4(0.25f, 0.25f, 0.25f, 0.0f), 
		                                uiProperty::IT_FORCIBLE, 0.01f, uiParameterProperty<color4>::OP_SUBSTRACT, 1.5f), windowHead);

	return window;
}

uiCheckBox* uiSimpleStuff::createCheckbox( uiWidgetsManager* widgetsManager, const std::string& id, const vec2& pos, 
	                                       const std::string& caption, bool checked /*= false*/ )
{
	float checkSize = 12;
	float chechThumbSize = 6;
	float textHeight = 20;
	float textOffs = 2;

	float checkOffs = (textHeight - checkSize)*0.5f;
	float thumbOffs = (checkSize - chechThumbSize)*0.5f;

	uiSolidRect* checkBack = new uiSolidRect(widgetsManager, "checkBack", mColor5, mColor2);
	checkBack->setPosition(vec2(0, checkOffs));
	checkBack->setSize(vec2(checkSize, checkSize));

	uiSolidRect* checkThumb = new uiSolidRect(widgetsManager, "checkThumb", mColor5, mColor4);
	checkThumb->setPosition(vec2(thumbOffs, thumbOffs + checkOffs));
	checkThumb->setSize(vec2(chechThumbSize, chechThumbSize));

	uiLabel* captionLabel = createLabel(widgetsManager, vec2(textOffs + checkSize, 0), vec2(10, textHeight), "caption", caption);
	captionLabel->setHorAlign(uiLabel::AL_LEFT);
	captionLabel->setDistCoef(vec2(-2, 0));

	uiCheckBox* checkBox = new uiCheckBox(widgetsManager, id, checkBack, checkThumb, captionLabel);
	checkBox->setCheck(checked);
	checkBox->setPosition(pos);
	
	checkBox->mSelectedState->addProperty(
		new uiParameterProperty<color4>(&checkBack->mResInColor, color4(0, 0, 0, 0), color4(0.05f, 0.05f, 0.05f, 0.0f), 
		                                uiProperty::IT_SMOOTH, 0.15f, uiParameterProperty<color4>::OP_ADDITION, 1.5f), checkBack);
	
	checkBox->mSelectedState->addProperty(
		new uiParameterProperty<color4>(&checkThumb->mResInColor, color4(0, 0, 0, 0), color4(0.05f, 0.05f, 0.05f, 0.0f), 
		                                uiProperty::IT_SMOOTH, 0.15f, uiParameterProperty<color4>::OP_ADDITION, 1.5f), checkThumb);

	checkBox->mPressedState->addProperty(
		new uiParameterProperty<color4>(&checkBack->mResInColor, color4(0, 0, 0, 0), color4(0.25f, 0.25f, 0.25f, 0.0f), 
		                                uiProperty::IT_FORCIBLE, 0.01f, uiParameterProperty<color4>::OP_SUBSTRACT, 1.5f), checkBack);

	return checkBox;
}


color4 uiSimpleStuff::mColor6 = color4(0.3f, 0.3f, 0.3f, 1.0f);
color4 uiSimpleStuff::mColor5 = color4(0.4f, 0.4f, 0.4f, 1.0f);
color4 uiSimpleStuff::mColor4 = color4(0.5f, 0.5f, 0.5f, 1.0f);
color4 uiSimpleStuff::mColor3 = color4(0.6f, 0.6f, 0.6f, 1.0f);
color4 uiSimpleStuff::mColor2 = color4(0.7f, 0.7f, 0.7f, 1.0f);
color4 uiSimpleStuff::mColor1 = color4(0.8f, 0.8f, 0.8f, 1.0f);
