#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <vector>
#include <list>

#include "public.h"

#include "util/type_indexation.h"
#include "util/input/input_message.h"
#include "util/math/vector2.h"
#include "util/property.h"

OPEN_O2_NAMESPACE

class uiWidget
{
	typedef std::vector<uiWidget*> WidgetsList;

	vec2f       mGlobalPosition;
	vec2f       mChildsOffset;

	uiWidget*   mParent;
	WidgetsList mChildWidgets;

public:
	DEFINE_TYPE(uiWidget);

	cProperty<std::string> mId;
	cProperty<vec2f>       mLocalPosition;

	uiWidget(const std::string& id = "", uiWidget* parent = NULL, const vec2f& localPos = vec2f());
	uiWidget(const uiWidget& widget);
	virtual ~uiWidget();

	virtual uiWidget* clone() const;

	virtual void draw();
	virtual void update(float dt);

	virtual void processInputMessage(const cInputMessage& msg);

	virtual uiWidget* addChild(uiWidget* widget);
	virtual bool removeChild(uiWidget* widget);
	virtual void removeAllChilds();

	virtual void setParent(uiWidget* parent);

	template<typename T>
	T* getWidgetByType(const std::string& id)
	{
		return static_cast<T*>(getWidget(id));
	}

	uiWidget* getWidget(const std::string& id);

private:
	virtual void localDraw() {}
	virtual void localUpdate(float dt) {}
	virtual void localProcessInputMessage(const cInputMessage& msg) {}
};

/*template<typename T = uiWidget>
T& uiWidget<T>::getWidget( const std::string& id )
{
	int delPos = id.find("/");
	std::string pathPart = id.substr(delPos);

	for (WidgetsList::iterator it = mW)
}*/

CLOSE_O2_NAMESPACE

#endif //UI_WIDGET_H