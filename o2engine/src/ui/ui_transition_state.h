#ifndef UI_TRANSITION_STATE_H
#define UI_TRANSITION_STATE_H

#include "ui_state.h"
#include "util/animation/frame.h"
#include "util/animation/frame_interpolation.h"

OPEN_O2_NAMESPACE

class uiTransitionState
{
public:
	struct IProperty
	{
		virtual void update(float dt) = 0;
		virtual void setState(bool state) = 0;
		virtual bool isComplete() = 0;
	};

	template<typename T>
	struct valueProperty: public IProperty
	{
		T*                     mPropertyPtr;
		cAnimFrame<T>          mStateOff;
		cAnimFrame<T>          mStateOn;
		cFrameInterpolation<T> mInterpolator;
		float                  mTime;
		bool                   mTargetState;
		bool                   mComplete;

		valueProperty(T* propertyPtr, const T& stateOff, const T& stateOn, float duration)
		{
			mPropertyPtr = propertyPtr;
			mStateOff = cAnimFrame<T>(stateOff, duration);
			mStateOn = cAnimFrame<T>(stateOn, duration);
			mInterpolator.initialize(&mStateOff, &mStateOn);
			mTime = 0;
			mTargetState = false;
			mComplete = true;
		}

		valueProperty(T* propertyPtr, const cAnimFrame<T>& stateOff, const cAnimFrame<T>& stateOn)	
		{
			mPropertyPtr = propertyPtr;
			mStateOff = stateOff;
			mStateOn = stateOn;
			mInterpolator.initialize(&mStateOff, &mStateOn);
			mTime = 0;
			mTargetState = false;
			mComplete = true;
		}

		void update(float dt)
		{
			if (mTargetState)
			{
			}
			else
			{
			}
		}

		void setState(bool state);
		bool isComplete();
	};

	typedef vector< shared<IProperty> > PropertiesVec;

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
