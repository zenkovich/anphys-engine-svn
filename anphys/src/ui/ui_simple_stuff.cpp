#include "ui_simple_stuff.h"

#include "sprite_widget.h"
#include "ui_manager.h"
#include "render/render_objects/2d/sprite.h"
#include "ui_label.h"

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
