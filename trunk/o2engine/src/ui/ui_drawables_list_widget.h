#ifndef UI_DRAWABLES_LIST_WIDGET_H
#define UI_DRAWABLES_LIST_WIDGET_H

#include "ui_widget.h"
#include "util/graphics/rect_drawable.h"

OPEN_O2_NAMESPACE

class uiDrawablesListWidget: public uiWidget
{
public: 
	class Drawable: public cPropertyList
	{
		friend class uiDrawablesListWidget;
	public:
		typedef vector< shared<Drawable> > DrawablesVec;

	protected:
		string                  mName;
		shared<IRectDrawable>   mDrawable;
		cLayout                 mLayout;
		shared<Drawable>        mParentDrawable;
		DrawablesVec            mChildDrawables;

	public:
		PROPERTY(Drawable, cLayout) layout;

		Drawable() {}
		Drawable(const string& name, const shared<IRectDrawable>& drawable, const cLayout& layout = cLayout::both(),
			     const shared<Drawable>& parentDrawable = NULL);
		Drawable(const Drawable& drawable);
		~Drawable();

		string getName() const;

		shared<IRectDrawable> getDrawable();

		void setDrawable(const shared<IRectDrawable>& drawable);

		shared<Drawable> addChildDrawable(const shared<Drawable>& drawable);

		shared<Drawable> addChildDrawable(const string& name, const shared<IRectDrawable>& drawable, 
			                              const cLayout& layout = cLayout::both());

		shared<Drawable> getChildDrawable(const string& path);

		void removeChildDrawable(shared<Drawable>& drawable, bool release = true);

		void removeAllChildDrawables();

		void setLayout(const cLayout& layout);

		cLayout getLayout() const;

		void updateLayout(const vec2f& parPos, const vec2f& parSize);

		void draw();

	protected:
		void initializeProperties();
	};
	typedef vector< shared<Drawable> > DrawablesVec;

protected:
	DrawablesVec mDrawables;

public:
	uiDrawablesListWidget(const cLayout& layout, const string& id = "", shared<uiWidget> parent = NULL);

	/** copy-ctor. */
	uiDrawablesListWidget(const uiDrawablesListWidget& widget);

	/** dtor. */
	~uiDrawablesListWidget();

	/** Returns clone of widget. */
	virtual shared<uiWidget> clone() const;

	/** Adding drawable. */
	shared<Drawable> addDrawable(const shared<Drawable>& drawable);

	/** Adding drawable with specified id. */
	shared<Drawable> addDrawable(const shared<IRectDrawable>& drawable, const string& id, 
		                         const cLayout& layout = cLayout::both(), const shared<Drawable>& parentDrawable = NULL);

	/** Returns drawable by id. */
	shared<Drawable> getDrawable(const string& path);

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
	virtual void addedDrawable(const shared<Drawable>& drawable) {}
};

CLOSE_O2_NAMESPACE

#endif //UI_DRAWABLES_LIST_WIDGET_H
