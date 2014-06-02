#ifndef UI_DRAWABLES_LIST_WIDGET_H
#define UI_DRAWABLES_LIST_WIDGET_H

#include "ui_widget.h"
#include "util/graphics/rect_drawable.h"

OPEN_O2_NAMESPACE

class uiDrawablesListWidget: public uiWidget
{
public: 
	struct DrawableContainer
	{
		string                mName;
		uiWidgetLayout        mLayout;
		shared<IRectDrawable> mDrawable;

		DrawableContainer() {}
		DrawableContainer(const string& name, const shared<IRectDrawable>& drawable, 
			              const uiWidgetLayout& layout = uiBothLayout()):
			mName(name), mDrawable(drawable), mLayout(layout) {}
	};
	typedef vector<DrawableContainer> DrawablesVec;

protected:
	DrawablesVec mDrawables;

public:
	uiDrawablesListWidget(const uiWidgetLayout& layout, const string& id = "", shared<uiWidget> parent = NULL);

	/** copy-ctor. */
	uiDrawablesListWidget(const uiDrawablesListWidget& widget);

	/** dtor. */
	~uiDrawablesListWidget();

	/** Returns clone of widget. */
	virtual shared<uiWidget> clone() const;

	/** Adding drawable with specified id. */
	shared<IRectDrawable> addDrawable(const shared<IRectDrawable>& drawable, const string& id, 
		                              const uiWidgetLayout& layout = uiBothLayout());

	/** Returns drawable by id. */
	shared<IRectDrawable> getDrawable(const string& id);

	/** Removes drawable. */
	void removeDrawable(const string& id);

	/** Removes all drawables. */
	void removeAllDrawables();

protected:
	/** Drawing current widget. */
	virtual void localDraw();

	/** Calls when widget's layout updated. */
	virtual void layoutUpdated();

	/** Calls when added drawable. */
	virtual void addedDrawable(const shared<IRectDrawable>& drawable, const string& id) {}
};

CLOSE_O2_NAMESPACE

#endif //UI_DRAWABLES_LIST_WIDGET_H
