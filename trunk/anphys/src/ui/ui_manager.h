#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <string>
#include <vector>

struct uiWidget;
struct grRender;
struct cDataObject;
struct cLogStream;

struct uiWidgetsManager
{
	typedef std::vector<uiWidget*> WidgetsList;

	grRender*   mRender;
	WidgetsList mWidgets;

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

private:
	friend struct uiWidget;

	uiWidget* createWidget(cDataObject* dataObject);
};

#endif //UI_MANAGER_H