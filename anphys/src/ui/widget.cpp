#include "widget.h"

#include <algorithm>

REGIST_TYPE(uiWidget)

uiWidget::uiWidget( const std::string& id /*= "noName"*/, uiWidget* parent /*= NULL*/ )
{
	mId = id;
	if (parent) parent->addChild(this);
	mTransparency = 0;
	mResTransparency = 0;
	mModal = false;
}

uiWidget::uiWidget( const uiWidget& widget )
{
	mId = widget.mId;
	mParent = NULL;
	mTransparency = widget.mTransparency;
	mResTransparency = widget.mResTransparency;
	mModal = widget.mModal;

	for (WidgetsList::const_iterator it = widget.mChilds.cbegin(); it != widget.mChilds.cend(); ++it)
		addChild((*it)->clone());
}

uiWidget::~uiWidget()
{
	removeAllChilds();
}

void uiWidget::addChild( uiWidget* widget )
{
	mChilds.push_back(widget);
	widget->mParent = this;
}

void uiWidget::removeChild( uiWidget* widget )
{
	WidgetsList::iterator fnd = std::find(mChilds.begin(), mChilds.end(), widget);

	if (fnd == mChilds.end())
		return;

	mChilds.erase(fnd);

	safe_release(widget);
}

void uiWidget::removeAllChilds()
{
	for (WidgetsList::iterator it = mChilds.begin(); it != mChilds.end(); ++it)
		safe_release(*it);

	mChilds.clear();
}

uiWidget* uiWidget::getWidget( const std::string& id ) const
{
	int slashPos = id.find('/');

	std::string searchId;
	if (slashPos == id.npos) searchId = id;
	else                     searchId = id.substr(0, slashPos);

	for (WidgetsList::const_iterator it = mChilds.cbegin(); it != mChilds.cend(); ++it)
	{
		if ((*it)->mId != searchId) continue;

		if (slashPos == id.npos) 
			return *it;
		else
			return (*it)->getWidget(id.substr(slashPos + 1));
	}

	return NULL;
}

void uiWidget::update( float dt )
{
	if (mParent) 
		mResTransparency = mParent->mResTransparency*mTransparency;
	else
		mResTransparency = mTransparency;

	for (WidgetsList::iterator it = mChilds.begin(); it != mChilds.end(); ++it)
	{
		(*it)->update(dt);
	}
}

void uiWidget::draw()
{
	for (WidgetsList::iterator it = mChilds.begin(); it != mChilds.end(); ++it)
	{
		(*it)->draw();
	}
}

uiWidget* uiWidget::clone() const
{
	return new uiWidget(*this);
}

serializeMethodImpl(uiWidget)
{
	serializeId(mId, "id");

	return true;
}
