#ifndef QUEUE_H
#define QUEUE_H

#include <vector>


/** Очередь из указателей на элемент. Хранит в себе список свбодных контейнеров и связанную цепь этих контейнеров.
*   Позволяет доставать элемент из начала и добавлять в конец. */
template<typename T>
struct cQueue
{
	/** Контейнер для элемента. Хранит указатель на элемент, указатели на предыдущий и следующий контейнер. */
	struct element
	{
		T* mValuePtr;
		element* mLastValue;
		element* mNextValue;

		element():mValuePtr(NULL), mLastValue(NULL), mNextValue(NULL) {}
	};

	element* mFirstElement;
	element* mLastElement;

	typedef std::vector< element* > ElementsList;
	ElementsList mFreeElements;


	cQueue(unsigned int reserveCount = 10):mFirstElement(NULL), mLastElement(NULL) 
	{
		for (unsigned int i = 0; i < reserveCount; ++i)
			mFreeElements.push_back(new element);
	}

	~cQueue()
	{
		if (!mFirstElement) return;

		element* currElement = mFirstElement;
		do 
		{
			if (currElement->mValuePtr) delete currElement->mValuePtr;
			if (currElement->mNextValue) currElement = currElement->mNextValue;
		} 
		while (currElement->mNextValue);
	}

	T* getValue()
	{
		T* res = mFirstElement->mValuePtr;

		//push elemtnt to free elements list
		element* freedElement = mFirstElement;
		mFirstElement = freedElement->mNextValue;

		freedElement->mLastValue = freedElement->mNextValue = freedElement->mValuePtr = NULL;
		mFreeElements.push_back(freedElement);

		return res;
	}

	void  putValue(T* value)
	{
		//getting free element
		element* newElement = NULL;
		if (mFreeElements.size() > 0)
		{
			ElementsList::iterator last = mFreeElements.end() - 1;
			newElement = *last;
			mFreeElements.erase(last);
		}
		else
		{
			newElement = new element;
		}

		mLastElement->mNextValue = newElement;
		mLastElement->mNextValue->mValuePtr = value;
		mLastElement->mNextValue->mLastValue = mLastElement;
		mLastElement = mLastElement->mNextValue;
	}
};

#endif //QUEUE_H