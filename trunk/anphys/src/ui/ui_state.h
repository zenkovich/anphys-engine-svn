#ifndef UI_STATE_H
#define UI_STATE_H

#include <string>
#include <vector>

#include "util/serialization/serialization.h"

struct uiWidget;
struct uiProperty;
struct cDataObject;

struct uiState
{
	typedef std::vector<uiProperty*> PropertiesList;

	std::string    mId;
	uiWidget*      mTargetWidget;
	bool           mActive;

	PropertiesList mProperties;

//functions
	uiState(uiWidget* targetWidget, const std::string& id);
	uiState(uiWidget* targetWidget, cDataObject* dataObject);
	~uiState();

	void        activate(bool forcible = false);
	void        deactivate(bool forcible = false);

	uiProperty* addProperty(uiProperty* uiproperty);
	void        removeProperty(uiProperty* uiproperty);
	void        removeAllProperties();

	void        update(float dt);

	serializeMethodDecl(uiState);
};

#endif //UI_STATE_H