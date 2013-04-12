#include "input_message.h"

#include <algorithm>

void cInputMessage::keyPressed( int key )
{
	if (isKeyDown(key)) return;
	mPressedKeys.push_back(key);
}

void cInputMessage::keyReleased( int key )
{
	IntArray::iterator fnd = std::find(mDownKeys.begin(), mDownKeys.end(), key);
	if (fnd != mDownKeys.end()) 
		mDownKeys.erase(fnd);

	mReleasedKeys.push_back(key);
}

void cInputMessage::update()
{
	for (IntArray::iterator it = mPressedKeys.begin(); it != mPressedKeys.end(); ++it)
		mDownKeys.push_back(*it);

	mPressedKeys.clear();
	mReleasedKeys.clear();
}

bool cInputMessage::isKeyPressed( int key ) const
{
	return std::find(mPressedKeys.cbegin(), mPressedKeys.cend(), key) != mPressedKeys.cend();
}

bool cInputMessage::isKeyDown( int key ) const
{
	return std::find(mDownKeys.cbegin(), mDownKeys.cend(), key) != mDownKeys.cend();
}

bool cInputMessage::isKeyReleased( int key ) const
{
	return std::find(mReleasedKeys.cbegin(), mReleasedKeys.cend(), key) != mReleasedKeys.cend();
}
