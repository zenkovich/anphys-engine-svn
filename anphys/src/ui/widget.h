#ifndef WIDGET_H
#define WIDGET_H

#include <string>
#include <vector>

#include "util/math/mmath.h"
#include "util/other/type_intexation.h"
#include "util/serialization/serialization.h"

#include "input/input_message.h"

struct uiWidgetsManager;
struct uiState;

struct uiWidget
{
	typedef std::vector<uiWidget*> WidgetsList;
	typedef std::vector<uiState*> StatesList;

	DEFINE_TYPE(uiWidget)

	uiWidgetsManager* mWidgetsManager;

	std::string       mId;

	uiWidget*         mParent;
	WidgetsList       mChilds;

	vec2              mPosition;
	vec2              mGlobalPosition;
	vec2              mOffset;
	vec2              mSize;
	float             mTransparency;
	float             mResTransparency;
	bool              mModal;

	StatesList        mStates;
	uiState*          mCurrentState;

//functions
	uiWidget(uiWidgetsManager* widgetsManager, const std::string& id = "noName", uiWidget* parent = NULL);
	uiWidget(const uiWidget& widget);
	virtual ~uiWidget();

	void addChild(uiWidget* widget);

	void removeChild(uiWidget* widget);
	void removeAllChilds();

	uiWidget* getWidget(const std::string& id) const;

	template<typename T>
	T* getWidgetByType(const std::string& id) const
	{
		return static_cast<T*>(getWidget(id));
	}

	void addState(uiState* state);
	void removeState(uiState* state);
	void removeAllStates();

	uiState* getState(const std::string& id);

	void setState(const std::string& id, bool forcible = false);

	void show(bool forcible = false);
	void hide(bool forcible = false);

	virtual void update(float dt);
	virtual void derivedUpdate(float dt) {}
	virtual void draw();

	virtual int processInputMessage(const cInputMessage& message) { return 0; }

	virtual uiWidget* clone() const;

	void createStdStates();

	serializeMethodDecl(uiWidget);
};

#endif //WIDGET_H