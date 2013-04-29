#include "ui_state.h"

#include <algorithm>

#include "ui_property.h"
#include "ui_widget.h"
#include "util/other/callback.h"
#include "util/serialization/data_object.h"

uiState::uiState(uiWidget* targetWidget, const std::string& id):
	mTargetWidget(targetWidget), mId(id), mActive(false), mOnActivatedCallback(NULL),
	mOnDeactivatedCallback(NULL), mChangeStateStarted(false)
{
}

uiState::uiState( uiWidget* targetWidget, cDataObject* dataObject ):
	mTargetWidget(targetWidget), mChangeStateStarted(false)
{
	serialize(*dataObject, AT_INPUT, "");
}
	
uiState::uiState(const uiState& state)
{
	mTargetWidget = state.mTargetWidget;
	mId = state.mId; 
	mActive = false;
	mChangeStateStarted = false;

	for (PropertiesList::const_iterator it = state.mProperties.cbegin(); it != state.mProperties.cend(); ++it)
		mProperties.push_back((*it)->clone());

	if (state.mOnActivatedCallback)
		mOnActivatedCallback = state.mOnActivatedCallback->clone();
	else
		mOnActivatedCallback = NULL;

	if (state.mOnDeactivatedCallback)
		mOnDeactivatedCallback = state.mOnDeactivatedCallback->clone();
	else
		mOnDeactivatedCallback = NULL;
}

uiState::~uiState()
{
	removeAllProperties();
	safe_release(mOnActivatedCallback);
	safe_release(mOnDeactivatedCallback);
}

void uiState::activate( bool forcible /*= false*/ )
{
	for (PropertiesList::iterator it = mProperties.begin(); it != mProperties.end(); ++it)
		(*it)->activate(forcible);

	mActive = true;
	mChangeStateStarted = true;
}

void uiState::deactivate( bool forcible /*= false*/ )
{
	for (PropertiesList::iterator it = mProperties.begin(); it != mProperties.end(); ++it)
		(*it)->deactivate(forcible);

	mActive = false;
	mChangeStateStarted = true;
}

uiProperty* uiState::addProperty( uiProperty* uiproperty, uiWidget* targetWidget /*= NULL*/ )
{
	uiproperty->mState = this;
	mProperties.push_back(uiproperty);

	if (targetWidget)
		targetWidget->registProperty(uiproperty);
	else
		mTargetWidget->registProperty(uiproperty);

	return uiproperty;
}

void uiState::removeProperty( uiProperty* uiproperty )
{
	mTargetWidget->unregistProperty(uiproperty);

	PropertiesList::iterator fnd = std::find(mProperties.begin(), mProperties.end(), uiproperty);

	if (fnd == mProperties.end()) 
		return;

	safe_release(uiproperty);
	mProperties.erase(fnd);
}

void uiState::removeAllProperties()
{
	for (PropertiesList::iterator it = mProperties.begin(); it != mProperties.end(); ++it)
	{
		mTargetWidget->unregistProperty(*it);
		safe_release(*it);
	}

	mProperties.clear();
}

serializeMethodImpl(uiState)
{
	//if (achieveType == AT_OUTPUT)

	return true;
}

void uiState::update()
{
	if (!mChangeStateStarted)
		return;

	for (PropertiesList::iterator it = mProperties.begin(); it != mProperties.end(); ++it)
	{
		if ((*it)->mChangeState != uiProperty::CS_NONE)
			return;
	}

	if (mActive && mOnActivatedCallback)
		mOnActivatedCallback->call();

	if (!mActive && mOnDeactivatedCallback)
		mOnDeactivatedCallback->call();

	mChangeStateStarted = false;
}

void uiState::setOnActivatedCallback( cCallbackInterface* callback )
{
	safe_release(mOnActivatedCallback);
	mOnActivatedCallback = callback;
}

void uiState::setOnDeactivatedCallback( cCallbackInterface* callback )
{
	safe_release(mOnDeactivatedCallback);
	mOnDeactivatedCallback = callback;
}
