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
struct uiProperty;
struct grStencilBufferRenderTarget;

struct uiWidget
{
	friend struct uiSimpleStuff;
	friend struct uiState;

	typedef std::vector<uiWidget*> WidgetsList;
	typedef std::vector<uiProperty*> PropertyList;

	DEFINE_TYPE(uiWidget)

	uiWidgetsManager* mWidgetsManager;

	std::string       mId;

	uiWidget*         mParent;
	WidgetsList       mChilds;

	bool              mVisible;
	uiState*          mVisibleState;

protected:
	vec2              mPosition;
	vec2              mGlobalPosition;
	vec2              mOffset;
	vec2              mSize;
	vec2              mResSize;
	float             mTransparency;
	float             mResTransparency;
	bool              mModal;

	PropertyList      mPropertyList;

	grStencilBufferRenderTarget* mClippingStencilBuffer;
	fRect             mClippingRect;
	bool              mIsClipping;

public:
//functions
	uiWidget(uiWidgetsManager* widgetsManager, const std::string& id = "noName");
	uiWidget(const uiWidget& widget);
	virtual ~uiWidget();

//childs
	virtual void addChild(uiWidget* widget);

	virtual void removeChild(uiWidget* widget);
	virtual void removeAllChilds();

	uiWidget* getWidget(const std::string& id) const;

	template<typename T>
	T* getWidgetByType(const std::string& id) const
	{
		return static_cast<T*>(getWidget(id));
	}

	void adjustSizeByChilds();

//states
	void show(bool forcible = false);
	void hide(bool forcible = false);

//system funcs
	virtual void update(float dt);
	virtual void derivedUpdate(float dt) {}

	virtual void draw();
	virtual void derivedDraw() {}

	int processInputMessage(const cInputMessage& message);
	virtual int processInputMessageDerived(const cInputMessage& message);

	bool isPointInside(const vec2& point);
	virtual bool isPointInsideDerived(const vec2& point);

//parametres
    virtual uiWidget* setPosition(const vec2& position);
	vec2              getPosition() const;
	vec2              getGlobalPosition() const;

	virtual uiWidget* setOffset(const vec2& offset);
	vec2              getOffset() const;

	virtual uiWidget* setSize(const vec2& size);
	vec2              getSize() const;

	uiWidget*         setModal(bool modal);
	bool              isModal() const;

	uiWidget*         setClipping(bool flag);
	bool              isClipping() const;

//other
	virtual uiWidget* clone() const;

	void createStdStates();

	serializeMethodDecl(uiWidget);

protected:
	void registProperty(uiProperty* property);
	void unregistProperty(uiProperty* property);
	virtual void setupInitialProperties();
	virtual void calcClippingRect();
};

#endif //WIDGET_H