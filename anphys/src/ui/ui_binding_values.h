#ifndef UI_BINDING_VALUES_H
#define UI_BINDING_VALUES_H

#include <vector>

template<typename T>
struct uiBindingValues
{
	struct BindValue
	{
		T* mValuePtr;
		T  mLastValue;

		BindValue():mValuePtr(NULL) {}
		BindValue(T* valuePtr):mValuePtr(valuePtr) { mLastValue = *mValuePtr; }

		void setValue(T* value)
		{
			*mValuePtr = *value;
			mLastValue = *mValuePtr;
		}

		virtual bool checkValue()
		{
			bool res = *mValuePtr == mLastValue;
			mLastValue = *mValuePtr;
			return res;
		}
	};
	typedef std::vector<BindValue> BindValuesList;

	BindValuesList mBindValues;

	uiBindingValues();
	~uiBindingValues();

	void bindValue(T* valuePtr);
	void unbindValue(T* valuePtr);
	void clearBindingValues();
};

#endif //UI_BINDING_VALUES_H