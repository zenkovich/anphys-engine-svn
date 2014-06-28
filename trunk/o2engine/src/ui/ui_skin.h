#ifndef UI_SKIN_H
#define UI_SKIN_H

#include "public.h"
#include "render_system/texture.h"
#include "util/math/layout.h"
#include "util/singleton.h"
#include "ui_button.h"
#include "ui_rect.h"

OPEN_O2_NAMESPACE

class uiWidget;
class uiSprite;
class uiProgressBar;
class uiCheckBox;
class uiScrollBar;

/** UI Skin help class. This load a some user interface skin and gives functions for fast and easy widgets creation. */
class uiSkinManager: public cSingleton<uiSkinManager>
{
protected:
	uiButton*      mButtonSample;
	uiRect*        mBackgroundSample;
	uiProgressBar* mProgressBarSample;
	uiCheckBox*    mCheckBoxSample;
	uiScrollBar*   mHorScrollbarSample;

public:
	/** Creates widget. */
	uiWidget* widget(const vec2f& size, const vec2f& position = vec2f(), const string& id = "");

	/** Creates background rectangle widget. */
	uiRect* background(const cLayout& layout = cLayout::both(), const string& id = "");

	/** Creates button. */
	uiButton* button(const string& caption, const cLayout& layout, const string& id = "");

	/** Creates sprite widget from texture. */
	uiSprite* sprite(const grTexture& texture, const cLayout& layout = cLayout::both(), const string& id = "");

	/** Creates progress bar. */
	uiProgressBar* progressBar(const cLayout& layout = cLayout::both(), const string& id = "",
		                       float value = 0, float minValue = 0, float maxValue = 1);

	/** Creates checkbox. */
	uiCheckBox* checkBox(const string& caption, const cLayout& layout, const string& id = "", bool checked = false);

	/** Creates horizontal scroll bar. */
	uiScrollBar* horScrollBar(const cLayout& layout = cLayout::both(), const string& id = "",
		                      float value = 0, float minValue = 0, float maxValue = 1, float barSize = 0.1f);

	/** Sets button sample. */
	void setButtonSample(uiButton* buttonSample);

	/** Sets background sample. */
	void setBackgroundSample(uiRect* backgroundSample);

	/** Sets progressbar sample. */
	void setProgressbarSample(uiProgressBar* progressbarSample);

	/** Sets checkbox sample. */
	void setCheckBoxSample(uiCheckBox* checkbox);

	/** Sets horizontal scroll bar sample. */
	void setHorScrollbarSample(uiScrollBar* scrollbar);

protected:
	void addVisibleState(uiWidget* widget);
};

#define uiSkin() uiSkinManager::instancePtr()

CLOSE_O2_NAMESPACE

#endif //UI_SKIN_H
