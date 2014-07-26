#include "ui_controller.h"

#include "app/application.h"
#include "render_system/render_system.h"

OPEN_O2_NAMESPACE

DECLARE_SINGLETON(uiController);

uiController::uiController():
	mFocusWidget(NULL), mBasicWidget(cLayout::both(), "UIController"), mChangedFocusWidget(false), mHintWidget(NULL),
	mHintWidgetLabel(NULL)
{
}

uiController::~uiController()
{
	safe_release(mHintWidget);
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

	if (mFocusWidget)
		mFocusWidget->processInputMessage(*appInput());

	mBasicWidget.processInputMessage(*appInput());
}

void uiController::draw()
{
	mBasicWidget.draw();
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

uiLabel* uiController::FindLabel(uiWidget* widget) 
{
	if (widget->getType() == uiLabel::getStaticType())
		return static_cast<uiLabel*>(widget);

	FOREACH(uiWidget::WidgetsVec, widget->mChildWidgets, child)
	{
		uiLabel* res = FindLabel(*child);
		if (res)
			return res;
	}

	return NULL;
}

void uiController::setHintWidget(uiWidget* hintWidget, const string& labelWidgetPath /*= ""*/)
{
	mHintWidget = hintWidget;

	if (labelWidgetPath != "")
		mHintWidgetLabel = mHintWidget->getWidgetByType<uiLabel>(labelWidgetPath);
	else
		mHintWidgetLabel = FindLabel(hintWidget);
}

CLOSE_O2_NAMESPACE