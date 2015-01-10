#ifndef MUTEX_H
#define MUTEX_H

#include "public.h"

#include "pthreads/pthread.h"

OPEN_O2_NAMESPACE

/** Mutex. Uses POSIX. */
class Mutex
{
	pthread_mutex_t mMutex;       /** Mutex identificator. */
	bool            mInitialized; /** True, when initialized. */

public:
	enum Type { MT_DEFAULT = 0, MT_RECURSIVE };

	/** ctor. */
	Mutex(Type type = MT_DEFAULT);

	/** dtor. */
	~Mutex();

	/** Locking mutex. Returns 0 if success. */
	int lock();

	/** Trying to lock mutex. Returns 0 if success. */
	int tryLock();

	/** Unlocking mutex. */
	int unlock();
};

CLOSE_O2_NAMESPACE

#endif //MUTEX_H