#include "ui_button.h"

#include "util/other/callback.h"
#include "ui_state.h"
#include "ui_manager.h"

REGIST_TYPE(uiButton)

uiButton::uiButton(uiWidgetsManager* widgetsManager, const std::string& id, cCallbackInterface* callback /*= NULL*/):
	uiWidget(widgetsManager, id), mOnClickCallback(callback), mPressed(false), mSelected(false)
{
	mSelectedState = new uiState(this, "");
	mPressedState = new uiState(this, "");
}

uiButton::uiButton( const uiButton& button ):
	uiWidget(button), mPressed(false), mSelected(false)
{
	mOnClickCallback = button.mOnClickCallback;
	mSelectedState = new uiState(*button.mSelectedState);
	mPressedState = new uiState(*button.mPressedState);
}

uiButton::~uiButton()
{
	safe_release(mOnClickCallback);
	safe_release(mSelectedState);
	safe_release(mPressedState);
}

void uiButton::setCallback( cCallbackInterface* callback )
{
	safe_release(mOnClickCallback);
	mOnClickCallback = callback;
}

int uiButton::processInputMessageDerived( const cInputMessage& message )
{
	int res = 0;
		
	if (!mSelected && isPointInside(message.mCursorPosition))
	{
		mSelected = true;
		mSelectedState->activate();

		res = 1;
	}

	if (message.isKeyPressed(CURSOR_BUTTON) && isPointInside(message.mCursorPosition))
	{
		mPressed = true;
		mSelected = false;
		mPressedState->activate();

		mWidgetsManager->setWidgetFocused(this);

		res = 1;
	}
	else if (message.isKeyReleased(CURSOR_BUTTON) && mPressed)
	{
		if (isPointInside(message.mCursorPosition))
		{
			if (mOnClickCallback)
				mOnClickCallback->call();
		}

		mPressed = false;
		mSelected = false;

		mWidgetsManager->unfocusWidget(this);

		mPressedState->deactivate();

		res = 0;
	}

	if (mFocused)
		res = 1;

	return res;
}

void uiButton::derivedUpdate( float dt )
{
	if (mSelected && !isPointInside(mWidgetsManager->mLastInputMessage->mCursorPosition))
	{
		mSelected = false;
		mSelectedState->deactivate();
	}
}
