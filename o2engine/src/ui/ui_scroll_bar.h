#ifndef UI_SCROLL_BAR_H
#define UI_SCROLL_BAR_H

#include "ui_drawables_list_widget.h"

OPEN_O2_NAMESPACE

class uiScrollBar: public uiDrawablesListWidget
{	
	friend class uiStdSkinInitializer;

public:
	enum Type { TP_HORISONTAL = 0, TP_VERTICAL };

protected:
	float     mMinValue;        /** Min value. */
	float     mMaxValue;        /** Max value. */
	float     mValue;           /** Current value. */
	float     mBarSize;         /** Current bar size. */
	Type      mType;            /** Scrolling type. */

	uiState*  mBarHoverState;   /** Bar hover state. */
	uiState*  mBarPressedState; /** Bar pressed state. */

	Drawable* mBar;             /** Bar drawable. */
	cLayout   mBarGeometry;     /** bar clicking geometry. */
	cLayout   mBackgrGeometry;  /** Background geometry. */
	 
	bool      mPressed;         /** True, when bar pressed. */
	bool      mHover;           /** True, when cursor hover bar. */

public:
	PROPERTY(uiScrollBar, float) minValue; /** Min value property. Uses set/getMinValue. */
	PROPERTY(uiScrollBar, float) maxValue; /** Max value property. Uses set/getMaxValue. */
	PROPERTY(uiScrollBar, float) value;    /** Current value. Uses set/getValue. */
	PROPERTY(uiScrollBar, float) barSize;  /** Current bar size. */

	cCallbackChain onValueChangedEvent; /** On changed value event. */

	/** ctor. */
	uiScrollBar(const cLayout& layout, const string& id = "", Type type = TP_HORISONTAL);

	/** copy-ctor. */
	uiScrollBar(const uiScrollBar& scrollbar);

	/** dtor. */
	virtual ~uiScrollBar();

	/** Returns clone of widget. */
	virtual uiWidget* clone() const;

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

	/** Sets bar geomatry layout. */
	void setBarGeometryLayout(const cLayout& layout);

	/** Sets background geometry layout. */
	void setBackgroundGeometryLayout(const cLayout& layout);

protected:
	/** Returns bar drawable. */
	Drawable* getBarDrawable();

	/** Sets bar drawable. */
	void setBarDrawable(Drawable* drawable);

	/** Updating current widget. */
	virtual void localUpdate(float dt);

	/** Calls when widget's layout updated. */
	virtual void layoutUpdated();

	/** Processing input message in current widget. */
	virtual bool localProcessInputMessage(const cInputMessage& msg);

	/** Returns true, if point inside current widget. */
	virtual bool isLocalInside(const vec2f& point) const;

	/** Updates bar drawable layout. */
	void updateBarLayout();

	/** Initialize properties. */
	void initializeProperties();
};

CLOSE_O2_NAMESPACE

#endif // UI_SCROLL_BAR_H
