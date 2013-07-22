#ifndef INPUT_MESSAGE_H
#define INPUT_MESSAGE_H

#include <vector>

#include "public.h"
#include "util/math/vector2.h"

OPEN_O2_NAMESPACE

typedef int VKey;

class cInputMessage
{
	typedef std::vector<VKey> KeysVec;
	typedef std::vector<vec2f> VecArr;

	KeysVec mPressedKeys;
	KeysVec mDownKeys;
	KeysVec mReleasedKeys;

	VecArr  mCursorsPositions;

public:
	bool isKeyPressed(VKey key) const;
	bool isKeyDown(VKey key) const;
	bool isKeyReleased(VKey key) const;

	const vec2f& getCursorPos(int idx = 0) const;

	void keyPressed(VKey key);
	void keyReleased(VKey key);
	void setCursorPos(const vec2f& pos, int idx = 0);

	void update();
};

CLOSE_O2_NAMESPACE

#endif //INPUT_MESSAGE_H