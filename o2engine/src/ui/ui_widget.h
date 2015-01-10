#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <list>
#include <map>

#include "public.h"

#include "ui_state.h"
#include "util/callback.h"
#include "util/geometry/geometry.h"
#include "util/input/input_message.h"
#include "util/math/layout.h"
#include "util/math/vector2.h"
#include "util/property.h"
#include "util/property_list.h"
#include "util/type_indexation.h"

OPEN_O2_NAMESPACE

class Geometry;
class uiState;

/** Basic widget object. Contains id, parent, childes, position and other strange data. */
class uiWidget: public PropertyList
{
	friend class uiController;
	friend class uiState;
	friend class uiSkinManager;
	friend class uiScrollArea;
	friend class uiStdSkinInitializer;
	friend class uiHorLayout;
	friend class uiVerLayout;

public:
	typedef vector< uiWidget* > WidgetsVec;
	typedef std::map< string, uiState* > StatesMap;
	
protected:
	string         mId;                      /**< Identificator or name. */
	string         mHint;                    /**< Hint text. */
	uiWidget*      mParent;                  /**< Parent widget. NULL if no parent. */
	layout        mLayout;                  /**< Widget layout. */
	layout        mChildsLayout;            /**< Childes layout. */
	WidgetsVec     mChildWidgets;            /**< Childes widgets. */
	vec2f          mGlobalPosition;          /**< Position in screen space. */
	vec2f          mSize;                    /**< Size of widget. Not including childes. */
	vec2f          mAbsMinSize;              /**< Absolute widget minimal size, depends on mLayout and child mResMinSize. */
	vec2f          mFixedMinSize;            /**< Widgets fixed min size. */
	vec2f          mChildsOffset;            /**< Offset for childrens. */
	Geometry*     mGeometry;                /**< Colliding geometry. May be NULL. */
	fRect          mBounds;                  /**< Widget with childes bounds. */
	bool           mResizeByChilds;          /**< Resize by childes, if true, widget will be resized so that all fit. */
	bool           mVisible;                 /**< True, if widget is visible. */
	bool           mFocused;                 /**< True, if widget on focus. */
	bool           mCursorInside;            /**< True, when cursor is inside widget. */
	float          mBasicTransparency;       /**< Basic transparency of widget. */
	float          mResTransparency;         /**< Result transparency of widget. */
				    				            
	StatesMap      mStates;                  /**< States map. */
	uiState*       mVisibleState;            /**< Shared to visible state. */
				    					     
	uint32         mUpdatedAtFrame;          /** Last update frame index. */
	uint32         mDrawedAtFrame;           /** Last drawing frame index. */
	uint32         mProcessedInputAtFrame;   /** Last input processing frame index. */

	CallbackChain mResTransparencyChanged;  /** Calls when result transparency changed. */
	ICallback*     mCheckParentTransparency; /** Calls when parent transparency changing. */

public:
	typedef uiWidget* uiWidgetPtr;

	//Type definition
	DEFINE_TYPE(uiWidget);

	CallbackChain onVisibleOn;
	CallbackChain onVisibleOff;

	//properties
	PROPERTY(uiWidget, uiWidget*) parent;         /**< Parent property. Using setParent/getParent. */
	PROPERTY(uiWidget, string)    id;             /**< Identificator property. Using setId/getId. */
	PROPERTY(uiWidget, vec2f)     position;       /**< Local position property. Using setPosition/getPosition. */
	PROPERTY(uiWidget, vec2f)     globalPosition; /**< Global position property. Using setGlobalPosition/get.. */
	PROPERTY(uiWidget, vec2f)     size;           /**< Size property. Using setSize/getSize. */
	PROPERTY(uiWidget, bool)      visible;        /**< Visibility property. Using set/isVisible. */
	PROPERTY(uiWidget, layout)   layout;         /**< Widget layout. Using set/getLayout. */
	PROPERTY(uiWidget, float)     transparency;   /**< Widget transparency. Using set/getTransparency. */


	/** ctor. */
	uiWidget(const layout& layout, const string& id = "");

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
	virtual bool processInputMessage(const InputMessage& msg);

	/** Returns true, if point inside widget or inside childes. */
	virtual bool isInside(const vec2f& point) const;

	/** Adding child widget. If position negative, adding at end. */
	virtual uiWidget* addChild(uiWidget* widget, int position = -1);

	/** Adding child widget and returns correct type. If position negative, adding at end. */
	template<typename T>
	T* addTChild(T* widget, int position = -1)
	{
		return static_cast<T*>(addChild(widget, position));
	}

	/** Removing child widget. */
	virtual void removeChild(uiWidget* widget, bool release = true);

	/** Remove all child widgets. */
	virtual void removeAllChilds();

	/** Get widget by id. Id format "some_child/child/", ".." - directs to parent. */
	template<typename T>
	T* getWidgetByType(const string& id)
	{
		return (T*)(getWidget(id));
	}

	/** Get widget by id. Id format "some_child/child/", ".." - directs to parent. */
	uiWidget* getWidget(const string& id);

	/** Returns widget's path from this widget. */
	string getWidgetPath(uiWidget* widget) const;


	//setters and getters
	/** Returns hint string. */
	virtual string getHint() const { return ""; }

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
	void setLayout(const layout& layout);

	/** Returns widget layout. */
	layout getLayout() const;

	/** Sets childes layout. */
	void setChildsLayout(const layout& layout);

	/** Returns childes layout. */
	layout getChildsLayout() const;

	/** Sets the colliding geometry. */
	void setGeometry(Geometry* geometry);

	/** Returns colliding geometry ptr. */
	Geometry* getGeometry() const;

	/** Sets transparency of widget. */
	void setTransparency(float transparency);

	/** Returns widget transparency. */
	float getTransparency() const;

	/** Sets resizing by childs. */
	void setRisizeByChilds(bool flag);

	/** Returns true, if resizing by childs. */
	bool isResizingByChilds() const;

protected:
	/** Updating current and child layouts: global positions and bounds. */
	virtual void updateLayout(bool force = false, bool onlyDownUpdates = false);

	/** Calls when added some state. */
	virtual void addedState(uiState* state) {}

	/** Updating states. */
	virtual void updateStates(float dt);

	/** Drawing current widget. */
	virtual void localDraw() {}

	/** Updating current widget. */
	virtual void localUpdate(float dt) {}

	/** Updating current widget layout: global position and bounds. */
	virtual bool localUpdateLayout();

	/** Calls when widget's layout updated. */
	virtual void layoutUpdated() {}

	/** Processing input message in current widget. */
	virtual bool localProcessInputMessage(const InputMessage& msg) { return false; }

	/** Returns true, if point inside current widget. */
	virtual bool isLocalInside(const vec2f& point) const { return true; }

	/** Calls when widget focused. */
	virtual void onFocused();

	/** Calls when widget lost focus. */
	virtual void onFocusLost();

	/** Calls when parent transparency changed. */
	virtual void updateResTransparency();

	/** Sets widget layout with parametres. */
	void setLayoutParams(const layout& layout, bool onlyDownUpdate);

	/** Calls when need to update result min size. */
	void updateAbsMinSize();

	/** Checking size by childs. */
	void checkResizingByChilds();

	/** Initialize all properties. */
	void initializeProperties();

	/** Setting mVisible value. */
	void setVisibleParam(bool param);

	/** Drawing debug frame. */
	void drawDebugFrame();
};

CLOSE_O2_NAMESPACE

#endif //UI_WIDGET_H