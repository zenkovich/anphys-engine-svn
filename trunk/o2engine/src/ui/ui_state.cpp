#include "ui_state.h"

OPEN_O2_NAMESPACE

uiState::uiState( const string& name):
	mName(name)
{
}

uiState::~uiState()
{
}

void uiState::initializeProperties()
{
	state.initNonConstSetter(this, &uiState::setState, &uiState::getState);
}

CLOSE_O2_NAMESPACE