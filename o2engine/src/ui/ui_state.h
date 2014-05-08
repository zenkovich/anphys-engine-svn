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

public: 
	PROPERTY(uiState, int) state;

	cCallbackChain mOnActiveStateCallbacks;
	cCallbackChain mOnDeactiveStateCallbacks;


	uiState(const string& name, int maxStatesCount = 2);
	virtual ~uiState();

	virtual void setState(const int& state) {}
	virtual int getState() const { return 0; }

	virtual void update(float dt) {}

protected:
	void initializeProperties();
};

CLOSE_O2_NAMESPACE

#endif // UI_STATE_H
