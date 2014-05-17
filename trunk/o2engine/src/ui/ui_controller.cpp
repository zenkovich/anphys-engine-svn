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
	WidgetsVec::iterator fnd = FIND(mWidgets, widget);
	if (fnd == mWidgets.end())
		return false;

	safe_release( shared(uiWidget)(widget));
	mWidgets.erase(fnd);
	return true;
}

bool uiController::removeAllWidgets()
{
	if (mWidgets.size() == 0)
		return false;

	FOREACH(WidgetsVec, mWidgets, widget)
		safe_release(*widget);

	mWidgets.clear();
	return true;
}

shared(uiWidget) uiController::getWidget(const string& idPath)
{
	int delPos = idPath.find("/");
	string pathPart = idPath.substr(0, delPos);

	if (pathPart == "..")
		return NULL;

	FOREACH(WidgetsVec, mWidgets, it)
	{
		if ((*it)->mId == pathPart)
		{
			if (delPos == idPath.npos)
				return (*it);
			else
				return (*it)->getWidget(idPath.substr(delPos + 1));
		}
	}

	return NULL;
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