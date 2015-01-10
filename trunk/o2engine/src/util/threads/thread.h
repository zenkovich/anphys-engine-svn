#ifndef THREAD_H
#define THREAD_H

#include "public.h"

#include "pthreads/pthread.h"
#include "util/smart_ptrs.h"
#include "util/callback.h"

OPEN_O2_NAMESPACE

//class ICallback;

/** Thread starter class. Uses POSIX. */
class Thread
{
public:
	enum ThreadPriority { TP_NORMAL = 0, TP_LOW, TP_HIGHT };

protected:
	pthread_t  mThreadId;   /** Thread id. */
	ICallback* mThreadFunc; /** Function starting in thread. */
	bool       mStarted;    /** True, when started. */

	/** copy ctor. */
	Thread(const Thread& thread) {}

	/** copy operator. */
	Thread& operator=(const Thread& thread) { return *this; }

	/** Thead function. Here calls mThreadFunc. */
	static void* threadFunc(void* arg);

public:
	/** ctor. */
	Thread();

	/** dtor. */
	~Thread();

	/** Starts function with priority. */
	int start(ICallback* threadCallback, ThreadPriority threadPriority = TP_NORMAL);

	/** Join. */
	int join();

	/** Cancelling thread. */
	int cancel();
};

CLOSE_O2_NAMESPACE

#endif //THREAD_H