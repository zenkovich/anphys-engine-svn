#ifndef UI_SKIN_H
#define UI_SKIN_H

#include "public.h"
#include "render_system/texture.h"
#include "util/singleton.h"
#include "ui_widget_layout.h"

OPEN_O2_NAMESPACE

class uiWidget;
class uiSprite;
class uiRect;
class uiButton;

/** UI Skin help class. This load a some user interface skin and gives functions for fast and easy widgets creation. */
class uiSkinManager: public cSingleton<uiSkinManager>
{
protected:
	shared<uiButton> mButtonSample;
	shared<uiRect> mBackgroundSamble;

public:
	/** Creates widget. */
	shared<uiWidget> createWidget(const vec2f& size, const vec2f& position = vec2f(), const string& id = "");

	/** Creates background rectangle widget. */
	shared<uiRect> createBackground(const uiWidgetLayout& layout = uiBothLayout(), const string& id = "");

	/** Creates sprite widget. */
	shared<uiSprite> createSprite(const grTexture& texture, const vec2f& size = vec2f(), 
		                                 const vec2f& position = vec2f(), const string& id = "");

	shared<uiRect> createRectangle(const grTexture& texture, const fRect& texRect, 
		                                  int left, int top, int right, int bottom, 
							  			  const vec2f& size = vec2f(), const vec2f& position = vec2f(), const string& id = "");

	shared<uiButton> createButton(const vec2f& size, const vec2f& position = vec2f(), const string& id = "");

	void setButtonSample(const shared<uiButton>& buttonSample);
	void setBackgroundSamble(const shared<uiRect>& backgroundSample);

protected:

	/** Adding a simple visible state to widget. */
	static void addVisibleState(const shared<uiWidget>& widget);
};

#define uiSkin() uiSkinManager::instancePtr()

CLOSE_O2_NAMESPACE

#endif //UI_SKIN_H
