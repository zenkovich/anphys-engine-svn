#include "animation_base.h"

#include <math.h>

#include "util/math/math.h"

OPEN_O2_NAMESPACE
	
IAnimation::IAnimation():
	mBeginPlayingCallback(NULL), mEndPlayingCallback(NULL), mTime(0), mPlaying(false), mPlayed(false), mBeginPlaying(0), 
	mEndPlaying(0), mDuration(0), mAnimationSpeed(1), mLoop(LT_NONE), mDirectionSign(1), mLastFrameTime(0), 
	mAbsoluteTime(0)
{
}

IAnimation::~IAnimation()
{
}

void IAnimation::play( bool restart /*= false*/, bool stopAtEnd /*= true*/ )
{
	play(restart ? 0.0f:mAbsoluteTime, stopAtEnd ? mDuration:FLT_MAX);
}

void IAnimation::play( float begin, float end )
{
	mBeginPlaying = begin;
	mEndPlaying = end;

	mPlaying = true;
	mPlayed = false;
	mAbsoluteTime = mBeginPlaying;

	mDirectionSign = sign(mEndPlaying - mBeginPlaying);

	updateTime();
	mLastFrameTime = mTime;

	evaluate();
	checkBeginCallback();
}

void IAnimation::update( float dt )
{
	if (!mPlaying)
		return;

	mAbsoluteTime += mDirectionSign*mAnimationSpeed*dt;

	float endDiff = (mAbsoluteTime - mEndPlaying)*mDirectionSign*sign(mAnimationSpeed);
	if (endDiff > 0)
	{
		mAbsoluteTime = mEndPlaying;
		mPlaying = false;
	}
	
	updateTime();
	evaluate();

	if (mTime > mLastFrameTime)
		checkTimeCallbacks(mLastFrameTime, mTime);
	else
		checkTimeCallbacks(mTime, mLastFrameTime);

	if (!mPlaying)
		checkEndCallback();
}

void IAnimation::updateTime()
{
	mLastFrameTime = mTime;

	if (mLoop == LT_NONE)
	{
		mTime = clamp(mAbsoluteTime, 0.0f, mDuration);
	}
	else if (mLoop == LT_REPEAT) 
	{
		float x;
		if (mAbsoluteTime > 0)
		{
			mTime = modff(mAbsoluteTime/mDuration, &x)*mDuration;
		}
		else
		{
			mTime = (1.0f - modff(-mAbsoluteTime/mDuration, &x))*mDuration;
		}
	}
	else //if (mLoop == LT_TOGGLE)
	{
		float x;
		if (mAbsoluteTime > 0)
		{
			mTime = modff(mAbsoluteTime/mDuration, &x)*mDuration;
			if ((int)x%2 == 1)
				mTime = mDuration - mTime;
		}
		else
		{
			mTime = (1.0f - modff(-mAbsoluteTime/mDuration, &x))*mDuration;
			if ((int)x%2 == 0)
				mTime = mDuration - mTime;
		}
	}
}

void IAnimation::stop()
{
	mPlaying = false;
}

float IAnimation::getTime() const
{
	return mAbsoluteTime;
}

float IAnimation::getLocalTime() const
{
	return mTime;
}

void IAnimation::setTime( float time )
{
	mAbsoluteTime = time;
	updateTime();
	mLastFrameTime = mTime;
	evaluate();
}

void IAnimation::setLoop( LoopType type /*= LT_REPEAT*/ )
{
	mLoop = type;
}

void IAnimation::setSpeed( float speed /*= 1*/ )
{
	mAnimationSpeed = speed;	
	mDirectionSign = sign(mEndPlaying - mBeginPlaying);
}

float IAnimation::getSpeed() const
{
	return mAnimationSpeed;
}

float IAnimation::getDuration() const
{
	return mDuration;
}

float IAnimation::getBeginTime() const
{
	return mBeginPlaying;
}

float IAnimation::getEndTime() const
{
	return mEndPlaying;
}

bool IAnimation::isPlaying() const
{
	return mPlaying;
}

bool IAnimation::isPlayed() const
{
	return mPlayed;
}

void IAnimation::setPlaying( bool playing )
{	
	if (playing == isPlaying()) 
		return;

	if (playing)
		play(mBeginPlaying, mEndPlaying);
	else
		stop();
}

void IAnimation::setBeginPlayingCallback( shared(ICallback) cb )
{
	mBeginPlayingCallback = cb;
}

void IAnimation::setEndPlayingCallback( shared(ICallback) cb )
{
	mEndPlayingCallback = cb;
}

void IAnimation::setTimeCallback( float time, shared(ICallback) cb )
{
	mTimedCallbacks.push_back(TimeCallback(cb, time));
}

void IAnimation::checkBeginCallback()
{
	if (mBeginPlayingCallback)
		mBeginPlayingCallback->call();
}

void IAnimation::checkEndCallback()
{
	if (mEndPlayingCallback)
		mEndPlayingCallback->call();
}

void IAnimation::checkTimeCallbacks( float begin, float end )
{
	for (TimeCallbacksVec::iterator it = mTimedCallbacks.begin(); it != mTimedCallbacks.end(); ++it)
	{
		if (!(it->mTime < begin || it->mTime > end))
			it->mCallback->call();
	}
}

CLOSE_O2_NAMESPACE