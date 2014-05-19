#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <list>
#include <map>

#include "public.h"

#include "util/type_indexation.h"
#include "util/input/input_message.h"
#include "util/math/vector2.h"
#include "util/property.h"
#include "util/geometry/geometry.h"
#include "ui_widget_layout.h"
#include "ui_state.h"

OPEN_O2_NAMESPACE

class cGeometry;
class uiState;

/** Basic widget object. Contains id, parent, childs, position and other strange data. */
class uiWidget
{
	friend class uiController;

	typedef vector< shared<uiWidget> > WidgetsVec;
	typedef std::map< string, shared<uiState> > StatesMap;
	
	string            mId;             /**< Identificator or name. */
	shared<uiWidget>  mParent;         /**< Parent widget. NULL if no parent. */
	uiWidgetLayout    mLayout;         /**< Widget layout. */
	WidgetsVec        mChildWidgets;   /**< Chiles widgets. */
	vec2f             mLocalPosition;  /**< Position relative to the parent. */
	vec2f             mGlobalPosition; /**< Position in screen space. */
	vec2f             mSize;           /**< Size of widget. Not including childes. */
	vec2f             mChildsOffset;   /**< Offset for childrens. */
	shared<cGeometry> mGeometry;       /**< Geometry. May be NULL. */
	fRect             mBounds;         /**< Widget with childes bounds. */
	bool              mVisible;
	bool              mFocused;
				      
	StatesMap         mStates;
	shared<uiState>   mVisibleState;


public:
	typedef uiWidget* uiWidgetPtr;

	//Type definition
	DEFINE_TYPE(uiWidget);

	//properties
	PROPERTY(uiWidget, shared<uiWidget>) parent;         /**< Parent property. Using setParent/getParent. */
	PROPERTY(uiWidget, string)           id;             /**< Identificator property. Using setId/getId. */
	PROPERTY(uiWidget, vec2f)            position;       /**< Local position property. Using setPosition/getPosition. */
	PROPERTY(uiWidget, vec2f)            globalPosition; /**< Global position property. Using setGlobalPosition/get.. */
	PROPERTY(uiWidget, vec2f)            size;           /**< Size property. Using setSize/getSize. */


	/** ctor. */
	uiWidget(const uiWidgetLayout& layout, const string& id = "", shared<uiWidget> parent = NULL);

	/** copy-ctor. */
	uiWidget(const uiWidget& widget);

	/** dtor. */
	virtual ~uiWidget();

	/** Returns clone of widget. */
	virtual shared<uiWidget> clone() const;

	/** Draw widget and childes. */
	virtual void draw();

	/** Update widget and childes. */
	virtual void update(float dt);

	/** Process message in widget, and after in childes. */
	virtual bool processInputMessage(const cInputMessage& msg);

	/** Returns true, if point inside widget or inside childes. */
	virtual bool isInside(const vec2f& point) const;

	/** Adding child widget. */
	virtual shared<uiWidget> addChild(shared<uiWidget> widget);

	/** Removing child widget. */
	virtual void removeChild(shared<uiWidget> widget);

	/** Remove all child widgets. */
	virtual void removeAllChilds();

	/** Get widget by id. Id format "some_child/child/", ".." - directs to parent. */
	template<typename T>
	shared<T> getWidgetByType(const string& id)
	{
		return shared<T>(getWidget(id));
	}

	/** Get widget by id. Id format "some_child/child/", ".." - directs to parent. */
	shared<uiWidget> getWidget(const string& id);


	//setters and getters

	virtual bool isFocusable() const;

	void setFocused(bool focused);
	bool isFocused() const;
	void makeFocused();
	void releaseFocus();

	void setState(const string& stateId, bool value);
	shared<uiState> getState(const string& stateId);

	void setVisible(bool visible);
	bool isVisible() const;
	
	/** Sets widget's parent. */
	void setParent(const shared<uiWidget>& parent);

	/** Returns parent ptr. */
	shared<uiWidget> getParent() const;

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

	/** Returns geometry ptr. */
	shared<cGeometry> getGeometry() const;

private:
	/** Updating current and child layouts: global positions and bounds. */
	virtual void updateLayout();

	/** Drawing current widget. */
	virtual void localDraw() {}

	/** Updating current widget. */
	virtual void localUpdate(float dt) {}

	/** Updating current widget layout: global position and bounds. */
	virtual void localUpdateLayout();

	/** Processing input message in current widget. */
	virtual bool localProcessInputMessage(const cInputMessage& msg) { return false; }

	/** Returns true, if point inside current widget. */
	virtual bool isLocalInside(const vec2f& point) const { return false; }

	virtual void onFocused();

	virtual void onFocusLost();

	/** Initialize all properties. */
	void initializeProperties();
};

CLOSE_O2_NAMESPACE

#endif //UI_WIDGET_H