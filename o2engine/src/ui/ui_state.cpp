#include "ui_state.h"

OPEN_O2_NAMESPACE

uiState::uiState( const string& name):
	mName(name)
{
}

uiState::uiState( const uiState& state )
{
	mName = state.mName;
	mOwnerWidget = NULL;
}

uiState::~uiState()
{
}

void uiState::initializeProperties()
{
	state.initNonConstSetter(this, &uiState::setStateNonForcible, &uiState::getState);
}

void uiState::setOwnerWidget( const shared<uiWidget>& ownerWidget )
{
	mOwnerWidget = ownerWidget;
}

void uiState::setStateNonForcible(bool state)
{
	setState(state, false);
}

CLOSE_O2_NAMESPACE