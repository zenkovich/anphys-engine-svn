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
	virtual cCallbackInterface* clone() = 0;
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

	cCallbackInterface* clone() 
	{
		return new cCallback<T>(*this);
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
	
	cCallbackInterface* clone() 
	{
		return new cCallback1Param<ArgT, T>(*this);
	}
};

template<typename ArgT, typename ArgT2, typename T = Dummy>
struct cCallback2Param:public cCallbackInterface
{
	ArgT  mArg;
	ArgT2 mArg2;
	T*    mObject;
	void (T::*mObjectFunction)(ArgT, ArgT2);
	void (*mFunction)(ArgT, ArgT2);

	cCallback2Param(T* object, void (T::*function)(ArgT, ArgT2)):mObject(object), mObjectFunction(function) {}
	cCallback2Param(void (*function)(ArgT, ArgT2)):mObject(NULL), mObjectFunction(NULL), mFunction(function) {}

	cCallback2Param(T* object, void (T::*function)(ArgT, ArgT2), const ArgT& arg1, const ArgT2& arg2 ):
		mObject(object), mObjectFunction(function), mArg(arg1), mArg2(arg2) {}

	cCallback2Param(void (*function)(ArgT, ArgT2), const ArgT& arg1, const ArgT2& arg2):
		mObject(NULL), mObjectFunction(NULL), mFunction(function), mArg(arg1), mArg2(arg2) {}
	
	cCallback2Param(const cCallback2Param<ArgT, ArgT2, T>& callback)
	{
		mObject = callback.mObject;
		mObjectFunction = callback.mObjectFunction;
		mFunction = callback.mFunction;
		mArg = callback.mArg;
		mArg2 = callback.mArg2;
	}

	void call()
	{
		if (mObject && mObjectFunction) (mObject->*mObjectFunction)(mArg, mArg2);
		else if (mFunction) (*mFunction)(mArg, mArg2);
	}
	
	cCallbackInterface* clone() 
	{
		return new cCallback2Param<ArgT, ArgT2, T>(*this);
	}
};

#endif //CALLBACK_H