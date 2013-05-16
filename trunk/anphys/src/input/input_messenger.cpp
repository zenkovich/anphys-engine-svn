#include "input_messenger.h"

#include <algorithm>

#include "input_listener.h"

void cInputMessenger::registInputListener( cInputListener* listener, float priority /*= 1.0f*/ )
{
	mInputListeners.push_back(Listener(listener, priority));

	std::sort(mInputListeners.begin(), mInputListeners.end(), &compareListenersPriority);
}

void cInputMessenger::unregistInputListener( cInputListener* listener )
{
	for (InputListenersList::iterator it = mInputListeners.begin(); it != mInputListeners.end(); ++it)
	{
		if (it->mListener == listener)
		{
			mInputListeners.erase(it);
			break;
		}
	}

	std::sort(mInputListeners.begin(), mInputListeners.end(), &compareListenersPriority);
}

void cInputMessenger::sendInputMessage()
{
	for (InputListenersList::iterator it = mInputListeners.begin(); it != mInputListeners.end(); ++it)
	{
		it->mListener->mLastInputMessage = &mInputMessage;
		it->mListener->processInputMessage(mInputMessage);
	}
}

bool cInputMessenger::compareListenersPriority( Listener& a, Listener& b )
{
	return a.mPriority > b.mPriority;
}
