#include "ui_button.h"

#include "util/graphics/stretch_rect.h"
#include "util/string.h"
#include "util/time_utils.h"
#include "app/application.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(uiButton);

uiButton::uiButton(const uiWidgetLayout& layout, const string& id /*= ""*/, shared<uiWidget> parent /*= NULL*/):
	uiWidget(layout, id, parent), mHoverState(NULL), mFocusedState(NULL), mPressedState(NULL), mPressed(false),
	mPressedByButton(false)
{
}

uiButton::uiButton(const uiButton& button):
	uiWidget(button), mPressed(false), mPressedByButton(false)
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
	{
		(*rt)->draw();
		//(*rt)->drawDebug();
	}
}

void uiButton::localUpdate(float dt)
{
	if (mHoverState)
		mHoverState->setState(mCursorInside);

	if (mPressedState)
		mPressedState->setState((mCursorInside || mPressedByButton) && mPressed);

	if (mFocusedState)
		mFocusedState->setState(mFocused && !mPressed);
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
	hlog("button processing %.i cursorPos %i %i cursor %i", 
		timeUtils()->getCurrentFrame(), (int)(appInput()->getCursorPos()).x, (int)(appInput()->getCursorPos()).y,
		(int)mPressedByButton);

	bool pressedCursor = msg.isCursorPressed() && mCursorInside;
	bool pressedKey = mFocused && (msg.isKeyPressed(VK_SPACE) || msg.isKeyPressed(VK_RETURN));

	if (pressedCursor || pressedKey)
	{
		mPressed = true;
		mPressedByButton = pressedKey;

		makeFocused();
	}
	else 
	{
		bool releasedCursor = msg.isCursorReleased() && mFocused;
		bool releasedKey = mFocused && (msg.isKeyReleased(VK_SPACE) || msg.isKeyReleased(VK_RETURN));

		if (releasedCursor || releasedKey)
		{
			mPressed = false;

			if (mCursorInside || releasedKey)
			{
				onClickEvent.call();
				hlog("CLICK!");
				releaseFocus();
			}
		}
	}

	return false;
}

void uiButton::onFocused()
{
	uiWidget::onFocused();
}

void uiButton::onFocusLost()
{
	uiWidget::onFocusLost();

	if (mPressedState)
		mPressedState->setState(false);
}

int uiButton::addDrawable(const shared<cStretchRect>& drawable)
{
	mDrawables.push_back(drawable);
	int idx = mDrawables.size() - 1;
	registProperty(drawable->transparency, format("drawable_%i_transparency", idx));
	return idx;
}

CLOSE_O2_NAMESPACE
