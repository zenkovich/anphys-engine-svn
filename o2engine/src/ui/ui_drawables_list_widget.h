#ifndef UI_DRAWABLES_LIST_WIDGET_H
#define UI_DRAWABLES_LIST_WIDGET_H

#include "ui_widget.h"
#include "util/graphics/rect_drawable.h"

OPEN_O2_NAMESPACE

/** Widget with drawables tree. */
class uiDrawablesListWidget: public uiWidget
{
public: 
	/** Base drawable class. COntains name, drawable, parent and childs, layout. */
	class Drawable: public PropertyList
	{
		friend class uiDrawablesListWidget;
	public:
		typedef vector< Drawable*> DrawablesVec;

	protected:
		string         mName;           /** Name of drawable. */
		IRectDrawable* mDrawable;       /** Rect drawable. Null, if no drawable. */
		layout        mLayout;         /** Layout. */
		Drawable*      mParentDrawable; /** Parent drawable. NULL if no parent. */
		DrawablesVec   mChildDrawables; /** Child drawables. */

	public:
		PROPERTY(Drawable, layout) layout; /** Layout property. Using set/getlayout. */

		/** ctor. */
		Drawable() {}

		/** ctor. */
		Drawable(const string& name, IRectDrawable* drawable, const layout& layout = layout::both(),
			     Drawable* parentDrawable = NULL);

		/** copy-ctor. */
		Drawable(const Drawable& drawable);

		/** dtor. */
		~Drawable();

		/** Copy operator. */
		Drawable& operator=(const Drawable& drw);

		/** Return name of drawable. */
		string getName() const;

		/** Returns path id. */
		string getPathId() const;

		/** Returns rect drawable. */
		IRectDrawable* getDrawable();

		/** Sets drawable. */
		void setDrawable(IRectDrawable* drawable);

		/** Adding child drawable. */
		Drawable* addChildDrawable(Drawable* drawable);

		/** Adding child drawable. */
		Drawable* addChildDrawable(const string& name, IRectDrawable* drawable, 
			                       const layout& layout = layout::both());

		/** Returns child drawable by path. */
		Drawable* getChildDrawable(const string& path);

		/** Removes child drawable. */
		void removeChildDrawable(Drawable* drawable, bool release = true);

		/** Removes all drawables. */
		void removeAllChildDrawables();

		/** Sets layout. */
		void setLayout(const layout& layout);

		/** Returns layout. */
		layout getLayout() const;

		/** Updates layout. */
		void updateLayoutManual(const vec2f& parPos, const vec2f& parSize);

		/** Updates layout by parent. */
		void updateLayout();

		/** Drawing. */
		void draw();

	protected:
		/** Initializing properties. */
		void initializeProperties();

		/** Calls when layout was updated. */
		void layoutUpdated();
	};
	typedef vector<Drawable*> DrawablesVec;

protected:
	Drawable mBasicDrawable; /** Basic drawable. */

public:
	/** ctor. */
	uiDrawablesListWidget(const layout& layout, const string& id = "");

	/** copy-ctor. */
	uiDrawablesListWidget(const uiDrawablesListWidget& widget);

	/** dtor. */
	~uiDrawablesListWidget();

	/** Returns clone of widget. */
	virtual uiWidget* clone() const;

	/** Adding drawable. */
	Drawable* addDrawable(Drawable* drawable);

	/** Adding drawable with specified id. */
	Drawable* addDrawable(IRectDrawable* drawable, const string& id, const layout& layout = layout::both());

	/** Returns drawable by id. */
	Drawable* getDrawable(const string& path);

	/** Removes drawable. */
	void removeDrawable(const string& id, bool release = true);

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
