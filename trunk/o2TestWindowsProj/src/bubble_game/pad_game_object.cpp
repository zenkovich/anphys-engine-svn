#include "pad_game_object.h"
#include "render_system\sprite.h"
#include "render_system\texture.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(PadGameObject);

PadGameObject::PadGameObject(const vec2f& position /*= vec2f()*/, float width /*= 6*/, float rotation /*= 0f*/):
	IGameObject(position, rotation), mParticleSprite(NULL), mWidth(0.3f), mLength(width), mLinksHardness(0.9f),
	mParticlesHardness(2.0f)
{	
}

PadGameObject::~PadGameObject()
{
	safe_release(mParticleSprite);
}

void PadGameObject::onLoad()
{
	const string texturePath = "bubble_game/circle";
	mParticleSprite = mnew grSprite(grTexture::createFromFile(texturePath));
	mParticleSprite->size = vec2f::one()*mWidth*2.5f;
	mParticleSprite->relPivot = vec2f::one()*0.5f;
	mParticleSprite->color = color4(1.0f, 1.0f, 1.0f, 0.7f);
}

void PadGameObject::draw()
{
	foreach(VeretPhysics::ParticlesArr, mPhysicsPartices, particleIt)
	{
		mParticleSprite->position = (*particleIt)->mPosition;
		mParticleSprite->draw();
	}
}

void PadGameObject::update(float dt)
{
	int n = mPhysicsPartices.count();
	for (int i = 0; i < n; i++)
	{
		float cf = (float)i/(float)(n - 1) - 0.5f;
		vec2f targetPos = transformLocalToWorld(vec2f::right()*cf*mLength);
		mPhysicsPartices[i]->mPosition = lerp(mPhysicsPartices[i]->mPosition, targetPos, dt*mParticlesHardness);
	}
}

void PadGameObject::initializePhysics()
{
	int partsCount = max((int)(mLength/mWidth), 1);

	for (int i = 0; i < partsCount; i++)
	{
		float cf = (float)i/(float)(partsCount - 1) - 0.5f;
		vec2f pos = transformLocalToWorld(vec2f::right()*cf*mLength);

		VeretPhysics::Particle* newParticle = verletPhysics()->createParticle(pos, mWidth);
		newParticle->mCollisionListener = this;
		mPhysicsPartices.add(newParticle);

		if (i > 0)
			mPhysicsLinks.add(verletPhysics()->createLink(mPhysicsPartices[i - 1], mPhysicsPartices[i], -1.0f, mLinksHardness));
	}
}

void PadGameObject::setPhysicsLayer( int layer )
{
	foreach(VeretPhysics::ParticlesArr, mPhysicsPartices, particleIt)
		(*particleIt)->mLayer = layer;
}

void PadGameObject::onActivate()
{
	initializePhysics();
}

void PadGameObject::onDeactivate()
{
	deinitializePhysics();
}

void PadGameObject::deinitializePhysics()
{
	foreach(VeretPhysics::ParticlesArr, mPhysicsPartices, particleIt)
		verletPhysics()->removeParticle(*particleIt);

	mPhysicsPartices.clear();
	
	foreach(VeretPhysics::LinksArr, mPhysicsLinks, linkIt)
		verletPhysics()->removeLink(*linkIt);

	mPhysicsLinks.clear();
}

SERIALIZE_INHERITED_METHOD_IMPL(PadGameObject)
{
	SERIALIZE_ID(mWidth, "width");
	SERIALIZE_ID(mLength, "length");

	return true;
}

CLOSE_O2_NAMESPACE