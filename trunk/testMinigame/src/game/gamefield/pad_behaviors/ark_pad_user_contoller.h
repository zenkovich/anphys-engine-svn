#ifndef ARK_PAD_USER_CONTOLLER_H
#define ARK_PAD_USER_CONTOLLER_H

#include "../input_messages.h"
#include "../ark_pad.h"

class ArkPadUserControllerBehavior:public ArkPadBehavior, public InputMessageListener
{
public:
	ArkPadUserControllerBehavior(ArkanoidPad* ownerPad):ArkPadBehavior(ownerPad) {}

	void update(float dt)
	{
		if (mInputMessage)
		{
			mOwnerPad->mPosition.x = mInputMessage->getCursorPos().x;
		}
	}
};

#endif //ARK_PAD_USER_CONTOLLER_H