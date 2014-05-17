#include "ui_controller.h"

#include "app/application.h"

OPEN_O2_NAMESPACE

uiController::uiController():
	mFocusWidget(NULL)
{
}

uiController::~uiController()
{
	removeAllWidgets();
}

void uiController::update(float dt)
{
	FOREACH(WidgetsVec, mWidgets, widget)
		(*widget)->update(dt);

	FOREACH_BACK(WidgetsVec, mWidgets, widget)
		if ((*widget)->processInputMessage(*appInput()))
			return;
}

void uiController::draw()
{
	FOREACH(WidgetsVec, mWidgets, widget)
		(*widget)->draw();
}

shared(uiWidget) uiController::addWidget(const shared(uiWidget)& widget)
{
	mWidgets.push_back(widget);
	return widget;
}

bool uiController::removeWidget(const shared(uiWidget)& widget)
{

}

bool uiController::removeAllWidgets()
{

}

shared(uiWidget) uiController::getWidget(const string& idPath)
{

}

void uiController::focusOnWidget(const shared(uiWidget)& widget)
{
	if (mFocusWidget)
		mFocusWidget->onFocusLost();

	mFocusWidget = widget;

	if (mFocusWidget)
	{
		mFocusWidget->onFocusLost();
		mWidgets.erase( FIND(mWidgets, mFocusWidget) );
		mWidgets.push_back(mFocusWidget);
	}
}

CLOSE_O2_NAMESPACE