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
		typedef vector< Drawable*> DrawablesVec;

	protected:
		string         mName;
		IRectDrawable* mDrawable;
		cLayout        mLayout;
		Drawable*      mParentDrawable;
		DrawablesVec   mChildDrawables;

	public:
		PROPERTY(Drawable, cLayout) layout;

		Drawable() {}
		Drawable(const string& name, IRectDrawable* drawable, const cLayout& layout = cLayout::both(),
			     Drawable* parentDrawable = NULL);
		Drawable(const Drawable& drawable);
		~Drawable();

		string getName() const;

		IRectDrawable* getDrawable();

		void setDrawable(IRectDrawable* drawable);

		Drawable* addChildDrawable(Drawable* drawable);

		Drawable* addChildDrawable(const string& name, IRectDrawable* drawable, 
			                              const cLayout& layout = cLayout::both());

		Drawable* getChildDrawable(const string& path);

		void removeChildDrawable(Drawable* drawable, bool release = true);

		void removeAllChildDrawables();

		void setLayout(const cLayout& layout);

		cLayout getLayout() const;

		void updateLayout(const vec2f& parPos, const vec2f& parSize);

		void draw();

	protected:
		void initializeProperties();
	};
	typedef vector<Drawable*> DrawablesVec;

protected:
	DrawablesVec mDrawables;

public:
	uiDrawablesListWidget(const cLayout& layout, const string& id = "", uiWidget* parent = NULL);

	/** copy-ctor. */
	uiDrawablesListWidget(const uiDrawablesListWidget& widget);

	/** dtor. */
	~uiDrawablesListWidget();

	/** Returns clone of widget. */
	virtual uiWidget* clone() const;

	/** Adding drawable. */
	Drawable* addDrawable(Drawable* drawable);

	/** Adding drawable with specified id. */
	Drawable* addDrawable(IRectDrawable* drawable, const string& id, 
		                  const cLayout& layout = cLayout::both(), Drawable* parentDrawable = NULL);

	/** Returns drawable by id. */
	Drawable* getDrawable(const string& path);

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
	virtual void addedDrawable(Drawable* drawable) {}
};

CLOSE_O2_NAMESPACE

#endif //UI_DRAWABLES_LIST_WIDGET_H
