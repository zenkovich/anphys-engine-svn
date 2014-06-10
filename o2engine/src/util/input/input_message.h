#ifndef INPUT_MESSAGE_H
#define INPUT_MESSAGE_H

#include "public.h"
#include "util/math/vector2.h"

OPEN_O2_NAMESPACE

/** Virtual key. */
typedef int VKey;

/** Input message. Containing pressed, down, released keys, cursors positions. */
class cInputMessage: public cShareObject
{
	friend class cApplicationBaseInterface;
	friend class cApplication;
	friend class InputMsgTest;

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
	typedef vector<Cursor> CursorVec;

protected:
	/** Key with pressed time. */
	struct Key
	{
		VKey  mKey;
		float mPressedTime;

		Key(VKey key = 0):mKey(key), mPressedTime(0) {}
	};

	typedef vector<Key>    KeysVec;

	KeysVec   mPressedKeys; 
	KeysVec   mDownKeys;
	KeysVec   mReleasedKeys;     
	
	CursorVec mCursors; /**< Cursors positions. First - main cursor. */
	CursorVec mReleasedCursors; /**< Cursors positions. First - main cursor. */
	vec2f     mMainCursorPos;
	vec2f     mMainCursorDelta;

public:
	/** Returns true if key was pressed at current frame. */
	bool isKeyPressed(VKey key) const;

	/** Returns true if key was pressed some frames ago. */
	bool isKeyDown(VKey key) const;

	/** Returns true, if key was released at current frame. */
	bool isKeyReleased(VKey key) const;

	/** Returns key pressing time.Returns  0, if key not pressed. */
	float getKeyPressingTime(VKey key) const;

	/** Returns position of cursor. */
	vec2f getCursorPos(int id = 0) const;

	/** Returns true, when cursor pressed at current frame. */
	bool isCursorPressed(int id = 0) const;

	/** Returns true, when cursor is down. */
	bool isCursorDown(int id = 0) const;

	/** Returns true, when cursor released at current frame. */
	bool isCursorReleased(int id = 0) const;

	/** Returns cursor pressed time. 0 if cursor not pressed. */
	float getCursorPressingTime(int id = 0) const;

	/** Returns cursor delta. */
	vec2f getCursorDelta(int id = 0) const;

	/** Returns true, when alt cursor pressed (right mouse button). */
	bool isAltCursorPressed() const;
	
	/** Returns true, when alt cursor down (right mouse button). */
	bool isAltCursorDown() const;
	
	/** Returns true, when alt cursor released (right mouse button). */
	bool isAltCursorReleased() const;
	
	/** Returns alt cursor pressed time (right mouse button). 0 if cursor not pressed. */
	float getAltCursorPressedTime() const;

	/** Returns true, when alt 2 cursor pressed (medium mouse button). */
	bool isAlt2CursorPressed() const;
	
	/** Returns true, when alt 2 cursor down (medium mouse button). */
	bool isAlt2CursorDown() const;
	
	/** Returns true, when alt 2 cursor released (medium mouse button). */
	bool isAlt2CursorReleased() const;
	
	/** Returns alt 2 cursor pressed time (medium mouse button). 0 if cursor not pressed. */
	float getAlt2CursorPressedTime() const;

	/** Returns cursors vector. */
	CursorVec const& getCursors() const;

protected:
	/** Call it when key pressed. */
	void keyPressed(VKey key);

	/** Call it when key released. */
	void keyReleased(VKey key);

	/** Call it when cursor pressed. */
	int cursorPressed(const vec2f& pos);

	/** Call it when cursor changed position. Id - index of cursor*/
	void setCursorPos(const vec2f& pos, int id = 0);

	/** Call it when cursor released. */
	void cursorReleased(int id = 0);

	/** Call it when alt cursor pressed (right mouse button). */
	void altCursorPressed(const vec2f& pos);

	/** Call it when alt cursor released (right mouse button). */
	void altCursorReleased();

	/** Call it when alt 2 cursor pressed (medium mouse button). */
	void alt2CursorPressed(const vec2f& pos);

	/** Call it when alt 2cursor released (medium mouse button). */
	void alt2CursorReleased();

	/** Call it after frame update. */
	void update(float dt);
};

CLOSE_O2_NAMESPACE

#endif //INPUT_MESSAGE_H