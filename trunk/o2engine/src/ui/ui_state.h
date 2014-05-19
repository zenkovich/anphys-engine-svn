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
	PROPERTY(uiState, bool) state;

	cCallbackChain mOnActiveStateCallbacks;
	cCallbackChain mOnDeactiveStateCallbacks;


	uiState(const string& name);
	virtual ~uiState();

	virtual shared<uiState> clone() const = 0;

	virtual void setState(bool state) {}
	virtual bool getState() const { return false; }

	virtual void update(float dt) {}

protected:
	void initializeProperties();
};

CLOSE_O2_NAMESPACE

#endif // UI_STATE_H
