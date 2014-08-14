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
class uiEditBox;
class uiLabel;
class uiScrollArea;
class uiHorLayout;
class uiVerLayout;
class uiRect;

/** UI Skin help class. This load a some user interface skin and gives functions for fast and easy widgets creation. */
class uiSkinManager: public cSingleton<uiSkinManager>
{
	friend class uiStdSkinInitializer;

protected:
	uiButton*      mButtonSample;
	uiRect*        mBackgroundSample;
	uiProgressBar* mProgressBarSample;
	uiCheckBox*    mCheckBoxSample;
	uiScrollBar*   mHorScrollbarSample;
	uiScrollBar*   mHorThinScrollbarSample;
	uiScrollBar*   mVerThinScrollbarSample;
	uiEditBox*     mSingleLineEditBoxSample;
	uiEditBox*     mMultilineEditBox;
	uiLabel*       mLabelSample;
	uiScrollArea*  mScrollAreaSample;
	uiRect*        mRectPadSample;
	int            mWidgetIdx;

public:
	/** ctor. */
	uiSkinManager();

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

	/** Creates thin horizontal scroll bar. */
	uiScrollBar* horThinScrollBar(const cLayout& layout = cLayout::both(), const string& id = "",
		                          float value = 0, float minValue = 0, float maxValue = 1, float barSize = 0.1f);

	/** Creates thin horizontal scroll bar. */
	uiScrollBar* verThinScrollBar(const cLayout& layout = cLayout::both(), const string& id = "",
		                          float value = 0, float minValue = 0, float maxValue = 1, float barSize = 0.1f);

	/** Creates edit box. */
	uiEditBox* editbox(const cLayout& layout = cLayout::both(), const string& id = "", const string& text = "", 
		               bool multiLine = false);

	/** Creates label. */
	uiLabel* label(const string& text, const cLayout& layout = cLayout::both(), const string& id = "");

	/** Creates scroll area. */
	uiScrollArea* scrollArea(const cLayout& layout = cLayout::both(), const string& id = "");

	/** Creates horizontal layout widget. */
	uiHorLayout* horLayout(const cLayout& layout = cLayout::both(), const string& id= "", float widgetsDistance = 10.0f);

	/** Creates vertical layout widget. */
	uiVerLayout* verLayout(const cLayout& layout = cLayout::both(), const string& id= "", float widgetsDistance = 10.0f);

	/** Creates rectangle pad widget. */
	uiRect* rectPad(const cLayout& layout = cLayout::both(), const string& id = "");


	/** Sets button sample. */
	void setButtonSample(uiButton* buttonSample);

	/** Sets background sample. */
	void setBackgroundSample(uiRect* backgroundSample);

	/** Sets progress bar sample. */
	void setProgressbarSample(uiProgressBar* progressbarSample);

	/** Sets checkbox sample. */
	void setCheckBoxSample(uiCheckBox* checkbox);

	/** Sets horizontal scroll bar sample. */
	void setHorScrollbarSample(uiScrollBar* scrollbar);

	/** Sets horizontal thin scroll bar sample. */
	void setHorThinScrollbarSample(uiScrollBar* scrollbar);

	/** Sets vertical thin scroll bar sample. */
	void setVerThinScrollbarSample(uiScrollBar* scrollbar);

	/** Sets single line edit box sample. */
	void setSingleLineEditBoxSample(uiEditBox* editboxSampl);

	/** Sets multi line edit box sample. */
	void setMultilineEditBoxSample(uiEditBox* editboxSample);

	/** Sets label sample. */
	void setLabelSample(uiLabel* labelSample);

	/** Sets scrollarea sample. */
	void setScrollAreaSample(uiScrollArea* scrollAreaSample);

	/** Sets rectangle pad sample. */
	void setRectPadSample(uiRect* padSample);

protected:
	void addVisibleState(uiWidget* widget, float duration = 0.5f);

	string checkEmptyId(const string& id, const string& prefix);
};

#define uiSkin() uiSkinManager::instancePtr()

CLOSE_O2_NAMESPACE

#endif //UI_SKIN_H
