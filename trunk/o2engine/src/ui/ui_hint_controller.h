#ifndef UI_HINT_CONTROLLER_H
#define UI_HINT_CONTROLLER_H

#include "public.h"

OPEN_O2_NAMESPACE

class uiHintController
{
	uiWidget* mWidget;
	uiLabel*  mLabel;

	string    mNextPlaceHintText;
	vec2f     mNextPlacePosition;

public:
	uiHintController();
	~uiHintController();
};

CLOSE_O2_NAMESPACE

#endif //UI_HINT_CONTROLLER_H
