#include "ui_simple_stuff.h"

#include "ui_sprite.h"
#include "ui_manager.h"
#include "render/render_objects/2d/sprite.h"
#include "ui_label.h"
#include "ui_button.h"
#include "ui_state.h"
#include "ui_property.h"
#include "ui_font.h"

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
		                                uiProperty::IT_LINEAR, 0.1f, uiParameterProperty<color4>::OP_ADDITION, 1.5f), panelSprite);

	button->mPressedState->addProperty(
		new uiParameterProperty<color4>(&panelSprite->mSpriteColor, color4(0, 0, 0, 0), color4(0.25f, 0.25f, 0.25f, 0.25f), 
		                                uiProperty::IT_FORCIBLE, 0.01f, uiParameterProperty<color4>::OP_SUBSTRACT, 1.5f), panelSprite);

	button->mPressedState->addProperty(
		new uiParameterProperty<vec2>(&panelSprite->mOffset, vec2(0, 0), press, 
		                                uiProperty::IT_FORCIBLE, 0.01f, uiParameterProperty<vec2>::OP_ADDITION, 1.5f), panelSprite);
	

	button->addChild(bkSprite);
	button->addChild(panelSprite);

	return button;
}


color4 uiSimpleStuff::mColor4 = color4(0.5f, 0.5f, 0.5f, 1.0f);
color4 uiSimpleStuff::mColor3 = color4(0.6f, 0.6f, 0.6f, 1.0f);
color4 uiSimpleStuff::mColor2 = color4(0.7f, 0.7f, 0.7f, 1.0f);
color4 uiSimpleStuff::mColor1 = color4(0.8f, 0.8f, 0.8f, 1.0f);
