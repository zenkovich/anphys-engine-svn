#ifndef UI_TRANSITION_STATE_H
#define UI_TRANSITION_STATE_H

#include "ui_state.h"
#include "util/animation/frame.h"
#include "util/animation/frame_interpolation.h"

OPEN_O2_NAMESPACE

class uiTransitionState
{
public:
	template<typename T>
	struct valueProperty
	{
		T*                     mPropertyPtr;
		cAnimFrame<T>          mStateOff;
		cAnimFrame<T>          mStateOn;
		cFrameInterpolation<T> mInterpolator;
		float                  mTime;

		valueProperty(T* propertyPtr, const T& stateOff, const T& stateOn);
		valueProperty(T* propertyPtr, const cAnimFrame<T>& stateOff, const cAnimFrame<T>& stateOn);
	};
	typedef vector<valueProperty> PropertiesVec;

protected:
	PropertiesVec mProperties;
	bool          mState;

public:
	uiTransitionState(const string& name);
	~uiTransitionState();

	shared<uiState> clone() const;

	void setState(bool state);
	bool getState() const;

	template<typename T>
	shared<uiTransitionState> addProperty(const valueProperty<T>& property);

	void update(float dt);
};

CLOSE_O2_NAMESPACE

#endif //UI_TRANSITION_STATE_H
