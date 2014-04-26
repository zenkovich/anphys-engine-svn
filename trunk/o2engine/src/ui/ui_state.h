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
	string     mName;
	cCallbackChain* mOnActiveStateCallbacks;
	cCallbackChain* mOnDeactiveStateCallbacks;

public: 
	PROPERTY(uiState, int) state;

	uiState(const string& name, int maxStatesCount = 2);
	virtual ~uiState();

	virtual void setState(const int& state) {}
	virtual int getState() const { return 0; }

	virtual void update(float dt) {}

	ICallback* registOnStateActivatedCallback(ICallback* callback, int stateId);
	void unregistOnStateActivatedCallback(ICallback* callback);
	void unregistAllOnActivatedStateCallbacks();

	ICallback* registOnStateDeactivatedCallback(ICallback* callback, int stateId);
	void unregistOnStateDeactivatedCallback(ICallback* callback);
	void unregistAllOnDeactivatedStateCallbacks();

protected:
	void initializeProperties();
};

CLOSE_O2_NAMESPACE

#endif // UI_STATE_H
