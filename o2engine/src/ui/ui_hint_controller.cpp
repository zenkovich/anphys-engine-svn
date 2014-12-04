#include "ui_hint_controller.h"

#include "ui_label.h"
#include "ui_controller.h"
#include "app/application.h"

OPEN_O2_NAMESPACE

uiHintController::uiHintController():
	mWidget(NULL), mLabel(NULL), mNeedShow(false)
{

}

uiHintController::~uiHintController()
{
	safe_release(mWidget);
}

void uiHintController::showAt( const vec2f& position, const string& hintString )
{
	if (!mWidget)
		return;

	mNeedShow = true;
	mNextPlacePosition = position;
	mNextPlaceHintText = hintString;
	
	if (mWidget->isVisible())
		mWidget->setVisible(false);
	else
		beginAppearing();
}

void uiHintController::hide()
{
	if (!mWidget)
		return;

	mWidget->setVisible(false);
}

void uiHintController::setupWidget( uiWidget* widget, uiLabel* label )
{
	mWidget = widget;
	mLabel = label;
	mWidget->onVisibleOff.add(callback<uiHintController>(this, &uiHintController::widgetDisappeared));
	mWidget->getState("visible")->setState(false, true);
}

void uiHintController::widgetDisappeared()
{
	if (!mNeedShow)
		return;

	beginAppearing();
}

void uiHintController::beginAppearing()
{
	if (!mWidget)
		return;

	mNeedShow = false;

	mLabel->setCText(mNextPlaceHintText);
	vec2f textRealSize = mLabel->getTextRealSize();
	mWidget->setSize(textRealSize + mLabel->getLayout().mLTAbsolute -  mLabel->getLayout().mRBAbsolute);
	
	mNextPlacePosition.x = clamp(mNextPlacePosition.x, 0.0f, uiHost()->getClientRectSize().x - mWidget->getSize().x);
	mNextPlacePosition.y = clamp(mNextPlacePosition.y, 0.0f, uiHost()->getClientRectSize().y - mWidget->getSize().y);

	mWidget->setPosition(mNextPlacePosition);
	mWidget->setVisible(true);
}

void uiHintController::update( float dt )
{
	if (!mWidget)
		return;

	mWidget->update(dt);
}

void uiHintController::draw()
{
	if (!mWidget)
		return;

	mWidget->draw();
}

CLOSE_O2_NAMESPACE