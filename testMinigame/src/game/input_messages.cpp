#include "input_messages.h"

#include <algorithm>

bool InputMessage::isKeyPressed( char key ) const
{
	return ( std::find(mPressedKeys.begin(), mPressedKeys.end(), key) != mPressedKeys.end() );
}

bool InputMessage::isKeyDown( char key ) const
{
	return ( std::find(mDownKeys.begin(), mDownKeys.end(), key) != mDownKeys.end() );
}

bool InputMessage::isKeyReleased( char key ) const
{
	return ( std::find(mReleasedKeys.begin(), mReleasedKeys.end(), key) != mReleasedKeys.end() );
}

vec2f InputMessage::getCursorPos() const
{
	return mCursorPos;
}

void InputMessage::keyPresed( char key )
{
	mPressedKeys.push_back(key);
}

void InputMessage::keyReleased( char key )
{
	mReleasedKeys.push_back(key);
}

void InputMessage::update()
{
	mDownKeys.insert(mDownKeys.begin(), mPressedKeys.begin(), mPressedKeys.end());
	mPressedKeys.clear();

	for (KeysVec::iterator it = mReleasedKeys.begin(); it != mReleasedKeys.end(); ++it)
	{
		KeysVec::iterator fnd = std::find(mDownKeys.begin(), mDownKeys.end(), *it);
		if (fnd != mDownKeys.end())
			mDownKeys.erase(fnd);
	}
	mReleasedKeys.clear();
}

InputMessageDispatcher::InputMessageDispatcher()
{
}

InputMessageDispatcher::~InputMessageDispatcher()
{
}

void InputMessageDispatcher::registListener( InputMessageListener* listener )
{
	listener->mInputMessage = &mCurrentMessage;
	mListeners.push_back(listener);
}

void InputMessageDispatcher::unregistListener( InputMessageListener* listener )
{
	ListenersVec::iterator fnd = std::find(mListeners.begin(), mListeners.end(), listener);
	if (fnd != mListeners.end())
	{
		(*fnd)->mInputMessage = NULL;
		mListeners.erase(fnd);
	}
}

void InputMessageDispatcher::updateMessage()
{
	mCurrentMessage.update();
}

void InputMessageDispatcher::keyPressed( char key )
{
	mCurrentMessage.keyPresed(key);
}

void InputMessageDispatcher::keyReleased( char key )
{
	mCurrentMessage.keyReleased(key);
}

void InputMessageDispatcher::setCursorPos( const vec2f& cursorPos )
{
	mCurrentMessage.mCursorPos = cursorPos;
}
