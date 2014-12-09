#include "background_game_object.h"

#include "render_system/sprite.h"
#include "render_system/render_system.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(BackgroundGameObject);


BackgroundGameObject::BackgroundGameObject():
	IGameObject()
{
}

BackgroundGameObject::~BackgroundGameObject()
{
	safe_release(mBgSprite);
	release_array(SpritesArr, mParallaxCirclesSprites);
}

void BackgroundGameObject::onLoad()
{
	const string bgTexturePath = "bubble_game/back";
	const string circleTexturePath = "bubble_game/circle";
	const int circlesCount = 50;

	mBgSprite = mnew grSprite(grTexture::createFromFile(bgTexturePath));
	mBgSprite->size = renderSystem()->getResolution();
	mBgSprite->relPivot = vec2f::one()*0.5f;
	
	vec2f rangeMax(15, 15);
	vec2f rangeMin(-15, -15);
	vec2f sizeRange(0.02f, 0.04f);
	for (int i = 0; i < circlesCount; i++)
	{
		float scale = random(sizeRange.x, sizeRange.y);
		grSprite* parallaxCirceSprite = mnew grSprite(grTexture::createFromFile(circleTexturePath));
		parallaxCirceSprite->position = vec2f(random(rangeMin.x, rangeMax.x), random(rangeMin.y, rangeMax.y));
		parallaxCirceSprite->color = color4(1.0f, 1.0f, 1.0f, 0.2f);
		parallaxCirceSprite->scale = vec2f::one()*scale;
		mParallaxCirclesSprites.add(parallaxCirceSprite);
	}
}

void BackgroundGameObject::update(float dt)
{
}

void BackgroundGameObject::draw()
{
	vec2f delta = renderSystem()->currentCamera()->mPosition - mBgSprite->position;

	mBgSprite->position = renderSystem()->currentCamera()->mPosition;
	mBgSprite->scale = renderSystem()->currentCamera()->mScale;
	mBgSprite->draw();

	foreach(SpritesArr, mParallaxCirclesSprites, sprIt)
	{
		(*sprIt)->position += delta*(0.9f + 0.15f*(float)sprIt.index()/(float)mParallaxCirclesSprites.count());
		(*sprIt)->draw();
	}
}

CLOSE_O2_NAMESPACE