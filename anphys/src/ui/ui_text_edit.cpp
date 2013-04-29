#include "ui_text_edit.h"

#include "ui_font.h"
#include "ui_state.h"
#include "ui_manager.h"
#include "ui_label.h"

REGIST_TYPE(uiTextEdit)

uiTextEdit::uiTextEdit(uiWidgetsManager* widgetsManager, const std::string& id, uiLabel* label, uiWidget* backWidget):
	uiWidget(widgetsManager, id), mLabel(label), mBackWidget(backWidget)
{
	addChild(mBackWidget);
	addChild(mLabel);
	mCaretPos = 0;

	mSelectedState = new uiState(this, "selected");
	mFocusedState = new uiState(this, "focused");
	mSelected = false;
	mPressed = false;
}

uiTextEdit::uiTextEdit( const uiTextEdit& textWidget ):
	uiWidget(textWidget)
{
	mSelectedState = new uiState(*textWidget.mSelectedState);
	mFocusedState = new uiState(*textWidget.mFocusedState);
	mCaretPos      = 0;
	mSelected      = false;
	mPressed = false;
}

uiTextEdit::~uiTextEdit()
{
	safe_release(mSelectedState);
	safe_release(mFocusedState);
}

void uiTextEdit::derivedUpdate( float dt )
{
	mLabel->setSize(mResSize);
}

void uiTextEdit::derivedDraw()
{
}

int uiTextEdit::processInputMessageDerived( const cInputMessage& message )
{
	int res = 0;
	if (!mVisible) return res;

	if (!mPressed && !message.isKeyDown(CURSOR_BUTTON))
	{
		if (mSelected && !isPointInside(message.mCursorPosition))
		{
			mSelected = false;
			mSelectedState->deactivate();
		}
		if (!mSelected && isPointInside(message.mCursorPosition))
		{
			mSelected = true;
			mSelectedState->activate();
			res = 1;
		}
	}
	if (message.isKeyPressed(CURSOR_BUTTON) && isPointInside(message.mCursorPosition))
	{
		mPressed = true;
		mSelected = false;
		mSelectedState->deactivate();

		mWidgetsManager->setWidgetFocused(this);

		res = 1;
	}

	if (message.isKeyReleased(CURSOR_BUTTON) && !isPointInside(message.mCursorPosition))
	{
		mPressed = false;

		mWidgetsManager->unfocusWidget(this);
	}

	if (mFocused)
	{
		std::string text = mLabel->getText();

		for (cInputMessage::IntArray::const_iterator it = message.mPressedKeys.cbegin(); 
			it != message.mPressedKeys.cend(); ++it)
		{
			if (!message.isCharacter(*it))
				continue;

			text = text + (char)(*it);

			res = 1;
		}

		setText(text);
	}

	if (mFocused)
		res = 1;

	return res;
}

uiTextEdit& uiTextEdit::setText( const std::string& text )
{
	mLabel->setText(text);

	return *this;
}

std::string& uiTextEdit::getText()
{
	return mLabel->getText();
}

uiTextEdit* uiTextEdit::clone()
{
	return new uiTextEdit(*this);
}

void uiTextEdit::onFocused()
{
	mFocusedState->activate();
}

void uiTextEdit::onUnfocused()
{
	mFocusedState->deactivate();
}
