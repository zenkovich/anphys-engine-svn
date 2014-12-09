#include "water_drop_game_object.h"

#include "render_system/sprite.h"
#include "render_system/texture.h"
#include "render_system/render_system.h"
#include "verlet_physics.h"

OPEN_O2_NAMESPACE
	
REGIST_TYPE(WaterDropGameObject);

WaterDropGameObject::WaterDropGameObject( const vec2f& position ):
	IGameObject(position), mSprite(NULL), VeretPhysics::Collider(position, 0.3f)
{
}

WaterDropGameObject::~WaterDropGameObject()
{
	verletPhysics()->removeCollider(this);
	safe_release(mSprite);
}

void WaterDropGameObject::onLoad()
{
	mSprite = mnew grSprite(grTexture::createFromFile("bubble_game/circle"));
	mSprite->relPivot = vec2f::one()*0.5f;
	mSprite->position = IGameObject::mPosition;
	mSprite->scale = vec2f::one()*0.004f;
	mSprite->color = color4(0.7f, 0.2f, 1.0f, 0.7f);
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

void WaterDropGameObject::onActivate()
{
	verletPhysics()->addCollider(this);
}

void WaterDropGameObject::onDeactivate()
{
	verletPhysics()->removeCollider(this);
}

SERIALIZE_INHERITED_METHOD_IMPL(WaterDropGameObject)
{
	SERIALIZE_ID(mRadius, "radius");
	VeretPhysics::Collider::mPosition = IGameObject::mPosition;

	return true;
}

CLOSE_O2_NAMESPACE