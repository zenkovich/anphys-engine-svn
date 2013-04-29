#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <string>
#include <vector>

#include "input/input_listener.h"

struct uiWidget;
struct grRender;
struct cDataObject;
struct cLogStream;

struct uiWidgetsManager:public cInputListener
{
	typedef std::vector<uiWidget*> WidgetsList;

	grRender*   mRender;
	WidgetsList mWidgets;
	WidgetsList mVisibleWidgets;
	WidgetsList mModalWidgets;
	uiWidget*   mFocusWidget;

	cLogStream* mLog;

//functions
	uiWidgetsManager(grRender* render);
	~uiWidgetsManager();

	uiWidget* addWidget(uiWidget* widget);
	uiWidget* addWidget(const std::string& dataObjectFile, const std::string& dataObjectPath);
	void      removeWidget(uiWidget* widget);

	void      removeAllWidgets();

	void      update(float dt);
	void      draw();

	int processInputMessage(const cInputMessage& message);

	void setWidgetFocused(uiWidget* widget);
	void unfocusWidget(uiWidget* widget);

private:
	friend struct uiWidget;

	uiWidget* createWidget(cDataObject* dataObject);
	void showedWidget(uiWidget* widget);
	void hidedWidget(uiWidget* widget);
};

#endif //UI_MANAGER_H