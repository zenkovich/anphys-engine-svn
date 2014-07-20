#ifndef UI_SCROLLAREA_H
#define UI_SCROLLAREA_H

#include "ui_drawables_list_widget.h"

OPEN_O2_NAMESPACE

class uiScrollBar;

class uiScrollArea: public uiDrawablesListWidget
{
	friend class uiStdSkinInitializer;

	uiScrollBar* mHorScrollbar;   /** Horisontal scroll bar child widget. */
	uiScrollBar* mVerScrollbar;   /** Vertical scroll bar child widget. */
	vec2f        mSmoothScroll;   /** Smooth scroll. */
	vec2f        mScroll;         /** Current scrolling. */
	vec2f        mScrollSpeed;    /** Current scrolling speed. */
	cLayout      mClippingLayout; /** Crrent clipping layout. */

public:
	//Type definition
	DEFINE_TYPE(uiScrollArea);

	cCallbackChain onScrolled;

	//properties
	PROPERTY(uiScrollArea, vec2f) scroll; /**< Scrolleed offset property. Using setScroll/getScroll. */


	/** ctor. */
	uiScrollArea(const cLayout& layout, uiScrollBar* horBarSample = NULL, uiScrollBar* verBarSample = NULL, 
		         const string& id = "");

	/** copy-ctor. */
	uiScrollArea(const uiScrollArea& scrollarea);

	/** dtor. */
	virtual ~uiScrollArea();

	/** Returns clone of widget. */
	virtual uiWidget* clone() const;

	/** Draw widget and childes. */
	virtual void draw();

	/** Returns true, if widget can take focus. */
	virtual bool isFocusable() const;

	/** Sets horizontal scroll bar sample. */
	void setHorScrollbar(uiScrollBar* scrollbar);

	/** Sets vertical scroll bar sample. */
	void setVerScrollbar(uiScrollBar* scrollbar);

	/** Sets current scroll. */
	void setScroll(const vec2f& scroll);

	/** Returns current scroll. */
	vec2f getScroll() const;

protected:
	/** Updating current and child layouts: global positions and bounds. */
	virtual void updateLayout();

	/** Updating current widget. */
	virtual void localUpdate(float dt);

	/** Processing input message in current widget. */
	virtual bool localProcessInputMessage(const cInputMessage& msg);

	/** Calls when widget focused. */
	virtual void onFocused();

	/** Calls when widget lost focus. */
	virtual void onFocusLost();

	/** Cals when hor or vertical bar moved. */
	void scrollChanged();

	/** Initializing properties. */
	void initializeProperties();
};

CLOSE_O2_NAMESPACE

#endif // UI_SCROLLAREA_H
