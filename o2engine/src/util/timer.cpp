#include "timer.h"

OPEN_O2_NAMESPACE

Timer::Timer()
{
	reset();
}

Timer::~Timer()
{
}

void Timer::reset()
{	
	QueryPerformanceFrequency(&mFrequency);
	QueryPerformanceCounter(&mStartTime);
	mLastElapsedTime = mStartTime.QuadPart;
}

float Timer::getTime()
{	
	LARGE_INTEGER curTime;
	QueryPerformanceCounter(&curTime);

	float res = (float)(curTime.QuadPart - (float)mStartTime.QuadPart)/(float)mFrequency.QuadPart;
	mLastElapsedTime = curTime.QuadPart;

	return res;
}

float Timer::getElapsedTime()
{
	LARGE_INTEGER curTime;
	QueryPerformanceCounter(&curTime);

	float res = (float)(curTime.QuadPart - (float)mLastElapsedTime)/(float)mFrequency.QuadPart;
	mLastElapsedTime = curTime.QuadPart;

	return res;
}

CLOSE_O2_NAMESPACE