#ifndef UI_DRAWABLES_LIST_WIDGET_H
#define UI_DRAWABLES_LIST_WIDGET_H

#include "ui_widget.h"
#include "util/graphics/rect_drawable.h"
#include <map>

OPEN_O2_NAMESPACE

class uiDrawablesListWidget: public uiWidget
{
public: 
	typedef std::map< string, shared<IRectDrawable> > DrawablesMap;

protected:
	DrawablesMap mDrawables;

public:
	uiDrawablesListWidget(const uiWidgetLayout& layout, const string& id = "", shared<uiWidget> parent = NULL);

	/** copy-ctor. */
	uiDrawablesListWidget(const uiWidget& widget);

	/** dtor. */
	~uiDrawablesListWidget();

	/** Returns clone of widget. */
	virtual shared<uiWidget> clone() const;

	shared<IRectDrawable> addDrawable(const shared<IRectDrawable>& drawable, const string& id);
	shared<IRectDrawable> getDrawable(const string& id);
	void removeDrawable(const string& id);
	void removeAllDrawables();

protected:
	/** Drawing current widget. */
	virtual void localDraw();
};

CLOSE_O2_NAMESPACE

#endif //UI_DRAWABLES_LIST_WIDGET_H
