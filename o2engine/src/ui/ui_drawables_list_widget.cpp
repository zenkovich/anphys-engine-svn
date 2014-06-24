#include "ui_drawables_list_widget.h"

OPEN_O2_NAMESPACE

uiDrawablesListWidget::uiDrawablesListWidget(const cLayout& layout, const string& id /*= ""*/, 
                                             uiWidget* parent /*= NULL*/):
	uiWidget(layout, id, parent)
{
}

uiDrawablesListWidget::uiDrawablesListWidget(const uiDrawablesListWidget& widget):
	uiWidget(widget)
{
	FOREACH_CONST(DrawablesVec, widget.mDrawables, drw) 
		addDrawable(mnew Drawable(**drw));

	FOREACH(StatesMap, mStates, state)
		(*state).second->setOwnerWidget( this );

}

uiDrawablesListWidget::~uiDrawablesListWidget()
{
	removeAllDrawables();
}

uiWidget* uiDrawablesListWidget::clone() const
{
	return mnew uiDrawablesListWidget(*this);
}

uiDrawablesListWidget::Drawable* uiDrawablesListWidget::addDrawable(IRectDrawable* drawable, 
	                                                                const string& id, 
		                                                            const cLayout& layout /*= cLayout::both()*/, 
														            Drawable* parentDrawable /*= NULL*/)
{
	return addDrawable(mnew Drawable(id, drawable, layout, parentDrawable));
}

uiDrawablesListWidget::Drawable* uiDrawablesListWidget::addDrawable( Drawable* drawable )
{
	if (drawable->mParentDrawable)
		drawable->mParentDrawable->addChildDrawable(drawable);

	mDrawables.push_back(drawable);
	addChildPropertyList((dynamic_cast<cPropertyList*>(drawable)));

	addedDrawable(drawable);

	return drawable;
}

uiDrawablesListWidget::Drawable* uiDrawablesListWidget::getDrawable(const string& id)
{
	FOREACH(DrawablesVec, mDrawables, drw)
		if ((*drw)->mName == id)
			return *drw;

	return NULL;
}

void uiDrawablesListWidget::removeDrawable(const string& id)
{	
	Drawable* drawable = NULL;
	DrawablesVec::iterator fnd;

	FOREACH(DrawablesVec, mDrawables, drw)
	{
		if ((*drw)->mName == id)
		{
			fnd = drw;
			drawable = *drw;
			break;
		}
	}

	removeChildPropertyList((dynamic_cast<cPropertyList*>(drawable)), false);
	mDrawables.erase(fnd);
	safe_release(drawable);
}

void uiDrawablesListWidget::removeAllDrawables()
{
	FOREACH(DrawablesVec, mDrawables, drw)
		safe_release(*drw);

	mDrawables.clear();
	
	removeAllChildPropertyLists();
}

void uiDrawablesListWidget::layoutUpdated()
{	
	FOREACH(DrawablesVec, mDrawables, drw)
	{
		(*drw)->updateLayout(mGlobalPosition, mSize);
	}
}

void uiDrawablesListWidget::localDraw()
{	
	FOREACH(DrawablesVec, mDrawables, drw)
		(*drw)->draw();
}


uiDrawablesListWidget::Drawable::Drawable( const string& name, IRectDrawable* drawable, 
	                                       const cLayout& layout /*= cLayout::both()*/, 
										   Drawable* parentDrawable /*= NULL*/ )
{
	mName = name;
	mDrawable = drawable;
	mLayout = layout;
	mParentDrawable = parentDrawable;

	setPropertyListName(mName);

	if (mDrawable)
	{
		mDrawable->setPropertyListName("drawable");
		addChildPropertyList((dynamic_cast<cPropertyList*>(mDrawable)));
	}

	initializeProperties();
}

uiDrawablesListWidget::Drawable::Drawable( const Drawable& drawable )
{

	if (drawable.mDrawable)
		mDrawable = drawable.mDrawable->clone();
	else
		mDrawable = NULL;
	
	mName = drawable.mName;
	mLayout = drawable.mLayout;
	mParentDrawable = NULL;

	setPropertyListName(mName);

	if (mDrawable)
	{
		mDrawable->setPropertyListName("drawable");
		addChildPropertyList((dynamic_cast<cPropertyList*>(mDrawable)));
	}

	FOREACH_CONST(DrawablesVec, drawable.mChildDrawables, drw)
		addChildDrawable(mnew Drawable(**drw));

	initializeProperties();
}

uiDrawablesListWidget::Drawable::~Drawable()
{
	removeAllChildDrawables();
}

uiDrawablesListWidget::Drawable* uiDrawablesListWidget::Drawable::addChildDrawable( Drawable* drawable )
{
	mChildDrawables.push_back(drawable);
	drawable->mParentDrawable = (this);
	addChildPropertyList((dynamic_cast<cPropertyList*>(drawable)));
	return drawable;
}

uiDrawablesListWidget::Drawable* uiDrawablesListWidget::Drawable::addChildDrawable( const string& name, 
		IRectDrawable* drawable, const cLayout& layout /*= cLayout::both()*/ )
{
	return addChildDrawable(mnew Drawable(name, drawable, layout));
}

uiDrawablesListWidget::Drawable* uiDrawablesListWidget::Drawable::getChildDrawable( const string& path )
{
	int delPos = path.find("/");
	string pathPart = path.substr(0, delPos);

	FOREACH(DrawablesVec, mChildDrawables, it)
	{
		if ((*it)->mName == pathPart)
		{
			if (delPos == path.npos)
				return (*it);
			else
				return (*it)->getChildDrawable(path.substr(delPos + 1));
		}
	}

	return NULL;
}

void uiDrawablesListWidget::Drawable::removeChildDrawable( Drawable* drawable, bool release /*= true*/ )
{
	DrawablesVec::iterator fnd = FIND(mChildDrawables, drawable);
	if (fnd == mChildDrawables.end())
		return;
	
	removeChildPropertyList((dynamic_cast<cPropertyList*>(drawable)), false);
	if (release)
		safe_release(drawable);

	mChildDrawables.erase(fnd);
}

void uiDrawablesListWidget::Drawable::removeAllChildDrawables()
{
	FOREACH(DrawablesVec, mChildDrawables, drw)
		safe_release(*drw);

	removeAllChildPropertyLists();

	mChildDrawables.clear();
}

void uiDrawablesListWidget::Drawable::updateLayout(const vec2f& parPos, const vec2f& parSize)
{
	mLayout.update(parPos, parSize);

	if (mDrawable)
		mDrawable->setRect(mLayout.getRect());

	FOREACH(DrawablesVec, mChildDrawables, drw)
		(*drw)->updateLayout(mLayout.mPosition, mLayout.mSize);
}

string uiDrawablesListWidget::Drawable::getName() const
{
	return mName;
}

void uiDrawablesListWidget::Drawable::setDrawable( IRectDrawable* drawable )
{
	if (mDrawable)
	{
		removeChildPropertyList((dynamic_cast<cPropertyList*>(mDrawable)));
		safe_release(mDrawable);
	}
	
	mDrawable = drawable;

	if (mDrawable)
	{
		mDrawable->setPropertyListName("drawable");
		addChildPropertyList((dynamic_cast<cPropertyList*>(mDrawable)));
	}
}

IRectDrawable* uiDrawablesListWidget::Drawable::getDrawable()
{
	return mDrawable;
}

void uiDrawablesListWidget::Drawable::setLayout( const cLayout& layout )
{
	mLayout = layout;
}

cLayout uiDrawablesListWidget::Drawable::getLayout() const
{
	return mLayout;
}

void uiDrawablesListWidget::Drawable::initializeProperties()
{
	REG_PROPERTY(Drawable, layout, setLayout, getLayout);
}

void uiDrawablesListWidget::Drawable::draw()
{
	if (mDrawable)
		mDrawable->draw();

	FOREACH(DrawablesVec, mChildDrawables, drw)
		(*drw)->draw();
}

CLOSE_O2_NAMESPACE