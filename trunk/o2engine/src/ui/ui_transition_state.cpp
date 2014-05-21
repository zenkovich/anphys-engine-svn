#include "ui_transition_state.h"

#include "util/callback.h"

OPEN_O2_NAMESPACE

namespace _uiTransitionState
{

template<typename T>
class ValueProperty: public IProperty
{
	T*                     mPropertyPtr;
	cAnimFrame<T>          mStateOff;
	cAnimFrame<T>          mStateOn;
	cFrameInterpolation<T> mInterpolator;
	float                  mTime;
	float                  mInvDuration;
	bool                   mTargetState;
	bool                   mComplete;
	shared<ICallback>      mOnChanged;
	string                 mWidgetPropertyId;

public:
	ValueProperty(T* propertyPtr, const T& stateOff, const T& stateOn, float duration, 
		          const shared<ICallback>& onChanged = NULL)
	{
		mPropertyPtr = propertyPtr;
		mStateOff = cAnimFrame<T>(stateOff, duration);
		mStateOn = cAnimFrame<T>(stateOn, duration);
		mInterpolator.initialize(&mStateOff, &mStateOn);
		mTime = 0;
		mTargetState = false;
		mComplete = true;
		mOnChanged = onChanged;
	}

	ValueProperty(T* propertyPtr, const cAnimFrame<T>& stateOff, const cAnimFrame<T>& stateOn, 
		          const shared<ICallback>& onChanged = NULL)	
	{
		mPropertyPtr = propertyPtr;
		mStateOff = stateOff;
		mStateOn = stateOn;
		mInterpolator.initialize(&mStateOff, &mStateOn);
		mTime = 0;
		mTargetState = false;
		mComplete = true;
		mOnChanged = onChanged;
	}

	void update(float dt)
	{
		if (mTargetState)
		{
			mTime += dt;
			if (mTime > mStateOn.mTime)
			{
				mComplete = true;
				mTime = mStateOn.mTime;
			}
		}
		else
		{
			mTime -= dt;
			if (mTime < 0)
			{
				mTime = 0;
				mComplete = true;
			}
		}

		*mPropertyPtr = mInterpolator.getValue(*mPropertyPtr, mTime*mInvDuration);

		if (mOnChanged)
			mOnChanged->call();
	}

	void setState(bool state)
	{
		mComplete = false;
		mTargetState = state;
		if (mTargetState)
		{
			mInvDuration = 1.0f/mStateOn.mTime;
			mTime = 0;
		}
		else
		{
			mInvDuration = 1.0f/mStateOff.mTime;
			mTime = mStateOff.mTime;
		}
	}

	bool isComplete()
	{
		return mComplete;
	}
};

} //_uiTransitionState


uiTransitionState::uiTransitionState(const string& name):
	uiState(name), mState(false)
{
}

uiTransitionState::uiTransitionState(const uiTransitionState& transitionState):
	uiState(transitionState.mName)
{
	FOREACH()
}

uiTransitionState::~uiTransitionState()
{
	FOREACH(PropertiesVec, mProperties, prop)
		safe_release(*prop);
}

shared<uiState> uiTransitionState::clone() const
{
	shared<uiTransitionState> res = mnew uiTransitionState(mName);
}

void uiTransitionState::setState(bool state)
{

}

bool uiTransitionState::getState() const
{

}

void uiTransitionState::update(float dt)
{

}

shared<uiTransitionState::IProperty> uiTransitionState::addProperty(const shared<IProperty>& property)
{

}

CLOSE_O2_NAMESPACE