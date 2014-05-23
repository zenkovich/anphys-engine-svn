#include "ui_transition_state.h"

#include "util/callback.h"

OPEN_O2_NAMESPACE

namespace _uiTransitionState
{

template<typename T>
class ValueProperty: public uiTransitionState::IProperty
{
	T*                        mPropertyPtr;
	cAnimFrame<T>             mStateOff;
	cAnimFrame<T>             mStateOn;
	cFrameInterpolation<T>    mInterpolator;
	float                     mTime;
	float                     mInvDuration;
	bool                      mTargetState;
	bool                      mComplete;
	shared<ICallback>         mOnChanged;
	string                    mWidgetPropertyId;
	shared<uiTransitionState> mStateOwner;

public:
	ValueProperty(const std::string& propertyId, const T& stateOff, const T& stateOn, float duration, 
		          const shared<ICallback>& onChanged = NULL)
	{
		mPropertyPtr = NULL;
		mStateOff = cAnimFrame<T>(stateOff, duration);
		mStateOn = cAnimFrame<T>(stateOn, duration);
		mInterpolator.initialize(&mStateOff, &mStateOn);
		mTime = 0;
		mInvDuration = 1.0f;
		mTargetState = false;
		mComplete = true;
		mOnChanged = onChanged;
		mWidgetPropertyId = propertyId;
		mStateOwner = NULL;
	}

	ValueProperty(const std::string& propertyId, const cAnimFrame<T>& stateOff, const cAnimFrame<T>& stateOn, 
		          const shared<ICallback>& onChanged = NULL)	
	{
		mPropertyPtr = NULL;
		mStateOff = stateOff;
		mStateOn = stateOn;
		mInterpolator.initialize(&mStateOff, &mStateOn);
		mTime = 0;
		mInvDuration = 1.0f;
		mTargetState = false;
		mComplete = true;
		mOnChanged = onChanged;
		mWidgetPropertyId = propertyId;
		mStateOwner = NULL;
	}

	shared<uiTransitionState::IProperty> clone() const
	{
		shared<ValueProperty> res = mnew ValueProperty(mWidgetPropertyId, mStateOff, mStateOn, mOnChanged);
		return res;
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

		mInterpolator.getValue(*mPropertyPtr, mTime*mInvDuration);

		if (mOnChanged)
			mOnChanged->call();
	}

	void setState(bool state, bool forcible = false)
	{
		if (forcible)
		{
			mComplete = true;
			mInterpolator.getValue(*mPropertyPtr, state ? 1.0f:0.0f);

			return;
		}

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

	void setOwner(const shared<uiTransitionState>& owner)
	{
		mStateOwner = owner;
		mPropertyPtr = owner->mOwnerWidget->getProperty(mWidgetPropertyId);
	}
};

} //_uiTransitionState


uiTransitionState::uiTransitionState(const string& name):
	uiState(name), mState(false), mChangingState(false)
{
}

uiTransitionState::uiTransitionState(const uiTransitionState& transitionState):
	uiState(transitionState), mChangingState(false)
{
	FOREACH(PropertiesVec, mProperties, prop)
		addProperty((*prop)->clone());

	mState = transitionState.mState;
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

	FOREACH(PropertiesVec, mProperties, prop)
		(*prop)->setState(state, forcible);
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
			mOnActiveStateCallbacks.call();
		else
			mOnDeactiveStateCallbacks.call();
	}
}

shared<uiTransitionState::IProperty> uiTransitionState::addProperty(const shared<IProperty>& property)
{
	mProperties.push_back(property);

	if (mOwnerWidget)
		property->setOwner(shared<uiTransitionState>(this).disableAutoRelease());

	return property;
}

void uiTransitionState::setOwnerWidget( const shared<uiWidget>& ownerWidget )
{
	uiState::setOwnerWidget(ownerWidget);
	shared<uiTransitionState> thisShared = shared<uiTransitionState>(this).disableAutoRelease();
	FOREACH(PropertiesVec, mProperties, prop)
		(*prop)->setOwner(thisShared);
}

CLOSE_O2_NAMESPACE