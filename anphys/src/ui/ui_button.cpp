#include "ui_button.h"

#include "util/other/callback.h"

REGIST_TYPE(uiButton)

uiButton::uiButton(uiWidgetsManager* widgetsManager, const std::string& id, cCallbackInterface* callback /*= NULL*/):
	uiWidget(widgetsManager, id), mOnClickCallback(callback), mPressed(false), mSelected(false)
{
}

uiButton::uiButton( const uiButton& button ):
	uiWidget(button), mPressed(false), mSelected(false)
{
	mOnClickCallback = button.mOnClickCallback;
}

uiButton::~uiButton()
{
	safe_release(mOnClickCallback);
}

void uiButton::setCallback( cCallbackInterface* callback )
{
	safe_release(mOnClickCallback);
	mOnClickCallback = callback;
}

int uiButton::processInputMessageDerived( const cInputMessage& message )
{
	int res = 0;

	if (!mPressed && !message.isKeyDown(CURSOR_BUTTON))
	{
		if (mSelected && !isPointInside(message.mCursorPosition))
		{
			mSelected = false;
			setState("visible", false, true);
		}
		if (!mSelected && isPointInside(message.mCursorPosition))
		{
			mSelected = true;
			setState("selected", false, true);
			res = 1;
		}
	}
	if (message.isKeyPressed(CURSOR_BUTTON) && isPointInside(message.mCursorPosition))
	{
		mPressed = true;
		mSelected = false;
		setState("pressed", false, true);
		res = 1;
	}
	if (message.isKeyReleased(CURSOR_BUTTON) && mPressed)
	{
		gLog->fout(1, "12\n");
		if (isPointInside(message.mCursorPosition))
		{
			if (mOnClickCallback)
				mOnClickCallback->call();

			res = 1;
		}

		setState("visible", false, true);
		mPressed = false;
		mSelected = false;
	}

	return res;
}
