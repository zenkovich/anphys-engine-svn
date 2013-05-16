#include "ui_checkbox.h"

#include "ui_state.h"
#include "ui_widget.h"
#include "ui_manager.h"

REGIST_TYPE(uiCheckBox)

uiCheckBox::uiCheckBox( uiWidgetsManager* widgetsManager, const std::string& id, uiWidget* checkBack, uiWidget* checkThumb, 
                        uiWidget* informationWidget ):
	uiWidget(widgetsManager, id), mCheckBack(checkBack), mCheckThumb(checkThumb), mInformationWidget(informationWidget), 
	mSelected(false), mPressed(false)
{
	addChild(mCheckBack);
	addChild(mCheckThumb);
	if (mInformationWidget)
		addChild(mInformationWidget);

	mCheckThumb->hide(true);
	
	mSelectedState = new uiState(this, "s");
	mPressedState = new uiState(this, "p");

	mSelected = mPressed = false;
	mChecked = false;
}

uiCheckBox::~uiCheckBox()
{
}

void uiCheckBox::setCheck( bool flag )
{
	if (flag == mChecked)
		return;

	mChecked = flag;

	if (mChecked)
		mCheckThumb->show();
	else
		mCheckThumb->hide();

	setBindingValue(&mChecked, BindValuePrototype::t_bool);
}

bool uiCheckBox::isChecked() const
{
	return mChecked;
}

int uiCheckBox::processInputMessageDerived( const cInputMessage& message )
{
	int res = 0;

	bool isIntersectCheck = mCheckBack->isPointInside(message.mCursorPosition);

	if (!mSelected && isIntersectCheck)
	{
		mSelected = true;
		mSelectedState->activate();
		res = 1;
	}

	if (message.isKeyPressed(CURSOR_BUTTON) && !mPressed && isIntersectCheck)
	{
		mPressed = true;
		mPressedState->activate();

		mWidgetsManager->setWidgetFocused(this);

		res = 1;
	}
	else if (message.isKeyReleased(CURSOR_BUTTON) && mPressed)
	{
		mPressed = false;
		mPressedState->deactivate();
		if (isIntersectCheck)
		{
			setCheck(!mChecked);
			res = 1;
		}
		mWidgetsManager->unfocusWidget(this);
	}

	if (mFocused)
		res = 1;

	return res;
}

void uiCheckBox::derivedUpdate( float dt )
{
	if (mSelected && mWidgetsManager->mLastInputMessage && 
		!mCheckBack->isPointInside(mWidgetsManager->mLastInputMessage->mCursorPosition))
	{
		mSelected = false;
		mSelectedState->deactivate();
	}
}

void uiCheckBox::checkBindedValues()
{
	if (!mFocused)
	{
		for (BindValuesList::iterator it = mBindValues.begin(); it != mBindValues.end(); ++it)
		{
			if ((*it)->checkValue())
			{
				bool newValue;
				(*it)->getValue(&newValue, BindValuePrototype::t_bool);

				setCheck(newValue);
			}
		}	
	}
}
