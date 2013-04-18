#include "ui_state.h"

#include <algorithm>

#include "util/serialization/data_object.h"
#include "ui_widget.h"
#include "ui_property.h"

uiState::uiState(uiWidget* targetWidget, const std::string& id):
	mTargetWidget(targetWidget), mId(id), mActive(false)
{
}

uiState::uiState( uiWidget* targetWidget, cDataObject* dataObject ):
	mTargetWidget(targetWidget)
{
	serialize(*dataObject, AT_INPUT, "");
}
	
uiState::uiState(const uiState& state)
{
	mTargetWidget = state.mTargetWidget;
	mId = state.mId;
	mActive = false;

	for (PropertiesList::const_iterator it = state.mProperties.cbegin(); it != state.mProperties.cend(); ++it)
		mProperties.push_back((*it)->clone());
}

uiState::~uiState()
{
	removeAllProperties();
}

void uiState::activate( bool forcible /*= false*/ )
{
	for (PropertiesList::iterator it = mProperties.begin(); it != mProperties.end(); ++it)
		(*it)->activate(forcible);

	mActive = true;
}

void uiState::deactivate( bool forcible /*= false*/ )
{
	for (PropertiesList::iterator it = mProperties.begin(); it != mProperties.end(); ++it)
		(*it)->deactivate(forcible);

	mActive = false;
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