#include "ui_drawables_list_widget.h"

OPEN_O2_NAMESPACE

uiDrawablesListWidget::uiDrawablesListWidget(const uiWidgetLayout& layout, const string& id /*= ""*/, 
                                             shared<uiWidget> parent /*= NULL*/):
	uiWidget(layout, id, parent)
{
}

uiDrawablesListWidget::uiDrawablesListWidget(const uiDrawablesListWidget& widget):
	uiWidget(widget)
{
	FOREACH_CONST(DrawablesVec, widget.mDrawables, drw)
		addDrawable(drw->mDrawable->clone(), drw->mName, drw->mLayout);
}

uiDrawablesListWidget::~uiDrawablesListWidget()
{
	removeAllDrawables();
}

shared<uiWidget> uiDrawablesListWidget::clone() const
{
	return mnew uiDrawablesListWidget(*this);
}

shared<IRectDrawable> uiDrawablesListWidget::addDrawable(const shared<IRectDrawable>& drawable, const string& id,
	                                                     const uiWidgetLayout& layout /*= uiBothLayout()*/)
{
	mDrawables.push_back(DrawableContainer(id, drawable, layout));
	drawable->setPropertyListName(id);
	addChildPropertyList(tempShared(dynamic_cast<cPropertyList*>(drawable.mObject)));
	addedDrawable(drawable, id);
	return drawable;
}

shared<IRectDrawable> uiDrawablesListWidget::getDrawable(const string& id)
{
	FOREACH(DrawablesVec, mDrawables, drw)
		if (drw->mName == id)
			return drw->mDrawable;

	return NULL;
}

void uiDrawablesListWidget::removeDrawable(const string& id)
{	
	shared<IRectDrawable> drawable = NULL;
	DrawablesVec::iterator fnd;

	FOREACH(DrawablesVec, mDrawables, drw)
	{
		if (drw->mName == id)
		{
			fnd = drw;
			drawable = drw->mDrawable;
			break;
		}
	}

	removeChildPropertyList(tempShared(dynamic_cast<cPropertyList*>(drawable.mObject)));
	mDrawables.erase(fnd);
	safe_release(drawable);
}

void uiDrawablesListWidget::removeAllDrawables()
{
	FOREACH(DrawablesVec, mDrawables, drw)
		safe_release(drw->mDrawable);

	mDrawables.clear();
	
	removeAllChildPropertyLists();
}

void uiDrawablesListWidget::layoutUpdated()
{	
	FOREACH(DrawablesVec, mDrawables, drw)
	{
		vec2f pos, size;
		drw->mLayout.calculate(mGlobalPosition, mSize, pos, size);
		drw->mDrawable->setRect(fRect(pos, pos + size));
	}
}

void uiDrawablesListWidget::localDraw()
{	
	FOREACH(DrawablesVec, mDrawables, drw)
		drw->mDrawable->draw();
}

CLOSE_O2_NAMESPACE