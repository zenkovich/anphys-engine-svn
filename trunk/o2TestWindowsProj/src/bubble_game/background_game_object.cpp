#include "background_game_object.h"

#include "render_system/sprite.h"
#include "render_system/render_system.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(BackgroundGameObject);


BackgroundGameObject::BackgroundGameObject():
	IGameObject()
{
	const string bgTexturePath = "bubble_game/back";
	const string circleTexturePath = "bubble_game/circle";

	mBgSprite = mnew grSprite(grTexture::createFromFile(bgTexturePath));
	mBgSprite->scale = vec2f(0.07f, 0.07f);
}

BackgroundGameObject::~BackgroundGameObject()
{
	safe_release(mBgSprite);
	release_array(SpritesArr, mParallaxCirclesSprites);
}

void BackgroundGameObject::update(float dt)
{
	mBgSprite->position = renderSystem()->currentCamera()->mPosition;
}

void BackgroundGameObject::draw()
{
	mBgSprite->draw();
}

CLOSE_O2_NAMESPACE