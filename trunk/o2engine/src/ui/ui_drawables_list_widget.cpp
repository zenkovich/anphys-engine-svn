#include "ui_drawables_list_widget.h"

OPEN_O2_NAMESPACE

uiDrawablesListWidget::uiDrawablesListWidget(const cLayout& layout, const string& id /*= ""*/):
	uiWidget(layout, id), mBasicDrawable("basic", NULL, cLayout::both())
{
	addChildPropertyList(dynamic_cast<cPropertyList*>(&mBasicDrawable));
}

uiDrawablesListWidget::uiDrawablesListWidget(const uiDrawablesListWidget& widget):
	uiWidget(widget), mBasicDrawable(widget.mBasicDrawable)
{
	addChildPropertyList(dynamic_cast<cPropertyList*>(&mBasicDrawable));
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
		                                                            const cLayout& layout /*= cLayout::both()*/)
{
	return addDrawable(mnew Drawable(id, drawable, layout, &mBasicDrawable));
}

uiDrawablesListWidget::Drawable* uiDrawablesListWidget::addDrawable( Drawable* drawable )
{
	mBasicDrawable.addChildDrawable(drawable);
	addedDrawable(drawable);
	return drawable;
}

uiDrawablesListWidget::Drawable* uiDrawablesListWidget::getDrawable(const string& id)
{
	return mBasicDrawable.getChildDrawable(id);
}

void uiDrawablesListWidget::removeDrawable(const string& id, bool release /*= true*/)
{	
	Drawable* remDrw = getDrawable(id);
	if (remDrw)
		mBasicDrawable.removeChildDrawable(remDrw, release);
}

void uiDrawablesListWidget::removeAllDrawables()
{
	mBasicDrawable.removeAllChildDrawables();
}

void uiDrawablesListWidget::layoutUpdated()
{	
	mBasicDrawable.updateLayoutManual(mGlobalPosition, mSize);
}

void uiDrawablesListWidget::localDraw()
{	
	mBasicDrawable.draw();
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

uiDrawablesListWidget::Drawable& uiDrawablesListWidget::Drawable::operator=(const Drawable& drw)
{
	removeAllChildDrawables();
	safe_release(mDrawable);
	mDrawable = NULL;

	if (drw.mDrawable)
		mDrawable = drw.mDrawable->clone();
	else
		mDrawable = NULL;
	
	mName = drw.mName;
	mLayout = drw.mLayout;
	mParentDrawable = NULL;

	setPropertyListName(mName);

	if (mDrawable)
	{
		mDrawable->setPropertyListName("drawable");
		addChildPropertyList((dynamic_cast<cPropertyList*>(mDrawable)));
	}

	FOREACH_CONST(DrawablesVec, drw.mChildDrawables, drwi)
		addChildDrawable(mnew Drawable(**drwi));

	return *this;
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

void uiDrawablesListWidget::Drawable::updateLayoutManual(const vec2f& parPos, const vec2f& parSize)
{
	mLayout.update(parPos, parSize);
	layoutUpdated();
}

void uiDrawablesListWidget::Drawable::layoutUpdated()
{
	if (mDrawable)
		mDrawable->setRect(mLayout.getRect());

	FOREACH(DrawablesVec, mChildDrawables, drw)
		(*drw)->updateLayoutManual(mLayout.mPosition, mLayout.mSize);
}

void uiDrawablesListWidget::Drawable::updateLayout()
{
	updateLayoutManual(mParentDrawable->mLayout.mPosition, mParentDrawable->mLayout.mSize);
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

string uiDrawablesListWidget::Drawable::getPathId() const
{
	string res = mName;
	Drawable* parent = mParentDrawable;
	while(parent && parent->mParentDrawable) 
	{
		res = parent->mName + "/" + res;
		parent = parent->mParentDrawable;
	}

	return res;
}

CLOSE_O2_NAMESPACE