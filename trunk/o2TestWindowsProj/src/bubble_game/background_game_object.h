#ifndef BACKGROUND_GAME_OBJECT_H
#define BACKGROUND_GAME_OBJECT_H

#include "game_object.h"

OPEN_O2_NAMESPACE

class grSprite;

class BackgroundGameObject: public IGameObject
{
	typedef array<grSprite*> SpritesArr;

	grSprite*  mBgSprite;
	SpritesArr mParallaxCirclesSprites;

public:
	DEFINE_TYPE(BackgroundGameObject);

	BackgroundGameObject();
	~BackgroundGameObject();

	void update(float dt);
	void draw();

protected:
	void onLoad();
};

CLOSE_O2_NAMESPACE

#endif // BACKGROUND_GAME_OBJECT_H
