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
	mWidget->update(dt);
}

void uiHintController::draw()
{
	mWidget->draw();
}

CLOSE_O2_NAMESPACE