#include "ui_checkbox.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(uiCheckBox);

uiCheckBox::uiCheckBox( const layout& layout, const string& id /*= ""*/ ):
	uiButton(layout, id), mCheckedState(NULL), mChecked(false)
{
	initializeProperties();
}

uiCheckBox::uiCheckBox( const uiCheckBox& checkbox ):
	uiButton(checkbox)
{
	mCheckedState = getState("checked");
	onCheckOn = checkbox.onCheckOn;
	onCheckOff = checkbox.onCheckOff;

	setChecked(checkbox.mChecked);
}

uiCheckBox::~uiCheckBox()
{
}

uiWidget* uiCheckBox::clone() const
{
	return mnew uiCheckBox(*this);
}

void uiCheckBox::setChecked( bool checked )
{
	if (mChecked == checked)
		return;

	this->checked.onChangeEvent.call();

	mChecked = checked;
	if (mCheckedState)
		mCheckedState->setState(mChecked);
}

bool uiCheckBox::isChecked() const
{
	return mChecked;
}

void uiCheckBox::onClicked()
{
	setChecked(!mChecked);
}

void uiCheckBox::initializeProperties()
{
	REG_PROPERTY_SETTER_NONCONST(uiCheckBox, checked, setChecked, isChecked);
}

CLOSE_O2_NAMESPACE