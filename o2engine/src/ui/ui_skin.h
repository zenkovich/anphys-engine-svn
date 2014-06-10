#ifndef UI_SKIN_H
#define UI_SKIN_H

#include "public.h"
#include "render_system/texture.h"
#include "util/math/layout.h"
#include "util/singleton.h"

OPEN_O2_NAMESPACE

class uiWidget;
class uiSprite;
class uiRect;
class uiButton;
class uiProgressBar;

/** UI Skin help class. This load a some user interface skin and gives functions for fast and easy widgets creation. */
class uiSkinManager: public cSingleton<uiSkinManager>
{
protected:
	shared<uiButton>      mButtonSample;
	shared<uiRect>        mBackgroundSample;
	shared<uiProgressBar> mProgressBarSample;

public:
	/** Creates widget. */
	shared<uiWidget> createWidget(const vec2f& size, const vec2f& position = vec2f(), const string& id = "");

	/** Creates background rectangle widget. */
	shared<uiRect> createBackground(const cLayout& layout = cLayout::both(), const string& id = "");

	/** Creates button. */
	shared<uiButton> createButton(const string& caption, const cLayout& layout, const string& id = "");

	/** Creates sprite widget from texture. */
	shared<uiSprite> createSprite(const grTexture& texture, const cLayout& layout = cLayout::both(), 
		                          const string& id = "");

	/** Creates progress bar. */
	shared<uiProgressBar> createProgressBar(const cLayout& layout = cLayout::both(), const string& id = "",
		                                    float value = 0, float minValue = 0, float maxValue = 1);

	/** Sets button sample. */
	void setButtonSample(const shared<uiButton>& buttonSample);

	/** Sets background sample. */
	void setBackgroundSample(const shared<uiRect>& backgroundSample);

	/** Sets progressbar sample. */
	void setProgressbarSample(const shared<uiProgressBar>& progressbarSample);

protected:
	void addVisibleState(const shared<uiWidget>& widget);
};

#define uiSkin() uiSkinManager::instancePtr()

CLOSE_O2_NAMESPACE

#endif //UI_SKIN_H
