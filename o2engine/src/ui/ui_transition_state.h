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

	template<typename T>
	class ValueProperty;

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

public:
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

				if (mStateOwner)
					mInterpolator.getValue(*mPropertyPtr, state ? 1.0f:0.0f);

				mTime = state ? mStateOn.mTime:0.0f;

				return;
			}

			mComplete = false;
			mTargetState = state;
			if (mTargetState)
			{
				mInvDuration = 1.0f/mStateOn.mTime;
				//mTime = 0;
			}
			else
			{
				mInvDuration = 1.0f/mStateOff.mTime;
				//mTime = mStateOff.mTime;
			}
		}

		bool isComplete()
		{
			return mComplete;
		}

		void setOwner(const shared<uiTransitionState>& owner)
		{
			mStateOwner = owner;
			mPropertyPtr = (T*)(owner->mOwnerWidget->getProperty(mWidgetPropertyId).mObjectPtr);
		}
	};
	

	template<typename T>
	shared< ValueProperty<T> > addProperty(const shared< ValueProperty<T> >& property)
	{
		addProperty(property);
		return property;
	}	

	template<typename T>
	shared< ValueProperty<T> > addProperty(const std::string& propertyId, const T& stateOff, const T& stateOn, float duration, 
					                       const shared<ICallback>& onChanged = NULL)
	{
		return addProperty(mnew ValueProperty<T>(propertyId, stateOff, stateOn, duration, onChanged));
	}

	template<typename T>
	shared< ValueProperty<T> > addProperty(const std::string& propertyId, const cAnimFrame<T>& stateOff, const cAnimFrame<T>& stateOn, 
					                       const shared<ICallback>& onChanged = NULL)
	{
		return addProperty(mnew ValueProperty<T>(propertyId, stateOff, stateOn, onChanged));
	}
};

CLOSE_O2_NAMESPACE

#endif //UI_TRANSITION_STATE_H
