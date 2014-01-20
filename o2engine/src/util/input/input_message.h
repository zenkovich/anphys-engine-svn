#ifndef INPUT_MESSAGE_H
#define INPUT_MESSAGE_H

#include <vector>

#include "public.h"
#include "util/math/vector2.h"

OPEN_O2_NAMESPACE

/** Virtual key. */
typedef int VKey;

// multitouch cursors
#define VK_CURSOR_1 -1
#define VK_CURSOR_2 -2
#define VK_CURSOR_3 -3
#define VK_CURSOR_4 -4

/** Input message. Containing pressed, down, released keys, cursors positions. */
class cInputMessage
{
public:
	/** Cursor definition. */
	struct Cursor 
	{
		vec2f mPosition;
		vec2f mDelta;
		int   mId;
		float mPressedTime;

		Cursor(const vec2f& position, int id):mPosition(position), mDelta(), mId(id), mPressedTime(0) {}
	};
	typedef std::vector<Cursor> CursorVec;

protected:
	/** Key with pressed time. */
	struct Key
	{
		VKey  mKey;
		float mPressedTime;

		Key(VKey key = 0):mKey(key), mPressedTime(0) {}
	};

	typedef std::vector<Key>    KeysVec;

	KeysVec    mPressedKeys; 
	KeysVec    mDownKeys;
	KeysVec    mReleasedKeys;     

	CursorVec  mCursors; /**< Cursors positions. First - main cursor. */

public:
	/** Returns true if key was pressed at current frame. */
	bool isKeyPressed(VKey key) const;

	/** Returns true if key was pressed some frames ago. */
	bool isKeyDown(VKey key) const;

	/** Returns true, if key was released at current frame. */
	bool isKeyReleased(VKey key) const;

	/** Returns key pressing time.Returns  -1, if key not pressed. */
	float getKeyPressingTime(VKey key) const;

	/** Returns position of cursor. */
	vec2f getCursorPos(int id = 0) const;

	/** Returns cursor pressed time. -1 if cursor not pressed. */
	float getCursorPressingTime(int id = 0) const;

	/** Returns cursor delta. */
	vec2f getCursorDelta(int id = 0) const;

	/** Returns cursors vector. */
	CursorVec& getCursors();


	/** Call it when key pressed. */
	void keyPressed(VKey key);

	/** Call it when key released. */
	void keyReleased(VKey key);

	/** Call it when cursor pressed. */
	void cursorPressed(const vec2f& pos, int id = 0);

	/** Call it when cursor changed position. Id - index of cursor*/
	void setCursorPos(const vec2f& pos, int id = 0);

	/** Call it when cursor released. */
	void cursorReleased(int id = 0);

	/** Call it after frame update. */
	void update(float dt);
};

CLOSE_O2_NAMESPACE

#endif //INPUT_MESSAGE_H