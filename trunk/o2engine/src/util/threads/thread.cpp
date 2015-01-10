#include "thread.h"

#include "../callback.h"

OPEN_O2_NAMESPACE

Thread::Thread():mStarted(false), mThreadFunc(NULL)
{
}

Thread::~Thread()
{
	safe_release(mThreadFunc);
}

int Thread::start( ICallback* threadCallback, ThreadPriority threadPriority /*= TP_NORMAL*/ )
{
	mThreadFunc = threadCallback;

	int res = pthread_create(&mThreadId, NULL, threadFunc, this);

	if (res)
		mStarted = true;

	return res;
}

int Thread::join()
{
	if (!mStarted)
		return -1;

	return pthread_join(mThreadId, NULL);
}

int Thread::cancel()
{
	if (!mStarted)
		return -1;

	return pthread_cancel(mThreadId);
}

void* Thread::threadFunc( void* arg )
{
	Thread* threadObj = reinterpret_cast<Thread*>(arg);
	threadObj->mThreadFunc->call();
	pthread_exit(NULL);
	return NULL;
}

CLOSE_O2_NAMESPACE