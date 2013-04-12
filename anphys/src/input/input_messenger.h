#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "input_message.h"

struct cInputListener;

struct cInputMessenger
{
	struct Listener
	{
		cInputListener* mListener;
		float           mPriority;

		Listener():mListener(NULL), mPriority(0) {}
		Listener(cInputListener* listener, float priority):mListener(listener), mPriority(priority) {}
	};
	typedef std::vector<Listener> InputListenersList;

	cInputMessage      mInputMessage;
	InputListenersList mInputListeners;

	void registInputListener(cInputListener* listener, float priority = 1.0f);
	void unregistInputListener(cInputListener* listener);

	void sendInputMessage();

	static bool compareListenersPriority(Listener& a, Listener& b);
};

#endif //INPUT_MANAGER_H