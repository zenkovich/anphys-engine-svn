#ifndef WIDGET_H
#define WIDGET_H

#include <string>
#include <vector>

#include "util/math/mmath.h"
#include "util/other/type_intexation.h"
#include "util/serialization/serialization.h"

struct uiWidget
{
	typedef std::vector<uiWidget*> WidgetsList;

	DEFINE_TYPE(uiWidget)


	std::string mId;

	uiWidget*   mParent;
	WidgetsList mChilds;

	vec2        mPosition;
	vec2        mOffset;
	float       mTransparency;
	float       mResTransparency;
	bool        mModal;

//functions
	uiWidget(const std::string& id = "noName", uiWidget* parent = NULL);
	uiWidget(const uiWidget& widget);
	virtual ~uiWidget();

	void addChild(uiWidget* widget);

	void removeChild(uiWidget* widget);
	void removeAllChilds();

	uiWidget* getWidget(const std::string& id) const;

	template<typename T>
	T* getWidgetByType(const std::string& id) const
	{
		return static_cast<T*>(getWidget(id));
	}

	virtual void update(float dt);
	virtual void draw();

	virtual uiWidget* clone() const;

	serializeMethodDecl(uiWidget);
};

#endif //WIDGET_H