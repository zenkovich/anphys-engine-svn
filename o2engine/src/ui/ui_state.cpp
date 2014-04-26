#include "ui_state.h"

OPEN_O2_NAMESPACE

uiState::uiState( const string& name, int maxStatesCount /*= 2*/ ):
	mName(name)
{
	mOnActiveStateCallbacks = new cCallbackChain[maxStatesCount];
}

uiState::~uiState()
{

}
/*
ICallback* uiState::registOnStateActivatedCallback( ICallback* callback, int stateId )
{

}

void uiState::unregistOnStateActivatedCallback( ICallback* callback )
{

}

ICallback* uiState::registOnStateDeactivatedCallback( ICallback* callback, int stateId )
{

}

void uiState::unregistOnStateDeactivatedCallback( ICallback* callback )
{

}*/

void uiState::initializeProperties()
{
	state.init(this, &uiState::setState, &uiState::getState);
}

CLOSE_O2_NAMESPACE