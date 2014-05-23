#ifndef UI_SKIN_H
#define UI_SKIN_H

#include "public.h"
#include "render_system/texture.h"

OPEN_O2_NAMESPACE

class uiWidget;
class uiSprite;

class uiSkin
{
	static shared<uiWidget> createWidget(const vec2f& size, const vec2f& position = vec2f(), const string& id = "");
	static shared<uiSprite> createSprite(const grTexture& texture, const vec2f& size = vec2f(), 
		                                 const vec2f& position = vec2f(), const string& id = "");
};

CLOSE_O2_NAMESPACE

#endif //UI_SKIN_H
