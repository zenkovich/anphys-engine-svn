#include "ui_skin.h"

#include "ui_widget.h"
#include "ui_sprite.h"
#include "ui_transition_state.h"
#include "ui_rect.h"

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
	addVisibleState(sprite);
	return sprite;
}

void uiSkin::addVisibleState(const shared<uiWidget>& widget)
{
	shared<uiTransitionState> visibleState = mnew uiTransitionState("visible");
	visibleState->onBeginActiveStateEvent.add( callback<bool, uiWidget>( widget, &uiWidget::setVisibleParam, true ) );
	visibleState->onDeactiveStateEvent.add( callback<bool, uiWidget>( widget, &uiWidget::setVisibleParam, false ) );
	visibleState->addProperty<float>("transparency", 0.0f, 1.0f);

	widget->addState(visibleState);
}

shared<uiRect> uiSkin::createRectangle(const grTexture& texture, const fRect& texRect, 
	                                   int left, int top, int right, int bottom, 
									   const vec2f& size /*= vec2f()*/, const vec2f& position /*= vec2f()*/, 
									   const string& id /*= ""*/)
{
	shared<uiRect> rect = mnew uiRect(uiStaightPixelLayout(position, size), id);
	rect->mStretchRect = cStretchRect(texture, left, top, right, bottom, texRect);
	rect->position = position;
	rect->size = size;
	addVisibleState(rect);
	return rect;
}

CLOSE_O2_NAMESPACE