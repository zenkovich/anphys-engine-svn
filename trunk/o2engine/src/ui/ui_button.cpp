#include "ui_button.h"

#include "util/graphics/stretch_rect.h"

OPEN_O2_NAMESPACE

uiButton::uiButton(const uiWidgetLayout& layout, const shared<cStretchRect>& regularDrawable, 
                   const shared<cStretchRect>& hoverDrawable /*= NULL*/, const shared<cStretchRect>& focusedDrawable /*= NULL*/, 
				   const shared<cStretchRect>& pressedDrawable /*= NULL*/, 
				   const string& id /*= ""*/, shared<uiWidget> parent /*= NULL*/):
	uiWidget(layout, id, parent), mRegularDrawable(regularDrawable), mFocusedDrawable(focusedDrawable), 
	mHoverDrawable(hoverDrawable), mPressedDrawable(pressedDrawable), mHoverState(NULL), mFocusedState(NULL),
	mPressedState(NULL)
{

}

uiButton::uiButton(const uiButton& button):
	uiWidget(button)
{
	mRegularDrawable = mnew cStretchRect(*(button.mRegularDrawable));

	if (button.mFocusedDrawable)
		mFocusedDrawable = mnew cStretchRect(*(button.mFocusedDrawable));
	else
		mFocusedDrawable = NULL;

	
	if (button.mHoverDrawable)
		mHoverDrawable = mnew cStretchRect(*(button.mHoverDrawable));
	else
		mHoverDrawable = NULL;
	
	if (button.mPressedDrawable)
		mPressedDrawable = mnew cStretchRect(*(button.mPressedDrawable));
	else
		mPressedDrawable = NULL;
	
	onClickEvent = button.onClickEvent;
	onHoverEvent = button.onHoverEvent;
	onHoverOffEvent = button.onHoverOffEvent;
	onFocusedEvent = button.onFocusedEvent;
	onFocusLostEvent = button.onFocusLostEvent;
}

uiButton::~uiButton()
{
	safe_release(mRegularDrawable);
	safe_release(mFocusedDrawable);
	safe_release(mHoverDrawable);
	safe_release(mPressedDrawable);
}

shared<uiWidget> uiButton::clone() const
{
	return mnew uiWidget(*this);
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
	mRegularDrawable->draw();
	
	if (mFocusedDrawable)
		mFocusedDrawable->draw();

	if (mHoverDrawable)
		mHoverDrawable->draw();

	if (mPressedDrawable)
		mPressedDrawable->draw();
}

void uiButton::localUpdate(float dt)
{
}

void uiButton::layoutUpdated()
{
	mRegularDrawable->setRect(mBounds);
	
	if (mFocusedDrawable)
		mFocusedDrawable->setRect(mBounds);

	if (mHoverDrawable)
		mHoverDrawable->setRect(mBounds);

	if (mPressedDrawable)
		mPressedDrawable->setRect(mBounds);
}

bool uiButton::localProcessInputMessage(const cInputMessage& msg)
{
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

CLOSE_O2_NAMESPACE
