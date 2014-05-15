#include "ui_state.h"

OPEN_O2_NAMESPACE

uiState::uiState( const string& name, int maxStatesCount /*= 2*/ ):
	mName(name)
{
}

uiState::~uiState()
{
}

void uiState::initializeProperties()
{
	state.init(this, &uiState::setState, &uiState::getState);
}

CLOSE_O2_NAMESPACE