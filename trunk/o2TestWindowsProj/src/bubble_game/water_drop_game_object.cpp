#include "water_drop_game_object.h"

#include "render_system/sprite.h"
#include "render_system/texture.h"
#include "render_system/render_system.h"
#include "verlet_physics.h"

OPEN_O2_NAMESPACE
	
REGIST_TYPE(WaterDropGameObject);

WaterDropGameObject::WaterDropGameObject( const vec2f& position ):
	IGameObject(position), mSprite(NULL), mRadius(0.3f), VeretPhysics::Collider(position, mRadius)
{
	mSprite = mnew grSprite(grTexture::createFromFile("bubble_game/circle"));
	mSprite->relPivot = vec2f::one()*0.5f;
	mSprite->position = IGameObject::mPosition;
	mSprite->scale = vec2f::one()*0.004f;
	mSprite->color = color4(0.7f, 0.2f, 1.0f, 0.7f);

	verletPhysics()->addCollider(this);
}

WaterDropGameObject::~WaterDropGameObject()
{
	verletPhysics()->removeCollider(this);
	safe_release(mSprite);
}

void WaterDropGameObject::update( float dt )
{
}

void WaterDropGameObject::draw()
{
	mSprite->draw();
}

void WaterDropGameObject::setPhysicsLayer( int layer )
{
	mLayer = layer;
}

void WaterDropGameObject::onDeactivate()
{
	verletPhysics()->removeCollider(this);
}

CLOSE_O2_NAMESPACE