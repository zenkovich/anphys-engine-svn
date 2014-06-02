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

	/** Creates button. */
	shared<uiButton> createButton(const string& caption, const uiWidgetLayout& layout, const string& id = "");

	/** Creates sprite widget from texture. */
	shared<uiSprite> createSprite(const grTexture& texture, const uiWidgetLayout& layout = uiBothLayout(), 
		                          const string& id = "");

	/** Sets button sample. */
	void setButtonSample(const shared<uiButton>& buttonSample);

	/** Sets background sample. */
	void setBackgroundSamble(const shared<uiRect>& backgroundSample);

protected:
	void addVisibleState(const shared<uiWidget>& widget);
};

#define uiSkin() uiSkinManager::instancePtr()

CLOSE_O2_NAMESPACE

#endif //UI_SKIN_H
