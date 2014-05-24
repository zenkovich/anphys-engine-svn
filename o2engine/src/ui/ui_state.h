#ifndef UI_STATE_H
#define UI_STATE_H

#include <map>

#include "public.h"
#include "util/property.h"
#include "util/callback.h"
#include "util/smart_ptrs.h"

OPEN_O2_NAMESPACE
	
class uiState
{
	friend class uiWidget;

protected:
	string           mName;
	shared<uiWidget> mOwnerWidget; 

public: 
	PROPERTY(uiState, bool) state;

	cCallbackChain onActiveStateCallbacks;
	cCallbackChain onBeginActiveStateCallbacks;
	cCallbackChain onDeactiveStateCallbacks;
	cCallbackChain onBeginDeactiveStateCallbacks;


	uiState(const string& name);
	uiState(const uiState& state);
	virtual ~uiState();

	virtual shared<uiState> clone() const = 0;

	virtual void setState(bool state, bool forcible = false) {}
	virtual bool getState() const { return false; }

	virtual void update(float dt) {}

protected:
	virtual void setOwnerWidget(const shared<uiWidget>& ownerWidget);
	void setStateNonForcible(bool state);

	void initializeProperties();
};

CLOSE_O2_NAMESPACE

#endif // UI_STATE_H
