#include "ui_transition_state.h"

#include "util/callback.h"

OPEN_O2_NAMESPACE

uiTransitionState::uiTransitionState(const string& name):
	uiState(name), mState(false), mChangingState(false)
{
}

uiTransitionState::uiTransitionState(const uiTransitionState& transitionState):
	uiState(transitionState), mChangingState(false)
{
	FOREACH_CONST(PropertiesVec, transitionState.mProperties, prop)
		addProperty((*prop)->clone());

	setState(transitionState.mState, true);
}

uiTransitionState::~uiTransitionState()
{
	FOREACH(PropertiesVec, mProperties, prop)
		safe_release(*prop);
}

shared<uiState> uiTransitionState::clone() const
{
	shared<uiTransitionState> res = mnew uiTransitionState(*this);
	return res;
}

void uiTransitionState::setState(bool state, bool forcible)
{
	if (mState == state)
		return;

	mState = state;
	mChangingState = !forcible;

	if (state)
		onBeginActiveStateEvent.call();
	else
		onBeginDeactiveStateEvent.call();

	FOREACH(PropertiesVec, mProperties, prop)
		(*prop)->setState(state, forcible);

	if (forcible)
	{
		if (state)
			onActiveStateEvent.call();
		else
			onDeactiveStateEvent.call();
	}
}

bool uiTransitionState::getState() const
{
	return mState;
}

void uiTransitionState::update(float dt)
{
	if (!mChangingState)
		return;

	bool complete = true;
	FOREACH(PropertiesVec, mProperties, prop)
	{
		if ((*prop)->isComplete())
			continue;

		(*prop)->update(dt);
		complete = false;
	}

	mChangingState = !complete;
	if (complete)
	{
		if (mState)
			onActiveStateEvent.call();
		else
			onDeactiveStateEvent.call();
	}
}

shared<uiTransitionState::IProperty> uiTransitionState::addProperty(const shared<IProperty>& property)
{
	mProperties.push_back(property);

	if (mOwnerWidget)
		property->setOwner( getShared<uiTransitionState>(this) );

	return property;
}

void uiTransitionState::setOwnerWidget( const shared<uiWidget>& ownerWidget )
{
	uiState::setOwnerWidget(ownerWidget);
	shared<uiTransitionState> thisShared = getShared<uiTransitionState>(this);
	FOREACH(PropertiesVec, mProperties, prop)
		(*prop)->setOwner(thisShared);
}

CLOSE_O2_NAMESPACE