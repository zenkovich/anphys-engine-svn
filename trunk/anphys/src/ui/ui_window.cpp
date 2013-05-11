#include "ui_window.h"

#include "ui_scroll_area.h"
#include "ui_button.h"
#include "ui_state.h"
#include "ui_manager.h"
#include "util/other/callback.h"

REGIST_TYPE(uiWindow)


uiWindow::uiWindow( uiWidgetsManager* widgetsManager, const std::string& id, const vec2& size, const vec2& pos, 
                    uiWidget* head, uiWidget* content, uiWidget* backWidget, uiButton* closeButton /*= NULL*/, 
					uiLabel* windowCaption /*= NULL*/ ):
	uiWidget(widgetsManager, id), mWindowHead(head), mWindowContent(content), mBackWidget(backWidget)
{
	mHeadSelectingState = new uiState(this, "s");
	mHeadPressedState = new uiState(this, "p");
	
	uiWidget::addChild(mBackWidget);
	uiWidget::addChild(mWindowHead);
	uiWidget::addChild(mWindowContent);

	setPosition(pos);
	setSize(size);

	/*mWindowHead->setSize(vec2(size.x, mWindowHead->getSize().y));

	mWindowContent->setPosition(vec2(0, head->getSize().y));
	mWindowContent->setSize(vec2(size.x, size.y - head->getSize().y));*/

	mCloseButton = closeButton;

	if (mCloseButton)
	{
		closeButton->setCallback(new cCallback<uiWindow>(this, &uiWindow::onCloseBtn));
	}

	mWindowCaption = windowCaption;

	mHeadSelected = mHeadPressed = false;
}

uiWindow::~uiWindow()
{
}

void uiWindow::addChild( uiWidget* widget )
{
	mWindowContent->addChild(widget);
}

void uiWindow::removeChild( uiWidget* widget )
{
	mWindowContent->removeChild(widget);
}

void uiWindow::removeAllChilds()
{
	mWindowContent->removeAllChilds();
}

uiWidget* uiWindow::getWidget( const std::string& id ) const
{
	return mWindowContent->getWidget(id);
}

int uiWindow::processInputMessageDerived( const cInputMessage& message )
{
	int res = 0;

	bool headIntersecting = mWindowHead->isPointInside(message.mCursorPosition);
	if (mCloseButton && mCloseButton->isPointInside(message.mCursorPosition))
		headIntersecting = false;

	if (headIntersecting)
		res = 1;

	if (isPointInside(message.mCursorPosition))
		res = 1;

	if (!mHeadSelected && headIntersecting)
	{
		mHeadSelected = true;

		mHeadSelectingState->activate();
	}
	else if (mHeadSelected && !headIntersecting)
	{
		mHeadSelected = false;

		mHeadSelectingState->deactivate();
	}

	if (message.isKeyPressed(CURSOR_BUTTON) && !mHeadPressed && headIntersecting)
	{
		mHeadPressed = true;
		
		mHeadPressedState->activate();

		mWidgetsManager->setWidgetFocused(this);

		mLastCursorPos = message.mCursorPosition;
	}
	else if (message.isKeyReleased(CURSOR_BUTTON) && mHeadPressed)
	{
		mHeadPressed = false;

		mHeadPressedState->deactivate();

		mWidgetsManager->unfocusWidget(this);

		return 0;
	}

	if (mHeadPressed && message.mCursorPosition != mLastCursorPos)
	{
		setPosition(getPosition() + message.mCursorPosition - mLastCursorPos);

		mLastCursorPos = message.mCursorPosition;
	}

	if (mFocused)
		res = 1;

	return res;
}

void uiWindow::onCloseBtn()
{
	hide();
}
