#include "shared_test.h"

OPEN_O2_NAMESPACE

A::A()
{
	b = share(B());
}


B::B()
{
	c = share(C());
}


C::C()
{
	a = NULL;
}

CLOSE_O2_NAMESPACE