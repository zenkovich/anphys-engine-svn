#include "ui_manager.h"

#include <algorithm>

#include "render/render.h"
#include "util/serialization/data_object.h"
#include "widget.h"
#include "sprite_widget.h"
#include "util/log/log_stream_in_file.h"
#include "util/log/log_system.h"
#include "engine/engine_options.h"

uiWidgetsManager::uiWidgetsManager( grRender* render ):mRender(render)
{
	mLog = gLogSystem->addStream(new cLogStreamInFile("ui_manager.txt"), "ui manager");
	mLog->mLogLevel = INITIAL_UI_MANAGER_LOG_LEVEL;
}

uiWidgetsManager::~uiWidgetsManager()
{
	removeAllWidgets();
	gLogSystem->removeStream(mLog);
}

uiWidget* uiWidgetsManager::addWidget(uiWidget* widget)
{
	mWidgets.push_back(widget);
	widget->mWidgetsManager = this;
	widget->hide(true);
	return widget;
}

uiWidget* uiWidgetsManager::addWidget( const std::string& dataObjectFile, const std::string& dataObjectPath )
{
	cDataObject* dataObject = 
		getDataObjectsManager().loadDataObject(dataObjectFile, cDataObjectsManager::DOT_XML)->getChild(dataObjectPath);

	uiWidget* newWidget = createWidget(dataObject);
	newWidget->mWidgetsManager = this;

	mWidgets.push_back(newWidget);

	return newWidget;
}

void uiWidgetsManager::removeWidget( uiWidget* widget )
{
	WidgetsList::iterator fnd = std::find(mWidgets.begin(), mWidgets.end(), widget);

	if (fnd == mWidgets.end()) 
		return;

	safe_release(widget);

	mWidgets.erase(fnd);
}

void uiWidgetsManager::removeAllWidgets()
{
	for (WidgetsList::iterator it = mWidgets.begin(); it != mWidgets.end(); ++it)
		safe_release(*it);

	mWidgets.clear();
}

void uiWidgetsManager::update( float dt )
{
	for (WidgetsList::iterator it = mWidgets.begin(); it != mWidgets.end(); ++it)
	{
		(*it)->update(dt);
	}
}

void uiWidgetsManager::draw()
{
	for (WidgetsList::iterator it = mWidgets.begin(); it != mWidgets.end(); ++it)
	{
		(*it)->draw();
	}
}

uiWidget* uiWidgetsManager::createWidget(cDataObject* dataObject)
{
	std::string widgetType;
	if (!dataObject->getChildValue("widgetType", widgetType))
		return NULL;

	uiWidget* newWidget = NULL;

	if      (widgetType == "uiWidget")       newWidget = new uiWidget(this);
	else if (widgetType == "uiSpriteWidget") newWidget = new uiSpriteWidget(this);

	if (!newWidget)
	{
		mLog->fout(1, "ERROR: Can't create widget with type '%s'", widgetType.c_str());
		return NULL;
	}

	newWidget->serialize(*dataObject, AT_INPUT, "");

	return newWidget;
}

int uiWidgetsManager::processInputMessage( const cInputMessage& message )
{
	int res = 0;

	if (mModalWidgets.size() > 0)
	{
		uiWidget* modalWidget = mModalWidgets.back();
		res = modalWidget->processInputMessage(message);
	}
	else
	{
		for (WidgetsList::reverse_iterator it = mVisibleWidgets.rbegin(); it != mVisibleWidgets.rend(); ++it)
		{
			int widgetRes = (*it)->processInputMessage(message);
			if (widgetRes != 0)
			{
				res = widgetRes;
				break;
			}
		}
	}

	return res;
}

void uiWidgetsManager::showedWidget( uiWidget* widget )
{
	WidgetsList::iterator fnd = std::find(mWidgets.begin(), mWidgets.end(), widget);
	if (fnd == mWidgets.end())
		return;

	if (widget->isModal())
		mModalWidgets.push_back(widget);
	else
		mVisibleWidgets.push_back(widget);
}

void uiWidgetsManager::hidedWidget( uiWidget* widget )
{
	if (widget->isModal())
	{
		WidgetsList::iterator fnd = std::find(mModalWidgets.begin(), mModalWidgets.end(), widget);
		if (fnd != mModalWidgets.end())
			mModalWidgets.erase(fnd);

		mVisibleWidgets.push_back(widget);
	}
	else
	{
		WidgetsList::iterator fnd = std::find(mVisibleWidgets.begin(), mVisibleWidgets.end(), widget);
		if (fnd != mVisibleWidgets.end())
			mVisibleWidgets.erase(fnd);
	}
}
