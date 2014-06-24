#ifndef UI_STATE_H
#define UI_STATE_H

#include <map>

#include "public.h"
#include "util/property.h"
#include "util/callback.h"
#include "ui_widget.h"

OPEN_O2_NAMESPACE
	
/** Widget state interface. Widget can get bool parameter and change some parameters from widget. */
class uiState
{
	friend class uiWidget;
	friend class uiButton;
	friend class uiDrawablesListWidget;

protected:
	string    mName;        /** Name of state. */
	uiWidget* mOwnerWidget; /** Owner widget. */

public: 
	PROPERTY(uiState, bool) state; /** State property. Using set/getState. */

	cCallbackChain onActiveStateEvent;        /** On state active callbacks. Calls when active state completely setted. */
	cCallbackChain onBeginActiveStateEvent;   /** On state begin active callbacks. Calls when state beginning. */
	cCallbackChain onDeactiveStateEvent;      /** On state deactive callbacks. Calls when active state completely setted. */   
	cCallbackChain onBeginDeactiveStateEvent; /** On state begin deactive callbacks. Calls when state beginning. */


	/** ctor. */
	uiState(const string& name);

	/** copy-ctor. */
	uiState(const uiState& state);

	/** dtor. */
	virtual ~uiState();

	/** Returns clone of state. */
	virtual uiState* clone() const = 0;

	/** Setting state. */
	virtual void setState(bool state, bool forcible = false) {}

	/** Returns state. */
	virtual bool getState() const { return false; }

	/** Updating. */
	virtual void update(float dt) {}

	/** Returns state name. */
	string getName() const;

protected:
	/** Calls when setting owner widget. */
	virtual void setOwnerWidget(uiWidget* ownerWidget);

	/** Sets state. */
	void setStateNonForcible(bool state);

	/** Initializing properties. */
	void initializeProperties();
};

CLOSE_O2_NAMESPACE

#endif // UI_STATE_H
