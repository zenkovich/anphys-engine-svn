#ifndef UI_PROPERTY_H
#define UI_PROPERTY_H

struct uiState;

struct uiProperty
{
	enum InterpolationType { IT_IMMEDIATELY = 0, IT_LINEAR };
	enum ChangeState { CS_NONE = 0, CS_ACTIVATING, CS_DEACTIVATING };

	uiState*          mState;

	InterpolationType mInterpolationType;
	float             mCurrentTime;
	float             mDuration;

	ChangeState       mChangeState;

//functions
	uiProperty(InterpolationType interpolationType = IT_LINEAR, float duration = 0.5f):
		mInterpolationType(interpolationType), mDuration(duration), mCurrentTime(0), mChangeState(CS_NONE) {}

	virtual ~uiProperty() {}

	virtual void activate(bool forcible = false) { mChangeState = CS_ACTIVATING; }
	virtual void deactivate(bool forcible = false) { mChangeState = CS_DEACTIVATING; }
	virtual void update(float dt) { mChangeState = CS_NONE; }
};

template<typename T>
struct uiParameterProperty:public uiProperty
{
	T* mParameterPtr;
	T  mTargetValue;

	T  mBeginInterpolationValue;

//functions
	uiParameterProperty(T* parameter, const T& targetValue, InterpolationType interpolationType = IT_LINEAR, float duration = 0.5f):
		uiProperty(interpolationType, duration), mParameterPtr(parameter), mTargetValue(targetValue) {}

	~uiParameterProperty() {}

	void activate(bool forcible = false)
	{
		if (forcible)
		{
			mChangeState = CS_NONE;
			mCurrentTime = mDuration;
			*mParameterPtr = mTargetValue;
		}
		else
		{
			mBeginInterpolationValue = *mParameterPtr;
			mChangeState = uiProperty::CS_ACTIVATING;
			mCurrentTime = 0;
		}
	}

	void update(float dt)
	{
		mCurrentTime = 0;
		float coef = 1.0f;
		if (mDuration > FLT_EPSILON)
			coef = mCurrentTime/mDuration;

		if (mChangeState == CS_ACTIVATING)
		{
			*mParameterPtr = mBeginInterpolationValue + (mTargetValue - mBeginInterpolationValue)*coef;
		}
	}
};

#endif //UI_PROPERTY_H