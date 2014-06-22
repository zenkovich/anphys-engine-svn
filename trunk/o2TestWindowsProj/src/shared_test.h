#ifndef SHARED_TEST_H
#define SHARED_TEST_H

#include "public.h"

OPEN_O2_NAMESPACE
	
struct A;
struct B;
struct C;

struct A: public cShareObject
{
	shared<B> b;

	A();
};

struct B: public cShareObject
{
	shared<C> c;

	B();
};

struct C: public cShareObject
{
	shared<A> a;

	C();
};


CLOSE_O2_NAMESPACE

#endif // SHARED_TEST_H
