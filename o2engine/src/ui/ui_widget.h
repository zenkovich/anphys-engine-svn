#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <list>
#include <map>

#include "public.h"

#include "ui_state.h"
#include "util/geometry/geometry.h"
#include "util/input/input_message.h"
#include "util/math/layout.h"
#include "util/math/vector2.h"
#include "util/property.h"
#include "util/property_list.h"
#include "util/type_indexation.h"

OPEN_O2_NAMESPACE

class cGeometry;
class uiState;

/** Basic widget object. Contains id, parent, childes, position and other strange data. */
class uiWidget: public cPropertyList
{
	friend class uiController;
	friend class uiState;
	friend class uiSkinManager;

public:
	typedef vector< uiWidget* > WidgetsVec;
	typedef std::map< string, uiState* > StatesMap;
	
protected:
	string      mId;                    /**< Identificator or name. */
	uiWidget*  mParent;                /**< Parent widget. NULL if no parent. */
	cLayout    mLayout;                /**< Widget layout. */
	WidgetsVec mChildWidgets;          /**< Chiles widgets. */
	vec2f      mGlobalPosition;        /**< Position in screen space. */
	vec2f      mSize;                  /**< Size of widget. Not including childes. */
	vec2f      mChildsOffset;          /**< Offset for childrens. */
	cGeometry* mGeometry;              /**< Colliding geometry. May be NULL. */
	fRect      mBounds;                /**< Widget with childes bounds. */
	bool       mVisible;               /**< True, if widget is visible. */
	bool       mFocused;               /**< True, if widget on focus. */
	bool       mCursorInside;          /**< True, when cursor is inside widget. */
	float      mTransparency;          /**< Transparency of widget. */
								          
	StatesMap  mStates;                /**< States map. */
	uiState*   mVisibleState;          /**< Shared to visible state. */

	uint32     mUpdatedAtFrame;        /** Last update frame index. */
	uint32     mDrawedAtFrame;         /** Last drawing frame index. */
	uint32     mProcessedInputAtFrame; /** Last input processing frame index. */


public:
	typedef uiWidget* uiWidgetPtr;

	//Type definition
	DEFINE_TYPE(uiWidget);

	cCallbackChain onVisibleOn;
	cCallbackChain onVisibleOff;

	//properties
	PROPERTY(uiWidget, uiWidget*) parent;         /**< Parent property. Using setParent/getParent. */
	PROPERTY(uiWidget, string)    id;             /**< Identificator property. Using setId/getId. */
	PROPERTY(uiWidget, vec2f)     position;       /**< Local position property. Using setPosition/getPosition. */
	PROPERTY(uiWidget, vec2f)     globalPosition; /**< Global position property. Using setGlobalPosition/get.. */
	PROPERTY(uiWidget, vec2f)     size;           /**< Size property. Using setSize/getSize. */
	PROPERTY(uiWidget, bool)      visible;        /**< Visibility property. Using set/isVisible. */
	PROPERTY(uiWidget, cLayout)   layout;         /**< Widget layout. Using set/getLayout. */
	PROPERTY(uiWidget, float)     transparency;   /**< Widget dtransparency. Using set/getTransparency. */


	/** ctor. */
	uiWidget(const cLayout& layout, const string& id = "");

	/** copy-ctor. */
	uiWidget(const uiWidget& widget);

	/** dtor. */
	virtual ~uiWidget();

	/** Returns clone of widget. */
	virtual uiWidget* clone() const;

	/** Draw widget and childes. */
	virtual void draw();

	/** Update widget and childes. */
	virtual void update(float dt);

	/** Process message in widget, and after in childes. */
	virtual bool processInputMessage(const cInputMessage& msg);

	/** Returns true, if point inside widget or inside childes. */
	virtual bool isInside(const vec2f& point) const;

	/** Adding child widget. */
	virtual uiWidget* addChild(uiWidget* widget);

	/** Removing child widget. */
	virtual void removeChild(uiWidget* widget);

	/** Remove all child widgets. */
	virtual void removeAllChilds();

	/** Get widget by id. Id format "some_child/child/", ".." - directs to parent. */
	template<typename T>
	T* getWidgetByType(const string& id)
	{
		return T*(getWidget(id));
	}

	/** Get widget by id. Id format "some_child/child/", ".." - directs to parent. */
	uiWidget* getWidget(const string& id);

	/** Returns widget's path from this widget. */
	string getWidgetPath(uiWidget* widget) const;


	//setters and getters
	/** Returns true, if widget can take focus. */
	virtual bool isFocusable() const;

	/** Setting focus on widget. */
	void setFocused(bool focused);

	/** Return true, when widget on focus. */
	bool isFocused() const;

	/** Makes widget focused. */
	void makeFocused();

	/** Releases widgets focus. */
	void releaseFocus();

	/** Adding new state to widget. */
	uiState* addState(uiState* state);

	/** Setting state parameter. */
	void setState(const string& stateId, bool value);

	/** Returns shared state by id. */
	uiState* getState(const string& stateId);

	/** Sets widget visible. */
	void setVisible(bool visible);

	/** Returns true, if widget is visible. */
	bool isVisible() const;
	
	/** Sets widget's parent. */
	void setParent(uiWidget* parent);

	/** Returns parent ptr. */
	uiWidget* getParent();

	/** Sets the local position. */
	void setPosition(const vec2f& position);

	/** Returns local position. */
	vec2f getPosition() const;

	/** Sets the id. */
	void setId(const string& id);

	/** Returns the id. */
	string getId() const;

	/** Sets the global position. */
	void setGlobalPosition(const vec2f& position);

	/** Returns the global position. */
	vec2f getGlobalPosition() const;

	/** Sets the size. */
	void setSize(const vec2f& size);

	/** Returns size. */
	vec2f getSize() const;

	/** Sets widget layout. */
	void setLayout(const cLayout& layout);

	/** Returns widget layout. */
	cLayout getlayout() const;

	/** Sets the colliding geometry. */
	void setGeometry(cGeometry* geometry);

	/** Returns colliding geometry ptr. */
	cGeometry* getGeometry() const;

	/** Sets transparency of widget. */
	void setTransparency(float transparency);

	/** Returns widget transparency. */
	float getTransparency() const;

protected:
	/** Updating current and child layouts: global positions and bounds. */
	virtual void updateLayout();

	/** Calls when added some state. */
	virtual void addedState(uiState* state) {}

	/** Updating states. */
	virtual void updateStates(float dt);

	/** Drawing current widget. */
	virtual void localDraw() {}

	/** Updating current widget. */
	virtual void localUpdate(float dt) {}

	/** Updating current widget layout: global position and bounds. */
	virtual void localUpdateLayout();

	/** Calls when widget's layout updated. */
	virtual void layoutUpdated() {}

	/** Processing input message in current widget. */
	virtual bool localProcessInputMessage(const cInputMessage& msg) { return false; }

	/** Returns true, if point inside current widget. */
	virtual bool isLocalInside(const vec2f& point) const { return true; }

	/** Calls when widget focused. */
	virtual void onFocused();

	/** Calls when widget lost focus. */
	virtual void onFocusLost();

	/** Initialize all properties. */
	void initializeProperties();

	/** Setting mVisible value. */
	void setVisibleParam(bool param);
};

CLOSE_O2_NAMESPACE

#endif //UI_WIDGET_H