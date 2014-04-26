#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <list>

#include "public.h"

#include "util/type_indexation.h"
#include "util/input/input_message.h"
#include "util/math/vector2.h"
#include "util/property.h"
#include "util/geometry/geometry.h"
#include "ui_widget_layout.h"

OPEN_O2_NAMESPACE

class cGeometry;

/** Basic widget object. Contains id, parent, childs, position and other strange data. */
class uiWidget
{
	typedef vector<uiWidget*> WidgetsVec;
	
	uiWidget*      mParent;         /**< Parent widget. NULL if no parent. */
	string    mId;             /**< Identificator or name. */
	uiWidgetLayout mLayout;         /**< Widget layout. */
	vec2f          mLocalPosition;	/**< Position relative to the parent. */
	vec2f          mGlobalPosition; /**< Position in screen space. */
	vec2f          mSize;           /**< Size of widget. Not including childs. */
	vec2f          mChildsOffset;   /**< Offset for childrens. */
	cGeometry*     mGeometry;       /**< Geometry. May be NULL. */
	fRect          mBounds;         /**< Widget with childs bounds. */

	WidgetsVec mChildWidgets;       /**< Childs widgets. */

public:
	typedef uiWidget* uiWidgetPtr;

	//Type definition
	DEFINE_TYPE(uiWidget);

	//properties
	PROPERTY(uiWidget, uiWidgetPtr) parent;         /**< Parent property. Using setParent/getParent. */
	PROPERTY(uiWidget, string) id;             /**< Identificator property. Using setId/getId. */
	PROPERTY(uiWidget, vec2f)       position;       /**< Local position property. Using setPosition/getPosition. */
	PROPERTY(uiWidget, vec2f)       globalPosition; /**< Global position property. Using setGlobalPosition/get.. */
	PROPERTY(uiWidget, vec2f)       size;           /**< Size property. Using setSize/getSize. */


	/** ctor. */
	uiWidget(const string& id = "", uiWidget* parent = NULL, const vec2f& localPos = vec2f());

	/** copy-ctor. */
	uiWidget(const uiWidget& widget);

	/** dtor. */
	virtual ~uiWidget();

	/** Returns clone of widget. */
	virtual uiWidget* clone() const;

	/** Draw widget and childs. */
	virtual void draw();

	/** Update widget and childs. */
	virtual void update(float dt);

	/** Process message in widget, and after in childs. */
	virtual void processInputMessage(const cInputMessage& msg);

	/** Returns true, if point inside widget or inside childs. */
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
		return static_cast<T*>(getWidget(id));
	}

	/** Get widget by id. Id format "some_child/child/", ".." - directs to parent. */
	uiWidget* getWidget(const string& id);


	//setters and getters
	
	/** Sets widget's parent. */
	void setParent(const uiWidgetPtr& parent);

	/** Returns parent ptr. */
	uiWidget* getParent() const;

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
	cGeometry* getGeometry() const;

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
	virtual void localProcessInputMessage(const cInputMessage& msg) {}

	/** Returns true, if point inside current widget. */
	virtual bool isLocalInside(const vec2f& point) const { return false; }

	/** Initialize all properties. */
	void initializeProperties();
};

CLOSE_O2_NAMESPACE

#endif //UI_WIDGET_H