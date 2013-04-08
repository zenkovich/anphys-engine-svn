#include "sprite_widget.h"

#include "render/render_objects/2d/sprite.h"
#include "util/serialization/data_objects_manager.h"

REGIST_TYPE(uiSpriteWidget)

uiSpriteWidget::uiSpriteWidget(grSprite* sprite = NULL, bool ownSprite = true, const std::string& id = "noName", 
                               uiWidget* parent = NULL):
	uiWidget(id, parent), mSprite(sprite), mOwnSprite(ownSprite)
{
}

uiSpriteWidget::uiSpriteWidget(const std::string& spriteSrcFile, const std::string& spriteSrcObjectpath,
	                           const std::string& id = "noName", uiWidget* parent = NULL):
	uiWidget(id, parent), mSprite(NULL), mOwnSprite(false)
{
}

uiSpriteWidget::uiSpriteWidget( cDataObject* dataObject, uiWidget* parent /*= NULL*/ )
{

}

uiSpriteWidget::uiSpriteWidget(const uiSpriteWidget& spriteWidget)
{

}

uiSpriteWidget::~uiSpriteWidget()
{

}

void uiSpriteWidget::update( float dt )
{

}

void uiSpriteWidget::draw()
{

}

uiWidget* uiSpriteWidget::clone() const
{
	return new uiSpriteWidget(*this);
}
