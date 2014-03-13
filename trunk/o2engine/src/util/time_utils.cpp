#include "time_utils.h"

OPEN_O2_NAMESPACE

DECLARE_SINGLETON(cTimeUtil);

cTimeUtil::cTimeUtil():
	mApplicationTime(0), mLocalTime(0), mCurrentFrame(0), mDeltaTime(0), mFPS(0), mFPSSum(0),
	mFramesSum(0), mLastFPSCheckingTime(0)
{
}

cTimeUtil::~cTimeUtil()
{
}

void cTimeUtil::update( float dt )
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

float cTimeUtil::getApplicationTime() const
{
	return mApplicationTime;
}

float cTimeUtil::getLocalTime() const
{
	return mLocalTime;
}

void cTimeUtil::resetLocalTime()
{
	mLocalTime = 0;
}

void cTimeUtil::setLocalTime( float time )
{
	mLocalTime = time;
}

int cTimeUtil::getCurrentFrame() const
{
	return mCurrentFrame;
}

float cTimeUtil::getDeltaTime() const
{
	return mDeltaTime;
}

float cTimeUtil::getFPS() const
{
	return mFPS;
}

CLOSE_O2_NAMESPACE