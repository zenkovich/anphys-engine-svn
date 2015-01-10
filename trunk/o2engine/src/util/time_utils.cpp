#include "time_utils.h"

OPEN_O2_NAMESPACE

TimeUtil::TimeUtil():
	mApplicationTime(0), mLocalTime(0), mCurrentFrame(0), mDeltaTime(0), mFPS(0), mFPSSum(0),
	mFramesSum(0), mLastFPSCheckingTime(0)
{
}

TimeUtil::~TimeUtil()
{
}

void TimeUtil::update( float dt )
{
	mDeltaTime = dt;
	mApplicationTime += dt;
	mLocalTime += dt;
	mCurrentFrame++;

	mFramesSum += 1.0f;
	mFPSSum += 1.0f/dt;
	if (mApplicationTime - mLastFPSCheckingTime > 0.3f)
	{
		mLastFPSCheckingTime = mApplicationTime;
		mFPS = mFPSSum/mFramesSum;
		mFPSSum = 0;
		mFramesSum = 0;
	}
}

float TimeUtil::getApplicationTime() const
{
	return mApplicationTime;
}

float TimeUtil::getLocalTime() const
{
	return mLocalTime;
}

void TimeUtil::resetLocalTime()
{
	mLocalTime = 0;
}

void TimeUtil::setLocalTime( float time )
{
	mLocalTime = time;
}

int TimeUtil::getCurrentFrame() const
{
	return (int)mCurrentFrame;
}

float TimeUtil::getDeltaTime() const
{
	return mDeltaTime;
}

float TimeUtil::getFPS() const
{
	return mFPS;
}
CLOSE_O2_NAMESPACE