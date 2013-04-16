#include "ui_simple_stuff.h"

#include "sprite_widget.h"
#include "ui_manager.h"
#include "render/render_objects/2d/sprite.h"
#include "ui_label.h"
#include "ui_button.h"
#include "ui_state.h"
#include "ui_property.h"

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
	panelSprite->addChild(label);

	uiState* visibleState = panelSprite->getState("visible");
	uiState* selectedState = addWidgetState(panelSprite, "selected");
	uiState* pressedState = addWidgetState(panelSprite, "pressed");

	addStateProperty(visibleState, &panelSprite->mSpriteColor, mColor2, 0.15f);
	addStateProperty(visibleState, &panelSprite->mOffset, vec2(0, 0), 0.15f);

	addStateProperty(selectedState, &panelSprite->mSpriteColor, (mColor1 + mColor2)*0.5f, 0.15f);
	addStateProperty(selectedState, &panelSprite->mOffset, vec2(0, 0), 0.05f);

	addStateProperty(pressedState, &panelSprite->mSpriteColor, (mColor3 + mColor2)*0.5f, 0.05f);
	addStateProperty(pressedState, &panelSprite->mOffset, press, 0.05f);

	button->addChild(bkSprite);
	button->addChild(panelSprite);

	return button;
}

void uiSimpleStuff::addWidgetState( uiWidget* widget, const std::string& id, float transparency, 
	                                const vec2& offs, float duration )
{
	widget->removeState(widget->getState(id));
	uiState* newState = new uiState(widget, id);
	newState->addProperty(new uiParameterProperty<float>(&widget->mTransparency, transparency, uiProperty::IT_LINEAR, duration));
	newState->addProperty(new uiParameterProperty<vec2>(&widget->mOffset, offs, uiProperty::IT_LINEAR, duration));
	widget->addState(newState);
}

uiState* uiSimpleStuff::addWidgetState( uiWidget* widget, const std::string& id )
{
	widget->removeState(widget->getState(id));
	uiState* newState = new uiState(widget, id);
	widget->addState(newState);

	return newState;
}

color4 uiSimpleStuff::mColor4 = color4(0.5f, 0.5f, 0.5f, 1.0f);
color4 uiSimpleStuff::mColor3 = color4(0.6f, 0.6f, 0.6f, 1.0f);
color4 uiSimpleStuff::mColor2 = color4(0.7f, 0.7f, 0.7f, 1.0f);
color4 uiSimpleStuff::mColor1 = color4(0.8f, 0.8f, 0.8f, 1.0f);
