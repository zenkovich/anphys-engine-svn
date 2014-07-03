#include "ui_editbox.h"

#include "render_system/text.h"
#include "render_system/render_system.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(uiEditBox);

uiEditBox::uiEditBox( grFont* font, const cLayout& layout, const string& id /*= ""*/ ):
	uiDrawablesListWidget(layout, id), mHoverState(NULL), mFocusedState(NULL), mText(NULL)
{
	mText = mnew grText(font);
	mText->setHorAlign(grText::HA_LEFT);
	mText->setVerAlign(grText::VA_CENTER);
	initializeProperties();
}

uiEditBox::uiEditBox( const uiEditBox& editbox ):
	uiDrawablesListWidget(editbox)
{
	mHoverState = getState("hover");
	mFocusedState = getState("focus");
	mText = mnew grText(*editbox.mText);
	mClippingLayout = editbox.mClippingLayout;
	mTextLayout = editbox.mTextLayout;
}

uiEditBox::~uiEditBox()
{
	safe_release(mText);
}

uiWidget* uiEditBox::clone() const
{
	return mnew uiEditBox(*this);
}

bool uiEditBox::isFocusable() const
{
	return true;
}

void uiEditBox::setCText( const string& text )
{
	mText->setCText(text);
}

string uiEditBox::getCText() const
{
	return mText->getCText();
}

void uiEditBox::setText( const wstring& text )
{
	mText->setText(text);
}

wstring uiEditBox::getText() const
{
	return mText->getText();
}

void uiEditBox::addedState( uiState* state )
{
	if (state->getName() == "hover")
		mHoverState = state;
	else if (state->getName() == "focus")
		mFocusedState = state;
}

void uiEditBox::localUpdate( float dt )
{
	if (mHoverState)
		mHoverState->setState(mCursorInside);
}

bool uiEditBox::localProcessInputMessage( const cInputMessage& msg )
{
	return false;
}

void uiEditBox::localDraw()
{
	uiDrawablesListWidget::localDraw();

	renderSystem()->setupScissorRect(mClippingLayout.getRect());
	renderSystem()->enableScissorTest();

	mText->draw();

	renderSystem()->disableScissorTest();
}

void uiEditBox::layoutUpdated()
{
	uiDrawablesListWidget::layoutUpdated();
	mClippingLayout.update(mGlobalPosition, mSize);
	mTextLayout.update(mGlobalPosition, mSize);
	mText->setRect(mTextLayout.getRect());
}

void uiEditBox::onFocused()
{
	if (mFocusedState)
		mFocusedState->setState(true);
}

void uiEditBox::onFocusLost()
{
	if (mFocusedState)
		mFocusedState->setState(false);
}

void uiEditBox::initializeProperties()
{
	REG_PROPERTY(uiEditBox, ctext, setCText, getCText);
	REG_PROPERTY(uiEditBox, text, setText, getText);
}

CLOSE_O2_NAMESPACE