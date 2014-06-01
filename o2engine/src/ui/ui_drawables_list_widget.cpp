#include "ui_drawables_list_widget.h"

OPEN_O2_NAMESPACE

uiDrawablesListWidget::uiDrawablesListWidget(const uiWidgetLayout& layout, const string& id /*= ""*/, 
                                             shared<uiWidget> parent /*= NULL*/):
	uiWidget(layout, id, parent)
{
}

uiDrawablesListWidget::uiDrawablesListWidget(const uiWidget& widget):
	uiWidget(widget)
{
	FOREACH(DrawablesMap, mDrawables, drw)
		addDrawable(drw->second, drw->first);
}

uiDrawablesListWidget::~uiDrawablesListWidget()
{
	removeAllDrawables();
}

shared<uiWidget> uiDrawablesListWidget::clone() const
{
	return mnew uiDrawablesListWidget(*this);
}

shared<IRectDrawable> uiDrawablesListWidget::addDrawable(const shared<IRectDrawable>& drawable, const string& id)
{
	mDrawables[id] = drawable;
	return drawable;
}

shared<IRectDrawable> uiDrawablesListWidget::getDrawable(const string& id)
{
	return mDrawables[id];
}

void uiDrawablesListWidget::removeDrawable(const string& id)
{

}

void uiDrawablesListWidget::removeAllDrawables()
{
	FOREACH(DrawablesMap, mDrawables, drw)
		safe_release(drw->second);
}

void uiDrawablesListWidget::localDraw()
{	
	FOREACH(DrawablesMap, mDrawables, drw)
		drw->second->draw();
}

CLOSE_O2_NAMESPACE