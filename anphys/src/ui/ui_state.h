#ifndef UI_STATE_H
#define UI_STATE_H

#include <string>
#include <vector>

#include "util/serialization/serialization.h"

struct uiWidget;
struct uiProperty;
struct cDataObject;
struct cCallbackInterface;

struct uiState
{
	typedef std::vector<uiProperty*> PropertiesList;

	std::string         mId;
	uiWidget*           mTargetWidget;
	bool                mActive;
	bool                mChangeStateStarted;

	cCallbackInterface* mOnActivatedCallback;
	cCallbackInterface* mOnDeactivatedCallback;

	PropertiesList      mProperties;

//functions
	uiState(uiWidget* targetWidget, const std::string& id);
	uiState(uiWidget* targetWidget, cDataObject* dataObject);
	uiState(const uiState& state);
	~uiState();

	void        activate(bool forcible = false);
	void        deactivate(bool forcible = false);

	void        update();

	uiProperty* addProperty(uiProperty* uiproperty, uiWidget* targetWidget = NULL);
	void        removeProperty(uiProperty* uiproperty);
	void        removeAllProperties();

	void setOnActivatedCallback(cCallbackInterface* callback);
	void setOnDeactivatedCallback(cCallbackInterface* callback);

	serializeMethodDecl(uiState);
};

#endif //UI_STATE_H