#include "ui_scroll_bar.h"

OPEN_O2_NAMESPACE

uiScrollBar::uiScrollBar( const cLayout& layout, const string& id /*= ""*/, Type type /*= TP_HORISONTAL*/, 
                          uiWidget* parent /*= NULL*/ ):
	uiDrawablesListWidget(layout, id, parent), mBarHoverState(NULL), mBarPressedState(NULL), mType(type), mBar(NULL),
	mBarGeometry(cLayout::both()), mBackgrGeometry(cLayout::both())
{
	mMinValue = 0;
	mMaxValue = 1;
	setValue(mMinValue);
	setBarSize(mMaxValue);

	initializeProperties();
}

uiScrollBar::uiScrollBar( const uiScrollBar& scrollbar ):
	uiDrawablesListWidget(scrollbar)
{
	if (scrollbar.mBar)
		mBar = getDrawable(scrollbar.mBar->getPathId());
	else
		mBar = NULL;

	mBarHoverState = getState("hover");
	mBarPressedState = getState("pressed");

	mType = scrollbar.mType;
	mMinValue = scrollbar.mMinValue;
	mMaxValue = scrollbar.mMaxValue;
	mBarSize = scrollbar.mBarSize;
	mBarGeometry = scrollbar.mBarGeometry;
	mBackgrGeometry = scrollbar.mBackgrGeometry;
	setValue(scrollbar.mValue);

	initializeProperties();
}

uiScrollBar::~uiScrollBar()
{
}

uiWidget* uiScrollBar::clone() const
{
	return mnew uiScrollBar(*this);
}

void uiScrollBar::setBarDrawable(Drawable* drawable)
{
	mBar = drawable;
}

uiDrawablesListWidget::Drawable* uiScrollBar::getBarDrawable()
{
	return mBar;
}

void uiScrollBar::setValueRange( float minValue, float maxValue )
{
	mMinValue = minValue;
	mMaxValue = maxValue;
	this->minValue.onChangeEvent.call();
	this->maxValue.onChangeEvent.call();
	updateBarLayout();
}

void uiScrollBar::setMinValue( float value )
{
	mMinValue = value;
	updateBarLayout();
	this->minValue.onChangeEvent.call();
}

void uiScrollBar::setMaxValue( float value )
{
	mMaxValue = value;
	updateBarLayout();
	this->maxValue.onChangeEvent.call();
}

float uiScrollBar::getMinValue() const
{
	return mMinValue;
}

float uiScrollBar::getMaxValue() const
{
	return mMaxValue;
}

void uiScrollBar::setValue( float value )
{
	mValue = value;
	updateBarLayout();
	this->value.onChangeEvent.call();
}

float uiScrollBar::getValue() const
{
	return mValue;
}

void uiScrollBar::setBarSize( float size )
{
	mBarSize = size;
	updateBarLayout();
	this->barSize.onChangeEvent.call();
}

float uiScrollBar::getBarSize() const
{
	return mBarSize;
}

void uiScrollBar::setBarGeometryLayout( const cLayout& layout )
{
	mBarGeometry = layout;
}

void uiScrollBar::setBackgroundGeometryLayout( const cLayout& layout )
{
	mBackgrGeometry = layout;
}

void uiScrollBar::localUpdate( float dt )
{
	if (mBarHoverState)
		mBarHoverState->setState(mHover && mCursorInside);
}

bool uiScrollBar::localProcessInputMessage( const cInputMessage& msg )
{
	bool res = false;
	fRect barRect = mBarGeometry.getRect();
	vec2f cursorPos = msg.getCursorPos();
	mHover = barRect.isInside(msg.getCursorPos());

	if (msg.isCursorPressed())
	{
		if (barRect.isInside(cursorPos))
		{
			mPressed = true;

			if (mBarPressedState)
				mBarPressedState->setState(mPressed);

		}
		else
		{
			bool cursorUpper = (mType == TP_HORISONTAL) ? cursorPos.x < barRect.left:cursorPos.y < barRect.top;
			bool cursorLower = (mType == TP_HORISONTAL) ? cursorPos.x > barRect.right:cursorPos.y > barRect.down;

			if (cursorUpper)
				setValue(clamp(mValue - mBarSize, mMinValue, mMaxValue));

			if (cursorLower)
				setValue(clamp(mValue + mBarSize, mMinValue, mMaxValue));
		}
		
		if (mCursorInside)
			makeFocused();
		else
			releaseFocus();

		res = mCursorInside;
	}
	else if (msg.isCursorDown()) 
	{
		if (mPressed)
		{
			float delta = (mType == TP_HORISONTAL) ? msg.getCursorDelta().x:msg.getCursorDelta().y;
			float length = (mType == TP_HORISONTAL) ? mBasicDrawable.getLayout().getRect().getSizeX():
				                                      mBasicDrawable.getLayout().getRect().getSizeY();

			float range = mMaxValue - mMinValue;
			length = length - (mBarSize/range)*length;
			setValue(clamp(mValue + delta/length*range, mMinValue, mMaxValue));
		}
		else if (msg.getCursorDelta().length() > 1.0f)
		{
			bool cursorUpper = (mType == TP_HORISONTAL) ? cursorPos.x < barRect.left:cursorPos.y < barRect.top;
			bool cursorLower = (mType == TP_HORISONTAL) ? cursorPos.x > barRect.right:cursorPos.y > barRect.down;

			if (cursorUpper)
				setValue(clamp(mValue - mBarSize, mMinValue, mMaxValue));

			if (cursorLower)
				setValue(clamp(mValue + mBarSize, mMinValue, mMaxValue));
		}

		res = true;
	}
	else if (msg.isCursorReleased() && mPressed)
	{
		mPressed = false;

		if (mBarPressedState)
			mBarPressedState->setState(mPressed);
	}

	return res;
}

bool uiScrollBar::isLocalInside( const vec2f& point ) const
{
	return mBackgrGeometry.getRect().isInside(point);
}

void uiScrollBar::updateBarLayout()
{
	onValueChangedEvent.call();

	if (!mBar)
		return;

	float range = mMaxValue - mMinValue;
	float locRange = max(range - mBarSize, 0.00001f);
	float relBarSize = mBarSize/range;
	float topCoef = clamp((mValue - mMinValue)/range*(1.0f - relBarSize), 0.0f, 1.0f);
	float bottomCoef = clamp(topCoef + relBarSize, topCoef, 1.0f);

	if (mType == TP_HORISONTAL)
		mBar->setLayout( cLayout(vec2f(topCoef, 0.0f), vec2f(), vec2f(bottomCoef, 1.0f)) );
	else
		mBar->setLayout( cLayout(vec2f(0.0f, topCoef), vec2f(), vec2f(1.0f, bottomCoef)) );

	mBar->updateLayoutManual(mGlobalPosition, mSize);
	mBarGeometry.update(mBar->getLayout().mPosition, mBar->getLayout().mSize);
	mBackgrGeometry.update(mGlobalPosition, mSize);
}

void uiScrollBar::initializeProperties()
{
	REG_PROPERTY_SETTER_NONCONST(uiScrollBar, minValue, setMinValue, getMinValue);
	REG_PROPERTY_SETTER_NONCONST(uiScrollBar, maxValue, setMaxValue, getMaxValue);
	REG_PROPERTY_SETTER_NONCONST(uiScrollBar, value, setValue, getValue);
	REG_PROPERTY_SETTER_NONCONST(uiScrollBar, barSize, setBarSize, getBarSize);
}

CLOSE_O2_NAMESPACE