#include "ui_controller.h"

#include "app/application.h"
#include "render_system/render_system.h"

OPEN_O2_NAMESPACE

DECLARE_SINGLETON(uiController);

uiController::uiController():
	mFocusWidget(NULL), mBasicWidget(uiWidgetLayout(), "UIController")
{
}

uiController::~uiController()
{
}

void uiController::update(float dt)
{
	mBasicWidget.size = renderSystem()->getResolution();
	mBasicWidget.update(dt);
	mBasicWidget.processInputMessage(*appInput());
}

void uiController::draw()
{
	mBasicWidget.draw();
}

shared<uiWidget> uiController::addWidget(const shared<uiWidget>& widget)
{
	mBasicWidget.addChild(widget);
	return widget;
}

bool uiController::removeWidget(const shared<uiWidget>& widget)
{
	mBasicWidget.removeChild(widget);
	return true;
}

bool uiController::removeAllWidgets()
{
	mBasicWidget.removeAllChilds();
	return true;
}

shared<uiWidget> uiController::getWidget(const string& idPath)
{
	return mBasicWidget.getWidget(idPath);
}

void uiController::focusOnWidget(const shared<uiWidget>& widget)
{
	if (mFocusWidget)
		mFocusWidget->onFocusLost();

	mFocusWidget = widget;

	if (mFocusWidget)
	{
		mFocusWidget->onFocusLost();
		mBasicWidget.mChildWidgets.erase( FIND(mBasicWidget.mChildWidgets, mFocusWidget) );
		mBasicWidget.mChildWidgets.push_back(mFocusWidget);
	}
}

CLOSE_O2_NAMESPACE