#ifndef GAME_STATE_H
#define GAME_STATE_H

class IGameState
{
	bool mLoaded;

public:
	IGameState():mLoaded(false) {}
	virtual ~IGameState() {}

	virtual void load()     { mLoaded = true; }
	virtual void unload()   { mLoaded = false; }
	virtual bool isLoaded() const { return mLoaded; }

	virtual void draw() {}
	virtual void update(float dt) {}

	virtual char* getName() const { return "noname"; }
};

#endif //GAME_STATE_H