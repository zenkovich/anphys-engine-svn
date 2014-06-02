#include "ui_state.h"

OPEN_O2_NAMESPACE

uiState::uiState( const string& name):
	mName(name)
{
	initializeProperties();
}

uiState::uiState( const uiState& state )
{
	mName = state.mName;
	mOwnerWidget = NULL;
	initializeProperties();
}

uiState::~uiState()
{
}

void uiState::initializeProperties()
{
	REG_PROPERTY_SETTER_NONCONST(uiState, state, setStateNonForcible, getState);
}

void uiState::setOwnerWidget( const shared<uiWidget>& ownerWidget )
{
	mOwnerWidget = ownerWidget;
}

void uiState::setStateNonForcible(bool state)
{
	setState(state, false);
}

string uiState::getName() const
{
	return mName;
}

CLOSE_O2_NAMESPACE