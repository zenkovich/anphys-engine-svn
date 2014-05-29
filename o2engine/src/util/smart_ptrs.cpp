#include "smart_ptrs.h"

#include "log.h"
#include "assert.h"

OPEN_O2_NAMESPACE
	

void SharedsErrorOut(const char* res, bool asserting /*= true*/)
{
	if (asserting)
	{
		assert(false, res);
	}
	else
	{
		gLog->error(res);
	}
}

void xrelease(void* data)
{
	delete data;
}


SharedBasics* SharedBasics::instance = new SharedBasics();


SharedBasics::SharedBasics()
{
	instance = this;

	const int initialCount = 100;

	mBasics = new Basics*[initialCount];
	for (int i = 0; i < initialCount; i++)
	{
		mBasics[i] = new Basics();
		if (i > 0)
			mBasics[i - 1]->mNext = mBasics[i];
	}
	mHead = mBasics[0];

	mBasicsCount = initialCount;
	mUsedBasics = 0;
	mUses = 0;
}

SharedBasics::~SharedBasics()
{
	for(unsigned int i = 0; i < mBasicsCount; i++)
		delete mBasics[i];

	delete[] mBasics;
}

SharedBasics::Basics* SharedBasics::get()
{
	const int additiveCount = 100;
	if (mHead == NULL)
		reallocArray(additiveCount);

	Basics* res = mHead;

	mHead = mHead->mNext;
	mUsedBasics++;
	mUses++;

	//printf("usedBasics: %i(%i)\n", mUsedBasics, mUses);

	return res;
}

void SharedBasics::free(Basics* bs)
{
	bs->mNext = mHead;
	bs->mObject = NULL;
	mHead = bs;
	mUsedBasics--;
}

void SharedBasics::reallocArray(unsigned int addCount)
{
	Basics** buf = new Basics*[mBasicsCount];
	memcpy(buf, mBasics, sizeof(Basics*)*mBasicsCount);

	delete[] mBasics;

	unsigned int newCount = mBasicsCount + addCount;
	mBasics = new Basics*[newCount];
	memcpy(mBasics, buf, sizeof(Basics*)*mBasicsCount);

	for (unsigned int i = mBasicsCount; i < newCount; i++)
	{
		mBasics[i] = new Basics();
		if (i > mBasicsCount)
			mBasics[i - 1]->mNext = mBasics[i];
	}
	mHead = mBasics[mBasicsCount];

	mBasicsCount = newCount;
}

SharedBasics::Basics* SharedBasics::find(void* ptr)
{
	for (unsigned int i = 0; i < mBasicsCount; i++)
	{
		Basics* bs = mBasics[i];
		if (bs->mObject == ptr)
			return bs;
	}

	return NULL;
}


SharedBasics::Basics::Basics():
	mObject(NULL), mRefCount(0), mValid(false), mNext(NULL)
{
}

void SharedsRelease(void* ptr)
{
	delete ptr;
}

CLOSE_O2_NAMESPACE