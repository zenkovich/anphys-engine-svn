#include "input_message.h"

#include <algorithm>

#include "util/math/math.h"
#include "util/log.h"

#ifdef PLATFORM_WIN
#include <Windows.h>
#endif //PLATFORM_WIN

OPEN_O2_NAMESPACE

bool cInputMessage::isKeyPressed( VKey key ) const
{
	for (KeysVec::const_iterator it = mPressedKeys.cbegin(); it != mPressedKeys.cend(); it++)
		if (it->mKey == key) return true;

	return false;
}

bool cInputMessage::isKeyDown( VKey key ) const
{
	for (KeysVec::const_iterator it = mDownKeys.cbegin(); it != mDownKeys.cend(); it++)
		if (it->mKey == key) return true;

	return false;
}

bool cInputMessage::isKeyReleased( VKey key ) const
{
	for (KeysVec::const_iterator it = mReleasedKeys.cbegin(); it != mReleasedKeys.cend(); it++)
		if (it->mKey == key) return true;

	return false;
}

vec2f cInputMessage::getCursorPos( int id /*= 0*/ ) const
{
	for (CursorVec::const_iterator it = mCursors.cbegin(); it != mCursors.cend(); ++it)
		if (it->mId == id) 
			return it->mPosition;

	return vec2f();
}

void cInputMessage::keyPressed( VKey key )
{
	if (isKeyDown(key))
		return;

	mPressedKeys.push_back(key);
}

void cInputMessage::keyReleased( VKey key )
{
	for (KeysVec::const_iterator it = mDownKeys.cbegin(); it != mDownKeys.cend(); it++)
	{
		if (it->mKey == key)
		{
			mDownKeys.erase(it);
			break;
		}
	}
	mReleasedKeys.push_back(key);
}

void cInputMessage::setCursorPos( const vec2f& pos, int id /*= 0*/ )
{
	for (CursorVec::iterator it = mCursors.begin(); it != mCursors.end(); ++it)
		if (it->mId == id) {
			it->mDelta = pos - it->mPosition;
			it->mPosition = pos;
			break;
		}
}

void cInputMessage::update(float dt)
{
	mDownKeys.insert(mDownKeys.end(), mPressedKeys.begin(), mPressedKeys.end());
	mPressedKeys.clear();
	mReleasedKeys.clear();

	for (KeysVec::iterator it = mDownKeys.begin(); it != mDownKeys.end(); it++)
		(*it).mPressedTime += dt;

	for (CursorVec::iterator it = mCursors.begin(); it != mCursors.end(); ++it) 
	{
		(*it).mPressedTime += dt;
		it->mDelta = vec2f();
	}
}

float cInputMessage::getKeyPressingTime( VKey key ) const
{
	for (KeysVec::const_iterator it = mReleasedKeys.cbegin(); it != mReleasedKeys.cend(); it++)
		if (it->mKey == key)
			return it->mPressedTime;

	return -1;
}

float cInputMessage::getCursorPressingTime( int id /*= 0*/ ) const
{
	for (CursorVec::const_iterator it = mCursors.cbegin(); it != mCursors.cend(); ++it)
		if (it->mId == id)
			return it->mPressedTime;

	return -1;
}

vec2f cInputMessage::getCursorDelta( int id /*= 0*/ ) const
{
	for (CursorVec::const_iterator it = mCursors.cbegin(); it != mCursors.cend(); ++it)
		if (it->mId == id)
			return it->mDelta;

	return vec2f();
}

cInputMessage::CursorVec& cInputMessage::getCursors()
{
	return mCursors;
}

void cInputMessage::cursorPressed( const vec2f& pos, int id /*= 0*/ )
{
	int cursorKeys[] = { VK_LBUTTON, VK_CURSOR_1, VK_CURSOR_2, VK_CURSOR_3, VK_CURSOR_4, VK_RBUTTON, VK_MBUTTON };
	keyPressed(cursorKeys[id]);

	mCursors.push_back(Cursor(pos, id));
}

void cInputMessage::cursorReleased( int id /*= 0*/ )
{
	for (CursorVec::iterator it = mCursors.begin(); it != mCursors.end(); ++it)
	{
		if (it->mId == id)
		{
			mCursors.erase(it);
			break;
		}
	}
}

CLOSE_O2_NAMESPACE