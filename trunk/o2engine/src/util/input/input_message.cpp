#include "input_message.h"

#include <algorithm>

#include "util/math/math.h"

OPEN_O2_NAMESPACE

bool cInputMessage::isKeyPressed( VKey key ) const
{
	return std::find(mPressedKeys.begin(), mPressedKeys.end(), key) != mPressedKeys.end();
}

bool cInputMessage::isKeyDown( VKey key ) const
{
	return std::find(mDownKeys.begin(), mDownKeys.end(), key) != mDownKeys.end();
}

bool cInputMessage::isKeyReleased( VKey key ) const
{
	return std::find(mReleasedKeys.begin(), mReleasedKeys.end(), key) != mReleasedKeys.end();
}

const vec2f& cInputMessage::getCursorPos( int idx /*= 0*/ ) const
{
	return mCursorsPositions[clamp(idx, 0, (int)(mCursorsPositions.size() - 1))];
}

void cInputMessage::keyPressed( VKey key )
{
	mPressedKeys.push_back(key);
}

void cInputMessage::keyReleased( VKey key )
{
	KeysVec::iterator fnd = std::find(mDownKeys.begin(), mDownKeys.end(), key);
	if (fnd != mDownKeys.end())
		mDownKeys.erase(fnd);

	mReleasedKeys.push_back(key);
}

void cInputMessage::setCursorPos( const vec2f& pos, int idx /*= 0*/ )
{
	if (idx < (int)mCursorsPositions.size() - 1)
	{
		for (int i = 0; i < idx + 1; i++)
			mCursorsPositions.push_back(vec2f());
	}

	mCursorsPositions[0] = pos;
}

void cInputMessage::update()
{
	mDownKeys.insert(mDownKeys.end(), mPressedKeys.begin(), mPressedKeys.end());
	mPressedKeys.clear();

	mReleasedKeys.clear();
}

CLOSE_O2_NAMESPACE