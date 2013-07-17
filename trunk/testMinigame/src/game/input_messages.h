#ifndef INPUT_MESSAGES_H
#define INPUT_MESSAGES_H

#include <vector>
#include "math/vector2.h"

class InputMessage
{
	friend class InputMessageDispatcher;

	typedef std::vector<char> KeysVec;

	KeysVec mPressedKeys;
	KeysVec mDownKeys;
	KeysVec mReleasedKeys;

	vec2f   mCursorPos;

public:
	bool isKeyPressed(char key) const;
	bool isKeyDown(char key) const;
	bool isKeyReleased(char key) const;

	vec2f getCursorPos() const;

private:
	void keyPresed(char key);
	void keyReleased(char key);
	void update();
};

class InputMessageListener
{
	friend class InputMessageDispatcher;

	InputMessage* mInputMessage;

	InputMessageListener():mInputMessage(NULL) {}
};

class InputMessageDispatcher
{
	typedef std::vector<InputMessageListener*> ListenersVec;

	InputMessage mCurrentMessage;
	ListenersVec mListeners;

public:
	InputMessageDispatcher();
	~InputMessageDispatcher();

	void registListener(InputMessageListener* listener);
	void unregistListener(InputMessageListener* listener);

	void updateMessage();

	void keyPressed(char key);
	void keyReleased(char key);
	void setCursorPos(const vec2f& cursorPos);
};

struct VKey
{
	enum { CURSOR_BTN     = -1, 
	       CURSOR_ALT_BTN = -2 };
};

#endif //INPUT_MESSAGES_H