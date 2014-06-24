#ifndef TEST_INTERFACE_H
#define TEST_INTERFACE_H

#include "public.h"

OPEN_O2_NAMESPACE

class ITest
{
public:
	virtual void draw() {}
	virtual void update(float dt) {}
};

CLOSE_O2_NAMESPACE

#endif // TEST_INTERFACE_H
