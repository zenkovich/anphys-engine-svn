#include "ui_widget.h"

#include "ui_controller.h"
#include "util\time_utils.h"
#include "render_system\render_system.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(uiWidget);

uiWidget::uiWidget( const cLayout& layout, const string& id/* = ""*/ ):
	mId(id), mLayout(layout), mGeometry(NULL), mVisible(true), mFocused(false), mBasicTransparency(1.0f), mParent(NULL),
	mVisibleState(NULL), mUpdatedAtFrame(0), mProcessedInputAtFrame(0), mDrawedAtFrame(0), mCursorInside(false),
	mChildsLayout(cLayout::both())
{
	mLayout = layout;
	mCheckParentTransparency = callback<uiWidget>(this, &uiWidget::updateResTransparency);
	initializeProperties();
	updateLayout();
}

uiWidget::uiWidget( const uiWidget& widget ):
	mUpdatedAtFrame(0), mProcessedInputAtFrame(0), mDrawedAtFrame(0), mCursorInside(false), mVisibleState(NULL)
{
	initializeProperties();
	
	mCheckParentTransparency = callback<uiWidget>(this, &uiWidget::updateResTransparency);

	mId = widget.mId;
	mLayout = widget.mLayout;
	mChildsLayout = widget.mChildsLayout;
	mParent = NULL;
	mChildsOffset = widget.mChildsOffset;
	mVisible = widget.mVisible;
	mFocused = false;
	mBasicTransparency = widget.mBasicTransparency;

	FOREACH_CONST(WidgetsVec, widget.mChildWidgets, it)
		addChild((*it)->clone());

	FOREACH_CONST(StatesMap, widget.mStates, state)
		addState(state->second->clone());

	updateLayout();
}

uiWidget::~uiWidget()
{
	removeAllChilds();

	FOREACH(StatesMap, mStates, state)
		safe_release(state->second);

	safe_release(mCheckParentTransparency);
}

void uiWidget::draw()
{
	if (!mVisible || mDrawedAtFrame == timeUtils()->getCurrentFrame())
		return;

	mDrawedAtFrame = timeUtils()->getCurrentFrame();

	localDraw();
	if (UI_DEBUG_FRAMES)
		drawDebugFrame();

	FOREACH(WidgetsVec, mChildWidgets, it)
		(*it)->draw();
}

void uiWidget::update( float dt )
{
	if (!mVisible || mUpdatedAtFrame == timeUtils()->getCurrentFrame())
		return;

	mUpdatedAtFrame = timeUtils()->getCurrentFrame();

	updateStates(dt);
	localUpdate(dt);

	FOREACH(WidgetsVec, mChildWidgets, it)
		(*it)->update(dt);
	
	mCursorInside = false;
}

void uiWidget::updateStates(float dt)
{
	FOREACH(StatesMap, mStates, state)
		(*state).second->update(dt);
}

void uiWidget::updateLayout()
{
	localUpdateLayout();
	
	FOREACH(WidgetsVec, mChildWidgets, it)
		(*it)->updateLayout();
}

void uiWidget::localUpdateLayout()
{
	vec2f parPos, parSize;
	if (mParent)
	{
		parPos = mParent->mChildsLayout.mPosition;
		parSize = mParent->mChildsLayout.mSize;
	}

	mLayout.update(parPos, parSize);
	mGlobalPosition = mLayout.mPosition;
	mSize = mLayout.mSize;
	mBounds.set(mGlobalPosition, mGlobalPosition + mSize);

	mChildsLayout.update(mLayout.mPosition, mLayout.mSize);

	layoutUpdated();
}

bool uiWidget::isInside( const vec2f& point ) const
{
	if (!mBounds.isInside(point))
		return false;

	if (!isLocalInside(point))
		return false;

	return true;
}

bool uiWidget::processInputMessage( const cInputMessage& msg )
{
	if (!mVisible || mProcessedInputAtFrame == timeUtils()->getCurrentFrame())
		return false;

	mProcessedInputAtFrame = timeUtils()->getCurrentFrame();
	mCursorInside = isInside(msg.getCursorPos());

	if (!mFocused && !mCursorInside)
		return false;

	if (localProcessInputMessage(msg))
		return true;

	FOREACH_BACK(WidgetsVec, mChildWidgets, it)
		if ((*it)->processInputMessage(msg))
			return true;

	return false;
}

uiWidget* uiWidget::clone() const
{
	return mnew uiWidget(*this);
}

uiWidget* uiWidget::addChild( uiWidget* widget, int position /*= -1*/ )
{	
	if (widget->mParent)
		widget->mParent->removeChild(widget, false);

	widget->mParent = this;

	if (position < 0)
		mChildWidgets.push_back(widget);
	else
		mChildWidgets.insert(mChildWidgets.begin() + position, widget);

	transparency.onChangeEvent.add(widget->mCheckParentTransparency);

	updateLayout();

	return widget;
}

void uiWidget::removeChild( uiWidget* widget, bool release /*= true*/ )
{
	WidgetsVec::iterator fnd = FIND(mChildWidgets, widget);
	if (fnd != mChildWidgets.end())
		mChildWidgets.erase(fnd);

	transparency.onChangeEvent.remove(widget->mCheckParentTransparency, false);

	widget->mParent = NULL;

	if (release)
		safe_release(widget);
}

void uiWidget::removeAllChilds()
{
	FOREACH(WidgetsVec, mChildWidgets, it)
	{
		(*it)->mParent = NULL;
		transparency.onChangeEvent.remove((*it)->mCheckParentTransparency, false);
		safe_release(*it);
	}

	mChildWidgets.clear();
}

void uiWidget::setParent(uiWidget* parent)
{
	if (mParent)
		mParent->removeChild(this, false);
	
	if (parent)
		parent->addChild(this);
	else 
		mParent = NULL;

	updateLayout();
}

uiWidget* uiWidget::getParent()
{
	return mParent;
}

uiWidget* uiWidget::getWidget( const string& id )
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

string uiWidget::getWidgetPath( uiWidget* widget ) const
{
	string path = widget->mId;
	uiWidget* parent = widget->mParent;
	while(parent)
	{
		path = parent->mId + "/" + path;
		parent = parent->mParent;
	}

	return path;
}

void uiWidget::setPosition( const vec2f& position )
{
	mLayout.mLTAbsolute = position;
	updateLayout();
}

vec2f uiWidget::getPosition() const
{
	return mLayout.mLTAbsolute;
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
	mLayout.mLTAbsolute = position;

	if (mParent)
		mLayout.mLTAbsolute -= mParent->mGlobalPosition;

	updateLayout();
}

vec2f uiWidget::getGlobalPosition() const
{
	return mGlobalPosition;
}

void uiWidget::setSize( const vec2f& size )
{
	if (equals(size, mLayout.mRBAbsolute))
		return;

	mLayout.mRBAbsolute = size;
	mLayout.mMinSize = size;
	updateLayout();
}

vec2f uiWidget::getSize() const
{
	return mSize;
}

void uiWidget::setGeometry(cGeometry* geometry)
{
	safe_release(mGeometry);
	mGeometry = geometry;
}

cGeometry* uiWidget::getGeometry() const
{
	return mGeometry;
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
		uiHost()->focusOnWidget( this );
	else
		uiHost()->focusOnWidget(NULL);
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

uiState* uiWidget::addState(uiState* state)
{
	mStates[state->mName] = state;
	state->setOwnerWidget( this );

	if (state->mName == "visible")
	{
		mVisibleState = state;
		state->onActiveStateEvent.add( &onVisibleOn );
		state->onDeactiveStateEvent.add( &onVisibleOff );
		state->setState(mVisible, true);
	}

	addedState(state);

	return state;
}

void uiWidget::setState(const string& stateId, bool value)
{
	uiState* state = getState(stateId);
	if (state)
		state->setState(value);
}

uiState* uiWidget::getState(const string& stateId)
{
	StatesMap::iterator fnd = mStates.find(stateId);
	if (fnd == mStates.end())
		return NULL;

	return mStates[stateId];
}

void uiWidget::setVisible(bool visible)
{
	if (mVisibleState)
	{
		mVisibleState->setState(visible);
	}
	else
	{
		mVisible = visible;	
		if (mVisible)
			onVisibleOn.call();
		else
			onVisibleOff.call();
	}
}

void uiWidget::setVisibleParam(bool param)
{
	mVisible = param;
}

void uiWidget::setLayout(const cLayout& layout)
{
	mLayout = layout;
	updateLayout();
}

cLayout uiWidget::getLayout() const
{
	return mLayout;
}

void uiWidget::setChildsLayout(const cLayout& layout)
{
	mChildsLayout = layout;
	updateLayout();
}

cLayout uiWidget::getChildsLayout() const
{
	return mChildsLayout;
}

void uiWidget::setTransparency( float transparency )
{
	mBasicTransparency = transparency;
	updateResTransparency();
}

float uiWidget::getTransparency() const
{
	return mBasicTransparency;
}

bool uiWidget::isVisible() const
{
	if (mVisibleState)
		return mVisibleState->getState();

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

void uiWidget::updateResTransparency()
{
	if (mParent)
		mResTransparency = mParent->mResTransparency*mBasicTransparency;
	else
		mResTransparency = mBasicTransparency;

	mResTransparencyChanged.call();
}

void uiWidget::initializeProperties()
{
	REG_PROPERTY(uiWidget, position, setPosition, getPosition);
	REG_PROPERTY_NONCONST(uiWidget, parent, setParent, getParent);
	REG_PROPERTY(uiWidget, id, setId, getId);
	REG_PROPERTY(uiWidget, globalPosition, setGlobalPosition, getGlobalPosition);
	REG_PROPERTY(uiWidget, size, setSize, getSize);
	REG_PROPERTY_SETTER_NONCONST(uiWidget, visible, setVisible, isVisible);
	REG_PROPERTY(uiWidget, layout, setLayout, getLayout);
	REG_PROPERTY_SETTER_NONCONST(uiWidget, transparency, setTransparency, getTransparency);
}

int widgetDbgFrameIdx = 0;
void uiWidget::drawDebugFrame()
{
	if (mParent == NULL)
		widgetDbgFrameIdx = 0;
	renderSystem()->drawRectFrame(mGlobalPosition, mGlobalPosition + mSize, color4::someColor(widgetDbgFrameIdx++));
}

CLOSE_O2_NAMESPACE
