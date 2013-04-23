#ifndef CALLBACK_H
#define CALLBACK_H

struct Dummy
{
	Dummy() {}
	~Dummy() {}
};

struct cCallbackInterface
{
	virtual ~cCallbackInterface() {}
	virtual void call() = 0;
};

template<typename T = Dummy>
struct cCallback:public cCallbackInterface
{
	T* mObject;
	void (T::*mObjectFunction)();
	void (*mFunction)();

	cCallback(T* object, void (T::*function)()):mObject(object), mObjectFunction(function) {}
	cCallback(void (*function)()):mObject(NULL), mObjectFunction(NULL), mFunction(function) {}
	cCallback(const cCallback& callback)
	{
		mObject = callback.mObject;
		mObjectFunction = callback.mObjectFunction;
		mFunction = callback.mFunction;
	}

	void call()
	{
		if (mObject && mObjectFunction) (mObject->*mObjectFunction)();
		else if (mFunction) (*mFunction)();
	}
};

template<typename ArgT, typename T = Dummy>
struct cCallback1Param:public cCallbackInterface
{
	ArgT mArg;
	T* mObject;
	void (T::*mObjectFunction)(ArgT);
	void (*mFunction)(ArgT);

	cCallback1Param(T* object, void (T::*function)(ArgT)):mObject(object), mObjectFunction(function) {}
	cCallback1Param(void (*function)(ArgT)):mObject(NULL), mObjectFunction(NULL), mFunction(function) {}
	cCallback1Param(const cCallback1Param<ArgT, T>& callback)
	{
		mObject = callback.mObject;
		mObjectFunction = callback.mObjectFunction;
		mFunction = callback.mFunction;
		mArg = callback.mArg;
	}

	void call()
	{
		if (mObject && mObjectFunction) (mObject->*mObjectFunction)(mArg);
		else if (mFunction) (*mFunction)(mArg);
	}
};

#endif //CALLBACK_H