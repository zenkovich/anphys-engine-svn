#include "ui_progressbar.h"

OPEN_O2_NAMESPACE

uiProgressBar::uiProgressBar( const cLayout& layout, const string& id /*= ""*/, shared<uiWidget> parent /*= NULL*/ ):
	uiDrawablesListWidget(layout, id, parent)
{
	mBackground = addDrawable(NULL, "background", cLayout::both());
	mBar = addDrawable(NULL, "bar", cLayout::both());
	mMinValue = 0;
	mMaxValue = 1;
	setValue(mMinValue);

	initializeProperties();
}

uiProgressBar::uiProgressBar( const uiProgressBar& widget ):
	uiDrawablesListWidget(widget)
{
	mBackground = getDrawable("background");
	mBar = getDrawable("bar");

	mMinValue = widget.mMinValue;
	mMaxValue = widget.mMaxValue;
	setValue(widget.mValue);

	initializeProperties();
}

uiProgressBar::~uiProgressBar()
{
}

shared<uiWidget> uiProgressBar::clone() const
{
	return mnew uiProgressBar(*this);
}

shared<uiDrawablesListWidget::Drawable> uiProgressBar::getBackgroundDrawable()
{
	return mBackground;
}

shared<uiDrawablesListWidget::Drawable> uiProgressBar::getBarDrawable()
{
	return mBar;
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
	mBar->setLayout(cLayout(vec2f(), vec2f(), vec2f((mValue - mMinValue)/(mMaxValue - mMinValue), 1.0f)));
	mBar->updateLayout(mGlobalPosition, mSize);
}

CLOSE_O2_NAMESPACE