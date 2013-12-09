#include "widget.h"

#include <algorithm>

OPEN_O2_NAMESPACE

REGIST_TYPE(uiWidget);

uiWidget::uiWidget( const std::string& id /*= ""*/, uiWidget* parent /*= NULL*/, const vec2f& localPos /*= vec2f()*/ ):
	mId(id), mLocalPosition(localPos), mGlobalPosition(localPos), mChildsOffset(), mParent(parent)
{
}

uiWidget::uiWidget( const uiWidget& widget )
{
	mId = widget.mId;
	mLocalPosition = widget.mLocalPosition;
	mGlobalPosition = *(widget.mLocalPosition);
	mParent = NULL;
	mChildsOffset = widget.mChildsOffset;

	for (WidgetsList::const_iterator it = widget.mChildWidgets.cbegin(); it != widget.mChildWidgets.cend(); ++it)
	{
		addChild((*it)->clone());
	}
}

uiWidget::~uiWidget()
{
	removeAllChilds();
}

void uiWidget::draw()
{
	localDraw();

	for (WidgetsList::iterator it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it)
		(*it)->draw();
}

void uiWidget::update( float dt )
{
	localUpdate(dt);

	for (WidgetsList::iterator it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it)
		(*it)->localUpdate(dt);
}

void uiWidget::processInputMessage( const cInputMessage& msg )
{
	for (WidgetsList::iterator it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it)
		(*it)->processInputMessage(msg);
}

uiWidget* uiWidget::clone() const
{
	return mnew uiWidget(*this);
}

uiWidget* uiWidget::addChild( uiWidget* widget )
{
	mChildWidgets.push_back(widget);
	widget->mParent = this;
	return widget;
}

bool uiWidget::removeChild( uiWidget* widget )
{
	WidgetsList::iterator fnd = std::find(mChildWidgets.begin(), mChildWidgets.end(), widget);
	if (fnd != mChildWidgets.end())
		mChildWidgets.erase(fnd);

	widget->mParent = NULL;
	safe_release(widget);

	return true;
}

void uiWidget::removeAllChilds()
{
	for (WidgetsList::iterator it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it)
	{
		(*it)->mParent = NULL;
		safe_release(*it);
	}

	mChildWidgets.clear();
}

void uiWidget::setParent(uiWidget* parent)
{
	if (mParent)
	{
		WidgetsList::iterator fnd = std::find(mParent->mChildWidgets.begin(), mParent->mChildWidgets.end(), this);
		if (fnd != mParent->mChildWidgets.end())
			mParent->mChildWidgets.erase(fnd);
	}

	mParent = parent;

	if (mParent)
	{
		mParent->addChild(this);
	}
}

uiWidget* uiWidget::getWidget( const std::string& id )
{
	int delPos = id.find("/");
	std::string pathPart = id.substr(0, delPos);

	if (pathPart == "..")
	{
		if (mParent)
		{
			if (delPos == id.npos)
				return mParent;
			else
				return mParent->getWidget(id.substr(delPos + 1));
		}

		return NULL;
	}

	for (WidgetsList::iterator it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it)
	{
		if (*(*it)->mId == pathPart)
		{
			if (delPos == id.npos)
				return (*it);
			else
				return (*it)->getWidget(id.substr(delPos + 1));
		}
	}

	return NULL;
}

CLOSE_O2_NAMESPACE
