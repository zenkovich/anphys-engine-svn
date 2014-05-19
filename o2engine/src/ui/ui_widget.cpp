#include "ui_widget.h"

#include "ui_controller.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(uiWidget);

uiWidget::uiWidget( const uiWidgetLayout& layout, const string& id/* = ""*/, shared<uiWidget> parent/* = NULL*/ ):
	mId(id), mChildsOffset(), mParent(parent), mVisible(true), mFocused(false)
{
	mLayout = layout;
	initializeProperties();
	updateLayout();
}

uiWidget::uiWidget( const uiWidget& widget )
{
	initializeProperties();

	mId = widget.mId;
	mLayout = widget.mLayout;
	mParent = NULL;
	mChildsOffset = widget.mChildsOffset;
	mVisible = widget.mVisible;
	mFocused = false;

	FOREACH_CONST(WidgetsVec, widget.mChildWidgets, it)
		addChild((*it)->clone());

	updateLayout();
}

uiWidget::~uiWidget()
{
	removeAllChilds();
}

void uiWidget::draw()
{
	if (!mVisible)
		return;

	localDraw();

	FOREACH(WidgetsVec, mChildWidgets, it)
		(*it)->draw();
}

void uiWidget::update( float dt )
{
	if (!mVisible)
		return;

	localUpdate(dt);

	FOREACH(WidgetsVec, mChildWidgets, it)
		(*it)->update(dt);
}

void uiWidget::updateLayout()
{
	localUpdateLayout();
	
	FOREACH(WidgetsVec, mChildWidgets, it)
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

	mGlobalPosition = parentPos + parentSize.scale(mLayout.mRelPosition) - pivot + mLayout.mPxPosition;

	mBounds.set(mGlobalPosition, mGlobalPosition + mSize);
}

bool uiWidget::isInside( const vec2f& point ) const
{
	if (!mBounds.isInside(point))
		return false;

	if (isLocalInside(point))
		return true;

	FOREACH_CONST(WidgetsVec, mChildWidgets, it)
		if ((*it)->isInside(point))
			return true;

	return false;
}

bool uiWidget::processInputMessage( const cInputMessage& msg )
{
	if (!mVisible)
		return false;

	if (!isInside(msg.getCursorPos()))
		return false;

	if (localProcessInputMessage(msg))
		return true;

	FOREACH(WidgetsVec, mChildWidgets, it)
		if ((*it)->processInputMessage(msg))
			return true;
}

shared<uiWidget> uiWidget::clone() const
{
	return mnew uiWidget(*this);
}

shared<uiWidget> uiWidget::addChild( shared<uiWidget> widget )
{
	widget->setParent(this);
	return widget;
}

void uiWidget::removeChild( shared<uiWidget> widget )
{
	WidgetsVec::iterator fnd = FIND(mChildWidgets, widget);
	if (fnd != mChildWidgets.end())
		mChildWidgets.erase(fnd);

	widget->mParent = NULL;
	safe_release(widget);

	updateLayout();
}

void uiWidget::removeAllChilds()
{
	FOREACH(WidgetsVec, mChildWidgets, it)
	{
		(*it)->mParent = NULL;
		safe_release(*it);
	}

	mChildWidgets.clear();

	updateLayout();
}

void uiWidget::setParent(const shared<uiWidget>& parent)
{
	if (mParent)
	{
		WidgetsVec::iterator fnd = FIND(mParent->mChildWidgets, shared<uiWidget>(this).disableAutoRelease());
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

shared<uiWidget> uiWidget::getParent() const
{
	return mParent;
}

shared<uiWidget> uiWidget::getWidget( const string& id )
{
	int delPos = id.find("/");
	string pathPart = id.substr(0, delPos);

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

	FOREACH(WidgetsVec, mChildWidgets, it)
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

void uiWidget::setId( const string& id )
{
	mId = id;
}

string uiWidget::getId() const
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

shared<cGeometry> uiWidget::getGeometry() const
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

bool uiWidget::isFocusable() const
{
	return false;
}

void uiWidget::setFocused(bool focused)
{
	if (focused == mFocused)
		return;

	if (focused)
		uiHost().focusOnWidget(shared<uiWidget>(this).disableAutoRelease());
	else
		uiHost().focusOnWidget(NULL);
}

bool uiWidget::isFocused() const
{
	return mFocused;
}

void uiWidget::makeFocused()
{
	setFocused(true);
}

void uiWidget::releaseFocus()
{
	setFocused(false);
}

void uiWidget::setState(const string& stateId, bool value)
{
	shared<uiState> state = getState(stateId);
	if (state)
		state->setState(value);
}

shared<uiState> uiWidget::getState(const string& stateId)
{
	StatesMap::iterator fnd = mStates.find(stateId);
	if (fnd == mStates.end())
		return NULL;

	return mStates[stateId];
}

void uiWidget::setVisible(bool visible)
{
	if (mVisibleState)
		mVisibleState->setState(visible);
	else
		mVisible = visible;	
}

bool uiWidget::isVisible() const
{
	return mVisible;
}

void uiWidget::onFocused()
{
	mFocused = true;
}

void uiWidget::onFocusLost()
{
	mFocused = false;
}

CLOSE_O2_NAMESPACE
