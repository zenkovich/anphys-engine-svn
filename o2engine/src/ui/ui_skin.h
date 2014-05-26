#ifndef UI_SKIN_H
#define UI_SKIN_H

#include "public.h"
#include "render_system/texture.h"

OPEN_O2_NAMESPACE

class uiWidget;
class uiSprite;
class uiRect;

/** UI Skin help class. This load a some user interface skin and gives functions for fast and easy widgets creation. */
class uiSkin
{
public:
	/** Creates widget. */
	static shared<uiWidget> createWidget(const vec2f& size, const vec2f& position = vec2f(), const string& id = "");

	/** Creates sprite widget. */
	static shared<uiSprite> createSprite(const grTexture& texture, const vec2f& size = vec2f(), 
		                                 const vec2f& position = vec2f(), const string& id = "");

	static shared<uiRect> createRectangle(const grTexture& texture, const fRect& texRect, 
		                                  int left, int top, int right, int bottom, 
							  			  const vec2f& size = vec2f(), const vec2f& position = vec2f(), const string& id = "");

protected:

	/** Adding a simple visible state to widget. */
	static void addVisibleState(const shared<uiWidget>& widget);
};

CLOSE_O2_NAMESPACE

#endif //UI_SKIN_H
