#include "widget.h"

#include <algorithm>

OPEN_O2_NAMESPACE

REGIST_TYPE(uiWidget);

uiWidget::uiWidget( const std::string& id /*= ""*/, uiWidget* parent /*= NULL*/, const vec2f& localPos /*= vec2f()*/ ):
	mId(id), mLocalPosition(localPos), mGlobalPosition(localPos), mChildsOffset(), mParent(parent)
{
	initializeProperties();
	updateLayout();
}

uiWidget::uiWidget( const uiWidget& widget )
{
	initializeProperties();

	mId = widget.mId;
	mLocalPosition = widget.mLocalPosition;
	mGlobalPosition = widget.mLocalPosition;
	mParent = NULL;
	mChildsOffset = widget.mChildsOffset;

	for (WidgetsList::const_iterator it = widget.mChildWidgets.cbegin(); it != widget.mChildWidgets.cend(); ++it)
	{
		addChild((*it)->clone());
	}

	position = mGlobalPosition;
	mGlobalPosition = position;

	updateLayout();
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
		(*it)->update(dt);
}

void uiWidget::updateLayout()
{
	localUpdateLayout();
	
	for (WidgetsList::iterator it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it)
		(*it)->updateLayout();
}

void uiWidget::localUpdateLayout()
{
	vec2f parentPos, parentSize;
	if (mParent)
	{
		parentPos = mParent->mGlobalPosition;
		parentSize = mParent->mSize;
	}
	
	mSize.x = clamp(parentSize.x*mLayout.mRelSize.x - mLayout.mRelSizeBorder.x, mLayout.mMinSize.x, mLayout.mMaxSize.x);
	mSize.y = clamp(parentSize.y*mLayout.mRelSize.y - mLayout.mRelSizeBorder.y, mLayout.mMinSize.y, mLayout.mMaxSize.y);

	vec2f pivot = mSize.scale(mLayout.mRelPivot) + mLayout.mPxPivot;

	mGlobalPosition = parentPos + parentSize.scale(mLayout.mRelPosition) - pivot;

	mBounds.set(mGlobalPosition, mGlobalPosition + mSize);
}

bool uiWidget::isInside( const vec2f& point ) const
{
	if (!mBounds.isInside(point))
		return false;

	if (isLocalInside(point))
		return true;

	for (WidgetsList::const_iterator it = mChildWidgets.cbegin(); it != mChildWidgets.cend(); ++it)
		if ((*it)->isInside(point))
			return true;
}

void uiWidget::processInputMessage( const cInputMessage& msg )
{
	localProcessInputMessage(msg);

	for (WidgetsList::iterator it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it)
		(*it)->processInputMessage(msg);
}

uiWidget* uiWidget::clone() const
{
	return mnew uiWidget(*this);
}

uiWidget* uiWidget::addChild( uiWidget* widget )
{
	widget->setParent(this);
	return widget;
}

void uiWidget::removeChild( uiWidget* widget )
{
	WidgetsList::iterator fnd = std::find(mChildWidgets.begin(), mChildWidgets.end(), widget);
	if (fnd != mChildWidgets.end())
		mChildWidgets.erase(fnd);

	widget->mParent = NULL;
	safe_release(widget);

	updateLayout();
}

void uiWidget::removeAllChilds()
{
	for (WidgetsList::iterator it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it)
	{
		(*it)->mParent = NULL;
		safe_release(*it);
	}

	mChildWidgets.clear();

	updateLayout();
}

void uiWidget::setParent(const uiWidgetPtr& parent)
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

	updateLayout();
}

uiWidget* uiWidget::getParent() const
{
	return mParent;
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
		if ((*it)->mId == pathPart)
		{
			if (delPos == id.npos)
				return (*it);
			else
				return (*it)->getWidget(id.substr(delPos + 1));
		}
	}

	return NULL;
}

void uiWidget::setPosition( const vec2f& position )
{
	mLocalPosition = position;
	updateLayout();
}

vec2f uiWidget::getPosition() const
{
	return mLocalPosition;
}

void uiWidget::setId( const std::string& id )
{
	mId = id;
}

std::string uiWidget::getId() const
{
	return mId;
}

void uiWidget::setGlobalPosition( const vec2f& position )
{
	mLocalPosition = position;

	if (mParent)
		mLocalPosition -= mParent->mGlobalPosition;

	updateLayout();
}

vec2f uiWidget::getGlobalPosition() const
{
	return mGlobalPosition;
}

void uiWidget::setSize( const vec2f& size )
{
	mSize = size;
	updateLayout();
}

vec2f uiWidget::getSize() const
{
	return mSize;
}

cGeometry* uiWidget::getGeometry() const
{
	return mGeometry;
}

void uiWidget::initializeProperties()
{
	position.init(this, &uiWidget::setPosition, &uiWidget::getPosition);
	parent.init(this, &uiWidget::setParent, &uiWidget::getParent);
	id.init(this, &uiWidget::setId, &uiWidget::getId);
	globalPosition.init(this, &uiWidget::setGlobalPosition, &uiWidget::getGlobalPosition);
	size.init(this, &uiWidget::setSize, &uiWidget::getSize);
}

CLOSE_O2_NAMESPACE
