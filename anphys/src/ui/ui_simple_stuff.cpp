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
	uiLabel* label = new uiLabel(widgetsManager, "fonts/system_font", "font", id);
	label->setText(text);
	label->setPosition(pos);
	label->setSize(size);

	return label;
}

uiButton* uiSimpleStuff::createButton( uiWidgetsManager* widgetManager, const vec2& pos, const vec2& size, 
	                                   const std::string& id, const std::string& caption, 
									   cCallbackInterface* callback )
{
	uiButton* button = new uiButton(widgetManager, id, callback);
	button->setPosition(pos);

	vec2 diff(3, 3);
	vec2 press(-2, -2);
	uiSpriteWidget* bkSprite = createSpriteWidget(widgetManager, mColor3, vec2(0, 0), size - diff, "bk");

	uiSpriteWidget* panelSprite = createSpriteWidget(widgetManager, mColor2, diff, size - diff, "panel");
	uiLabel* label = createLabel(widgetManager, vec2(0, 0), size - diff, "label", caption);
	label->mFont->setClipping(true);
	panelSprite->addChild(label);

	button->mSelectedState->addProperty(
		new uiParameterProperty<color4>(&panelSprite->mSpriteColor, color4(0, 0, 0, 0), color4(0.05f, 0.05f, 0.05f, 0.05f), 
		                                uiProperty::IT_SMOOTH, 0.15f, uiParameterProperty<color4>::OP_ADDITION, 1.5f), panelSprite);

	button->mPressedState->addProperty(
		new uiParameterProperty<color4>(&panelSprite->mSpriteColor, color4(0, 0, 0, 0), color4(0.25f, 0.25f, 0.25f, 0.25f), 
		                                uiProperty::IT_FORCIBLE, 0.01f, uiParameterProperty<color4>::OP_SUBSTRACT, 1.5f), panelSprite);

	button->mPressedState->addProperty(
		new uiParameterProperty<vec2>(&panelSprite->mOffset, vec2(0, 0), press, 
		                                uiProperty::IT_FORCIBLE, 0.01f, uiParameterProperty<vec2>::OP_ADDITION, 1.5f), panelSprite);
	

	button->addChild(bkSprite);
	button->addChild(panelSprite);

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
	grSprite* bkSprite = new grSprite(widgetManager->mRender, NULL);
	grSprite* scrollerSprite = new grSprite(widgetManager->mRender, NULL);

	bkSprite->setColor(mColor1);
	scrollerSprite->setColor(mColor2);

	uiScrollbar* scrollbar = new uiScrollbar(widgetManager, id, (uiScrollbar::ScrollbarType)type, size, bkSprite,
		scrollerSprite, minv, maxv,minv, -1.0f);

	scrollbar->setPosition(pos);

	scrollbar->mSelectedState->addProperty(
		new uiParameterProperty<color4>(&scrollbar->mResScrollerColor, color4(0, 0, 0, 0), color4(0.05f, 0.05f, 0.05f, 0.05f), 
		uiProperty::IT_SMOOTH, 0.15f, uiParameterProperty<color4>::OP_ADDITION, 1.5f));

	scrollbar->mPressedState->addProperty(
		new uiParameterProperty<color4>(&scrollbar->mResScrollerColor, color4(0, 0, 0, 0), color4(0.25f, 0.25f, 0.25f, 0.25f), 
		uiProperty::IT_FORCIBLE, 0.01f, uiParameterProperty<color4>::OP_SUBSTRACT, 1.5f));

	return scrollbar;
}

uiScrollArea* uiSimpleStuff::createScrollarea( uiWidgetsManager* widgetsManager, const vec2& pos, const vec2& size, const std::string& id )
{
	uiScrollbar* horScrollbar = createScrollbar(widgetsManager, vec2(0.0f, 0.0f), vec2(15.0f, 15.0f), "horScrollbar",
		(int)uiScrollbar::ST_HORISONTAL);

	uiScrollbar* verScrollbar = createScrollbar(widgetsManager, vec2(0.0f, 0.0f), vec2(15.0f, 15.0f), "verScrollbar",
		(int)uiScrollbar::ST_VERTICAL);

	uiScrollArea* scrollarea = new uiScrollArea(widgetsManager, id, size);
	scrollarea->setPosition(pos);
	scrollarea->setHorScrollbar(horScrollbar);
	scrollarea->setVerScrollbar(verScrollbar);

	return scrollarea;
}

uiTextEdit* uiSimpleStuff::createTextEdit( uiWidgetsManager* widgetsManager, const vec2& pos, const vec2& size, const std::string& id )
{
	uiFont* font = new uiFont(widgetsManager->mRender);
	font->load("fonts/system_font", "font");
	font->setWordWrap(true);

	uiSpriteWidget* backWidget = createSpriteWidget(widgetsManager, mColor1, vec2(0, 0), size, "bk");

	uiTextEdit* textEdit = new uiTextEdit(widgetsManager, id, font, backWidget);
	textEdit->setPosition(pos);
	textEdit->setText("000 111 222 333 444\n\n\n000\n123");
	textEdit->setSize(size);

	textEdit->mSelectedState->addProperty(
		new uiParameterProperty<color4>(&backWidget->mSpriteColor, color4(0, 0, 0, 0), color4(0.05f, 0.05f, 0.05f, 0.05f), 
		uiProperty::IT_SMOOTH, 0.15f, uiParameterProperty<color4>::OP_ADDITION, 1.5f), backWidget);

	textEdit->mFocusedState->addProperty(
		new uiParameterProperty<color4>(&backWidget->mSpriteColor, color4(0, 0, 0, 0), color4(0.05f, 0.05f, 0.05f, 0.05f), 
		uiProperty::IT_FORCIBLE, 0.01f, uiParameterProperty<color4>::OP_ADDITION, 1.5f), backWidget);

	return textEdit;
}


color4 uiSimpleStuff::mColor4 = color4(0.5f, 0.5f, 0.5f, 1.0f);
color4 uiSimpleStuff::mColor3 = color4(0.6f, 0.6f, 0.6f, 1.0f);
color4 uiSimpleStuff::mColor2 = color4(0.7f, 0.7f, 0.7f, 1.0f);
color4 uiSimpleStuff::mColor1 = color4(0.8f, 0.8f, 0.8f, 1.0f);
