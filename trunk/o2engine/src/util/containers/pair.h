#ifndef PAIR_H
#define PAIR_H

#include "util/public_namespace.h"

OPEN_O2_NAMESPACE

template<typename _first_type, typename _second_type>
class pair
{
public:
	_first_type mFirst;
	_second_type mSecond;

public:
	pair() {}

	pair(const _first_type& first, const _second_type& second):
		mFirst(first), mSecond(second) {}

	pair(const pair& otherPair):
		mFirst(otherPair.mFirst), mSecond(otherPair.mSecond) {}

	bool operator==(const pair& otherPair) 
	{
		return mFirst == otherPair.mFirst && mSecond == otherPair.mSecond;
	}

	bool operator!=(const pair& otherPair) 
	{
		return mFirst != otherPair.mFirst || mSecond != otherPair.mSecond;
	}
};

CLOSE_O2_NAMESPACE

#endif // PAIR_H
