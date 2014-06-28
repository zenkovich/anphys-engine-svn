#ifndef UI_SCROLL_BAR_H
#define UI_SCROLL_BAR_H

#include "ui_drawables_list_widget.h"

OPEN_O2_NAMESPACE

class uiScrollBar: public uiDrawablesListWidget
{	
public:
	enum Type { TP_HORISONTAL = 0, TP_VERTICAL };

protected:
	float     mMinValue;   /** Min value. */
	float     mMaxValue;   /** Max value. */
	float     mValue;      /** Current value. */
	float     mBarSize;    /** Current bar size. */
	Type      mType;       /** Scrolling type. */

	uiState*  mBarHoverState;
	uiState*  mBarPressedState;

	Drawable* mBackground; /** Background drawable. */
	Drawable* mBar;        /** Bar drawable. */

	bool      mPressed;

public:
	PROPERTY(uiScrollBar, float) minValue; /** Min value property. Uses set/getMinValue. */
	PROPERTY(uiScrollBar, float) maxValue; /** Max value property. Uses set/getMaxValue. */
	PROPERTY(uiScrollBar, float) value;    /** Current value. Uses set/getValue. */
	PROPERTY(uiScrollBar, float) barSize;  /** Current bar size. */

	/** ctor. */
	uiScrollBar(const cLayout& layout, const string& id = "", Type type = TP_HORISONTAL, uiWidget* parent = NULL);

	/** copy-ctor. */
	uiScrollBar(const uiScrollBar& scrollbar);

	/** dtor. */
	virtual ~uiScrollBar();

	/** Returns clone of widget. */
	virtual uiWidget* clone() const;

	/** Returns background drawable. */
	Drawable* getBackgroundDrawable();

	/** Returns bar drawable. */
	Drawable* getBarDrawable();

	/** Sets value range. */
	void setValueRange(float minValue, float maxValue);

	/** Sets min value. */
	void setMinValue(float value);

	/** Sets max value. */
	void setMaxValue(float value);

	/** Returns min value. */
	float getMinValue() const;

	/** Returns max value. */
	float getMaxValue() const;

	/** Sets current value. */
	void setValue(float value);

	/** Returns current value. */
	float getValue() const;

	/* Sets bar size. */
	void setBarSize(float size);

	/** Returns bar size. */
	float getBarSize() const;

protected:
	/** Updating current widget. */
	virtual void localUpdate(float dt);

	/** Processing input message in current widget. */
	virtual bool localProcessInputMessage(const cInputMessage& msg);

	/** Updates bar drawable layout. */
	void updateBarLayout();

	/** Initialize properties. */
	void initializeProperties();
};

CLOSE_O2_NAMESPACE

#endif // UI_SCROLL_BAR_H
