#ifndef INPUT_MESSAGE_H
#define INPUT_MESSAGE_H

#include <vector>

#include "util/math/mmath.h"

#define CURSOR_BUTTON -1
#define RM_BUTTON     -2

struct cInputMessage
{
	typedef std::vector<int> IntArray;

	IntArray mPressedKeys;
	IntArray mDownKeys;
	IntArray mReleasedKeys;

	vec2     mCursorPosition;

	void keyPressed(int key);
	void keyReleased(int key);

	void update();

	bool isKeyPressed(int key) const;
	bool isKeyDown(int key) const;
	bool isKeyReleased(int key) const;
};

#endif //INPUT_MESSAGE_H