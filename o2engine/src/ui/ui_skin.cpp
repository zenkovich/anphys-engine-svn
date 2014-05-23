#include "ui_skin.h"

#include "ui_widget.h"
#include "ui_sprite.h"

OPEN_O2_NAMESPACE

shared<uiWidget> uiSkin::createWidget(const vec2f& size, const vec2f& position /*= vec2f()*/, const string& id /*= ""*/)
{
	shared<uiWidget> widget = mnew uiWidget(uiStaightPixelLayout(position, size), id);
	return widget;
}

shared<uiSprite> uiSkin::createSprite(const grTexture& texture, const vec2f& size /*= vec2f()*/, 
	                                  const vec2f& position /*= vec2f()*/, const string& id /*= ""*/)
{
	shared<uiSprite> sprite = mnew uiSprite(uiStaightPixelLayout(position, size), id);
	sprite->mSprite.setTexture(texture);
	sprite->mSprite.setTextureSrcRect(fRect(vec2f(), texture.getSize()));
	return sprite;
}

CLOSE_O2_NAMESPACE