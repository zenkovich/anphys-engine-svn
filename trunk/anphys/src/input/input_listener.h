#ifndef INPUT_LISTENER_H
#define INPUT_LISTENER_H

#include "input_message.h"

struct cInputListener
{
	virtual int processInputMessage(const cInputMessage& inputMessage) = 0;
};

#endif //INPUT_LISTENER_H