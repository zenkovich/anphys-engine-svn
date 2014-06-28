#include "ui_button.h"

#include "app/application.h"
#include "render_system/text.h"
#include "util/graphics/stretch_rect.h"
#include "util/string.h"
#include "util/time_utils.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(uiButton);

uiButton::uiButton(const cLayout& layout, const string& id /*= ""*/, uiWidget* parent /*= NULL*/):
	uiDrawablesListWidget(layout, id, parent), mHoverState(NULL), mFocusedState(NULL), mPressedState(NULL), mPressed(false),
	mPressedByButton(false), mHover(false)
{
	initializeProperties();
}

uiButton::uiButton(const uiButton& button):
	uiDrawablesListWidget(button), mPressed(false), mPressedByButton(false), mHover(false)
{	
	onClickEvent = button.onClickEvent;
	onHoverEvent = button.onHoverEvent;
	onHoverOffEvent = button.onHoverOffEvent;
	onFocusedEvent = button.onFocusedEvent;
	onFocusLostEvent = button.onFocusLostEvent;

	mHoverState = getState("hover");
	mPressedState = getState("pressed");
	mFocusedState = getState("focus");

	mCaption = static_cast<grText*>(getDrawable("caption")->getDrawable());

	initializeProperties();
}

uiButton::~uiButton()
{
}

uiWidget* uiButton::clone() const
{
	return mnew uiButton(*this);
}

bool uiButton::isFocusable() const
{
	return !(mFocusedState);
}

void uiButton::addedState(uiState* state)
{
	if (state->getName() == "hover")
		mHoverState = state;
	else if (state->getName() == "focus")
		mFocusedState = state;
	else if (state->getName() == "pressed")
		mPressedState = state;
}

void uiButton::localUpdate(float dt)
{
	if (mHoverState)
		mHoverState->setState(mCursorInside);

	if (mCursorInside && !mHover)
		onHoverEvent.call();

	if (!mCursorInside && mHover)
		onHoverOffEvent.call();

	mHover = mCursorInside;

	if (mPressedState)
		mPressedState->setState((mCursorInside || mPressedByButton) && mPressed);

	if (mFocusedState)
		mFocusedState->setState(mFocused && !mPressed);
}

bool uiButton::localProcessInputMessage(const cInputMessage& msg)
{
	/*hlog("button processing %.i cursorPos %i %i cursor %i", 
		timeUtils()->getCurrentFrame(), (int)(appInput()->getCursorPos()).x, (int)(appInput()->getCursorPos()).y,
		(int)mPressedByButton);*/

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
				onClicked();
				onClickEvent.call();
				if (!mPressedByButton)
					releaseFocus();
			}
		}
	}

	return false;
}

void uiButton::onClicked()
{
}

void uiButton::onFocused()
{
	uiWidget::onFocused();
	onFocusedEvent.call();
}

void uiButton::onFocusLost()
{
	uiWidget::onFocusLost();

	if (mPressedState)
		mPressedState->setState(false);

	onFocusLostEvent.call();
}

void uiButton::addedDrawable(Drawable* drawable)
{
	if (drawable->getName() == "caption")
		mCaption = static_cast<grText*>(drawable->getDrawable());
}

void uiButton::setCCaption( const string& caption )
{
	if (mCaption)
		mCaption->setCText(caption);
}

string uiButton::getCCaption() const
{
	if (mCaption)
		return mCaption->getCText();

	return "";
}

void uiButton::setCaption( const wstring& caption )
{
	if (mCaption)
		mCaption->setText(caption);
}

wstring uiButton::getCaption() const
{
	if (mCaption)
		return mCaption->getText();

	return convString2Wide("");
}

void uiButton::initializeProperties()
{
	REG_PROPERTY(uiButton, ccaption, setCCaption, getCCaption);
	REG_PROPERTY(uiButton, caption, setCaption, getCaption);
}

CLOSE_O2_NAMESPACE
