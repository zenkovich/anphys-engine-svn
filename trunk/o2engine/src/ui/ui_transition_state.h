#ifndef UI_TRANSITION_STATE_H
#define UI_TRANSITION_STATE_H

#include "ui_state.h"
#include "util/animation/frame.h"
#include "util/animation/frame_interpolation.h"

OPEN_O2_NAMESPACE

class uiTransitionState: public uiState
{
public:
	class IProperty
	{
	public:
		virtual shared<IProperty> clone() const = 0;
		virtual void update(float dt) = 0;
		virtual void setState(bool state, bool forcible = false) = 0;
		virtual bool isComplete() = 0;
		virtual void setOwner(const shared<uiTransitionState>& owner) = 0;
	};

	typedef vector< shared<IProperty> > PropertiesVec;

protected:
	PropertiesVec mProperties;
	bool          mState;
	bool          mChangingState;

public:
	uiTransitionState(const string& name);
	uiTransitionState(const uiTransitionState& transitionState);
	~uiTransitionState();

	shared<uiState> clone() const;

	void setState(bool state, bool forcible = false);
	bool getState() const;

	shared<IProperty> addProperty(const shared<IProperty>& property);

	void update(float dt);

protected:
	void setOwnerWidget(const shared<uiWidget>& ownerWidget);
};

CLOSE_O2_NAMESPACE

#endif //UI_TRANSITION_STATE_H
