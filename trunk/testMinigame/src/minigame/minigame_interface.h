#ifndef MINIGAME_INTERFACE_H
#define MINIGAME_INTERFACE_H

class MiniGame
{
public:
	/** Virtual dtor. */
	virtual ~MiniGame() {};

	/** called before start. */
	virtual void initialize() = 0;

	/** called on mouse click ((x, y) - screen coordinates). */
	virtual void click(float x, float y) = 0;

	/** called when cursor moved ((x, y) - screen coordinates). */
	virtual void cursorMove(float x, float y) = 0;

	/** returns true then minigame successfully completed. */
	virtual bool isComplete() = 0;

	/** called on rendering. */
	virtual void render() = 0;

	/** called on updating. */
	virtual void update(float dt) = 0;
};

#endif //MINIGAME_INTERFACE_H