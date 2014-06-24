#ifndef UI_PROGRESSBAR_H
#define UI_PROGRESSBAR_H

#include "ui_drawables_list_widget.h"

OPEN_O2_NAMESPACE

class uiProgressBar: public uiDrawablesListWidget
{	
protected:
	float     mMinValue;   /** Min value. */
	float     mMaxValue;   /** Max value. */
	float     mValue;      /** Current value. */

	Drawable* mBackground; /** Background drawable. */
	Drawable* mBar;        /** Bar drawable. */

public:
	PROPERTY(uiProgressBar, float) minValue; /** Min value property. Uses set/getMinValue. */
	PROPERTY(uiProgressBar, float) maxValue; /** Max value property. Uses set/getMaxValue. */
	PROPERTY(uiProgressBar, float) value;    /** Current value. Uses set/getValue. */

	/** ctor. */
	uiProgressBar(const cLayout& layout, const string& id = "", uiWidget* parent = NULL);

	/** copy-ctor. */
	uiProgressBar(const uiProgressBar& widget);

	/** dtor. */
	virtual ~uiProgressBar();

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

protected:
	/** Updating current widget. */
	virtual void localUpdate(float dt);

	/** Updates bar drawable layout. */
	void updateBarLayout();

	/** Initialize properties. */
	void initializeProperties();
};

CLOSE_O2_NAMESPACE

#endif // UI_PROGRESSBAR_H
