#include "ui_button.h"

#include "util/graphics/stretch_rect.h"
#include "util/string.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(uiButton);

uiButton::uiButton(const uiWidgetLayout& layout, const string& id /*= ""*/, shared<uiWidget> parent /*= NULL*/):
	uiWidget(layout, id, parent), mHoverState(NULL), mFocusedState(NULL), mPressedState(NULL), mHover(false), mPressed(false)
{
}

uiButton::uiButton(const uiButton& button):
	uiWidget(button), mHover(false), mPressed(false)
{
	FOREACH_CONST(RectsVec, button.mDrawables, rt)
		addDrawable(mnew cStretchRect(**rt));
	
	onClickEvent = button.onClickEvent;
	onHoverEvent = button.onHoverEvent;
	onHoverOffEvent = button.onHoverOffEvent;
	onFocusedEvent = button.onFocusedEvent;
	onFocusLostEvent = button.onFocusLostEvent;

	FOREACH(StatesMap, mStates, state)
	{
		addedState((*state).second);
		(*state).second->setOwnerWidget( tempShared<uiButton>(this) );
	}
}

uiButton::~uiButton()
{
	FOREACH(RectsVec, mDrawables, rt)
		safe_release(*rt);
}

shared<uiWidget> uiButton::clone() const
{
	return mnew uiButton(*this);
}

bool uiButton::isFocusable() const
{
	return !(mFocusedState);
}

void uiButton::addedState(const shared<uiState>& state)
{
	if (state->getName() == "hover")
		mHoverState = state;
	else if (state->getName() == "focus")
		mFocusedState = state;
	else if (state->getName() == "pressed")
		mPressedState = state;
}

void uiButton::localDraw()
{
	FOREACH(RectsVec, mDrawables, rt)
		(*rt)->draw(false);
}

void uiButton::localUpdate(float dt)
{
	if (mHoverState)
		mHoverState->setState(mHover);

	mHover = false;
}

void uiButton::layoutUpdated()
{	
	float off = 0.0f;
	FOREACH(RectsVec, mDrawables, rt)
	{
		//off += 20.0f;
		(*rt)->setRect(mBounds + vec2f(off, off));
	}
}

bool uiButton::localProcessInputMessage(const cInputMessage& msg)
{
	mHover = true;

	if (msg.isCursorPressed())
	{
		mPressed = true;

		if (mPressedState)
			mPressedState->setState(true);
	}
	else if (msg.isCursorReleased())
	{
		mPressed = false;

		if (mPressedState)
			mPressedState->setState(false);

		if (isInside(msg.getCursorPos()))
			onClickEvent.call();
	}

	return false;
}

void uiButton::onFocused()
{
	uiWidget::onFocused();

	if (mFocusedState)
		mFocusedState->setState(true);
}

void uiButton::onFocusLost()
{
	uiWidget::onFocusLost();

	if (mFocusedState)
		mFocusedState->setState(false);
}

int uiButton::addDrawable(const shared<cStretchRect>& drawable)
{
	mDrawables.push_back(drawable);
	int idx = mDrawables.size() - 1;
	registProperty(drawable->transparency, format("drawable_%i_transparency", idx));
	return idx;
}

CLOSE_O2_NAMESPACE
