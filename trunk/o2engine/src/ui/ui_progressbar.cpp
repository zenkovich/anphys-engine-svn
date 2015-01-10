#include "ui_progressbar.h"

OPEN_O2_NAMESPACE

uiProgressBar::uiProgressBar( const layout& layout, const string& id /*= ""*/ ):
	uiDrawablesListWidget(layout, id), mBar(NULL)
{
	mMinValue = 0;
	mMaxValue = 1;
	setValue(mMinValue);
	initializeProperties();
}

uiProgressBar::uiProgressBar( const uiProgressBar& widget ):
	uiDrawablesListWidget(widget)
{	
	if (widget.mBar)
		mBar = getDrawable(widget.mBar->getPathId());
	else
		mBar = NULL;

	mMinValue = widget.mMinValue;
	mMaxValue = widget.mMaxValue;
	setValue(widget.mValue);

	initializeProperties();
}

uiProgressBar::~uiProgressBar()
{
}

uiWidget* uiProgressBar::clone() const
{
	return mnew uiProgressBar(*this);
}

uiDrawablesListWidget::Drawable* uiProgressBar::getBarDrawable()
{
	return mBar;
}

void uiProgressBar::setBarDrawable(Drawable* barDrawable)
{
	mBar = barDrawable;
}

void uiProgressBar::setValueRange( float minValue, float maxValue )
{
	mMinValue = minValue;
	mMaxValue = maxValue;
	updateBarLayout();
}

void uiProgressBar::setMinValue( float value )
{
	mMinValue = value;
	updateBarLayout();
}

void uiProgressBar::setMaxValue( float value )
{
	mMaxValue = value;
	updateBarLayout();
}

float uiProgressBar::getMinValue() const
{
	return mMinValue;
}

float uiProgressBar::getMaxValue() const
{
	return mMaxValue;
}

void uiProgressBar::setValue( float value )
{
	if (equals(value, mValue))
		return;

	mValue = clamp(value, mMinValue, mMaxValue);
	updateBarLayout();
}

float uiProgressBar::getValue() const
{
	return mValue;
}

void uiProgressBar::localUpdate( float dt )
{
}

void uiProgressBar::initializeProperties()
{
	REG_PROPERTY_SETTER_NONCONST(uiProgressBar, minValue, setMinValue, getMinValue);
	REG_PROPERTY_SETTER_NONCONST(uiProgressBar, maxValue, setMaxValue, getMaxValue);
	REG_PROPERTY_SETTER_NONCONST(uiProgressBar, value, setValue, getValue);
}

void uiProgressBar::updateBarLayout()
{
	if (!mBar)
		return;

	mBar->setLayout(layout(vec2f(), vec2f(), vec2f((mValue - mMinValue)/(mMaxValue - mMinValue), 1.0f)));
	mBar->updateLayout();
}


CLOSE_O2_NAMESPACE