#include "input_message.h"

#include "util/math/math.h"
#include "util/data_utils.h"
#include "util/time_utils.h"
#include "util/log.h"
#include "app/application.h"

#ifdef PLATFORM_WIN
#include <Windows.h>
#endif //PLATFORM_WIN

OPEN_O2_NAMESPACE
	
InputMessage::InputMessage():
	mMouseWheelDelta(0.0f)
{
}

bool InputMessage::isKeyPressed( VKey key ) const
{
	for (KeysVec::const_iterator it = mPressedKeys.cbegin(); it != mPressedKeys.cend(); it++)
		if (it->mKey == key) return true;

	return false;
}

bool InputMessage::isKeyDown( VKey key ) const
{
	for (KeysVec::const_iterator it = mDownKeys.cbegin(); it != mDownKeys.cend(); it++)
		if (it->mKey == key) return true;

	return isKeyPressed(key);
}

bool InputMessage::isKeyReleased( VKey key ) const
{
	for (KeysVec::const_iterator it = mReleasedKeys.cbegin(); it != mReleasedKeys.cend(); it++)
		if (it->mKey == key) return true;

	return false;
}

bool InputMessage::isKeyRepeating( VKey key, float delay /*= 0.1f*/, float beginDelay /*= 0.5f*/ ) const
{
	float time = getKeyPressingTime(key) - beginDelay;
	float repeatTime = max(floor(time/delay)*delay, 0.0f);

	return time - timeUtils()->getDeltaTime() < repeatTime && time > repeatTime;
}

float InputMessage::getKeyPressingTime( VKey key ) const
{
	for (KeysVec::const_iterator it = mDownKeys.cbegin(); it != mDownKeys.cend(); it++)
		if (it->mKey == key)
			return it->mPressedTime;

	return 0;
}

vec2f InputMessage::getCursorPos( int id /*= 0*/ ) const
{
	for (CursorVec::const_iterator it = mCursors.cbegin(); it != mCursors.cend(); ++it)
		if (it->mId == id) 
			return it->mPosition;

	if (id == 0)
		return mMainCursorPos;

	return vec2f();
}

bool InputMessage::isCursorPressed( int id /*= 0*/ ) const
{
	for (CursorVec::const_iterator it = mCursors.cbegin(); it != mCursors.cend(); ++it)
		if (it->mId == id && it->mPressedTime < FLT_EPSILON)
			return true;

	return false;
}

bool InputMessage::isCursorDown( int id /*= 0*/ ) const
{
	for (CursorVec::const_iterator it = mCursors.cbegin(); it != mCursors.cend(); ++it)
		if (it->mId == id)
			return true;

	return false;
}

bool InputMessage::isCursorReleased( int id /*= 0*/ ) const
{
	for (CursorVec::const_iterator it = mReleasedCursors.cbegin(); it != mReleasedCursors.cend(); ++it)
		if (it->mId == id)
			return true;

	return false;
}

float InputMessage::getCursorPressingTime( int id /*= 0*/ ) const
{
	for (CursorVec::const_iterator it = mCursors.cbegin(); it != mCursors.cend(); ++it)
		if (it->mId == id)
			return it->mPressedTime;

	return 0;
}

vec2f InputMessage::getCursorDelta( int id /*= 0*/ ) const
{
	for (CursorVec::const_iterator it = mCursors.cbegin(); it != mCursors.cend(); ++it)
		if (it->mId == id)
			return it->mDelta;

	if (id == 0)
		return mMainCursorDelta;

	return vec2f();
}

bool InputMessage::isAltCursorPressed() const
{
	return isKeyPressed((VKey)-1);
}

bool InputMessage::isAltCursorDown() const
{
	return isKeyDown((VKey)-1);
}

bool InputMessage::isAltCursorReleased() const
{
	return isKeyReleased((VKey)-1);
}

float InputMessage::getAltCursorPressedTime() const
{
	return getKeyPressingTime((VKey)-1);
}

bool InputMessage::isAlt2CursorPressed() const
{
	return isKeyPressed((VKey)-2);
}

bool InputMessage::isAlt2CursorDown() const
{
	return isKeyDown((VKey)-2);
}

bool InputMessage::isAlt2CursorReleased() const
{
	return isKeyReleased((VKey)-2);
}

float InputMessage::getAlt2CursorPressedTime() const
{
	return getKeyPressingTime((VKey)-2);
}

float InputMessage::getMouseWheelDelta() const
{
	return mMouseWheelDelta;
}

InputMessage::CursorVec const& InputMessage::getCursors() const
{
	return mCursors;
}

InputMessage::KeysVec const& InputMessage::getPressedKeys() const
{
	return mPressedKeys;
}

InputMessage::KeysVec const& InputMessage::getDownKeys() const
{
	return mDownKeys;
}

InputMessage::KeysVec const& InputMessage::getReleasedKeys() const
{
	return mReleasedKeys;
}


void InputMessage::keyPressed( VKey key )
{
	if (isKeyDown(key))
		return;

	mPressedKeys.push_back(key);
}

void InputMessage::keyReleased( VKey key )
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

int InputMessage::cursorPressed( const vec2f& pos )
{
	//find free id
	int id = 0;
	while(id < 100) 
	{
		bool success = true;
		for(CursorVec::iterator it = mCursors.begin(); it != mCursors.end(); ++it)
		{
			if (it->mId == id) 
			{
				success =false;
				break;
			}
		}

		if (success)
			break;
		else 
			id++;
	}

	mCursors.push_back(Cursor(pos, id));

	return id;
}

void InputMessage::cursorReleased( int id /*= 0*/ )
{
	Cursor releasedCuros(vec2f(), -100);
	for (CursorVec::iterator it = mCursors.begin(); it != mCursors.end(); it++)
	{
		if (it->mId == id)
		{
			releasedCuros = *it;
			mCursors.erase(it);
			break;
		}
	}
	mReleasedCursors.push_back(releasedCuros);
}

void InputMessage::setCursorPos( const vec2f& pos, int id /*= 0*/ )
{
	for (CursorVec::iterator it = mCursors.begin(); it != mCursors.end(); ++it)
	{
		if (it->mId == id) {
			it->mDelta = pos - it->mPosition;
			it->mPosition = pos;
			break;
		}
	}

	if (id == 0) 
	{
		mMainCursorDelta = pos - mMainCursorPos;
		mMainCursorPos = pos;
	}
}

void InputMessage::update(float dt)
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

	mMainCursorDelta = vec2f();

	mReleasedCursors.clear();

	mMouseWheelDelta = 0;
}

void InputMessage::altCursorPressed( const vec2f& pos )
{
	keyPressed(-1);
	setCursorPos(pos);
}

void InputMessage::altCursorReleased()
{
	keyReleased(-1);
}

void InputMessage::alt2CursorPressed( const vec2f& pos )
{
	keyPressed(-2);
	setCursorPos(pos);
}

void InputMessage::alt2CursorReleased()
{
	keyReleased(-2);
}

void InputMessage::setMouseWheelDelta( float delta )
{
	mMouseWheelDelta = delta;
}

CLOSE_O2_NAMESPACE