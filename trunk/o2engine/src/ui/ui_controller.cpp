#include "ui_controller.h"

#include "app/application.h"
#include "render_system/render_system.h"

OPEN_O2_NAMESPACE

uiController::uiController():
	mFocusWidget(NULL), mBasicWidget(layout::both(), "UIController"), mChangedFocusWidget(false)
{
}

uiController::~uiController()
{
}

void uiController::update(float dt)
{
	if (mChangedFocusWidget)
	{
		mFocusWidget->onFocused();
		/*mBasicWidget.mChildWidgets.erase( FIND(mBasicWidget.mChildWidgets, mFocusWidget) );
		mBasicWidget.mChildWidgets.push_back(mFocusWidget);*/

		mChangedFocusWidget = false;
	}

	mBasicWidget.size = renderSystem()->getResolution();
	mBasicWidget.update(dt);
	mHintController.update(dt);

	if (mFocusWidget)
		mFocusWidget->processInputMessage(*appInput());

	mBasicWidget.processInputMessage(*appInput());
}

void uiController::draw()
{
	mBasicWidget.draw();
	mHintController.draw();
}

uiWidget* uiController::addWidget(uiWidget* widget)
{
	mBasicWidget.addChild(widget);
	return widget;
}

bool uiController::removeWidget(uiWidget* widget)
{
	mBasicWidget.removeChild(widget);
	return true;
}

bool uiController::removeAllWidgets()
{
	mBasicWidget.removeAllChilds();
	return true;
}

uiWidget* uiController::getWidget(const string& idPath)
{
	return mBasicWidget.getWidget(idPath);
}

void uiController::focusOnWidget(uiWidget* widget)
{
	if (mFocusWidget)
		mFocusWidget->onFocusLost();

	mFocusWidget = widget;

	if (mFocusWidget)
		mChangedFocusWidget = true;
}

void uiController::showHint( const string& hintText, const vec2f& position )
{
	mHintController.showAt(position, hintText);
}

void uiController::hideHint()
{
	mHintController.hide();
}

vec2f uiController::getClientRectSize() const
{
	return mBasicWidget.getSize();
}

CLOSE_O2_NAMESPACE