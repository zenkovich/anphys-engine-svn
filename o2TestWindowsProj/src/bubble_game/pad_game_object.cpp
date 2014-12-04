#include "pad_game_object.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(PadGameObject);

PadGameObject::PadGameObject(const vec2f& position /*= vec2f()*/, float width /*= 6*/, float rotation /*= 0f*/):
	IGameObject(position, rotation), mMesh(NULL), mWidth(0.6f), mLength(width), mLinksHardness(0.1f),
	mParticlesHardness(4.0f)
{
	initializePhysics();
}

PadGameObject::~PadGameObject()
{
	safe_release(mMesh);

	foreach(VeretPhysics::ParticlesArr, mPhysicsPartices, particleIt)
		verletPhysics()->removeParticle(*particleIt);
	
	foreach(VeretPhysics::LinksArr, mPhysicsLinks, linkIt)
		verletPhysics()->removeLink(*linkIt);
}

void PadGameObject::draw()
{

}

void PadGameObject::update(float dt)
{
	int n = mPhysicsPartices.count();
	for (int i = 0; i < n; i++)
	{
		float cf = (float)i/(float)(n - 1) - 0.5f;
		vec2f targetPos = transformLocalToWorld(vec2f::right()*cf*mLength);
		mPhysicsPartices[i]->mPosition = interpolate(mPhysicsPartices[i]->mPosition, targetPos, dt*mParticlesHardness);
	}
}

void PadGameObject::initializePhysics()
{
	int partsCount = max((int)(mLength/mWidth), 1);

	for (int i = 0; i < partsCount; i++)
	{
		float cf = (float)i/(float)(partsCount - 1) - 0.5f;
		vec2f pos = transformLocalToWorld(vec2f::right()*cf*mLength);

		mPhysicsPartices.add(verletPhysics()->createParticle(pos, mWidth*0.6f));
		if (i > 0)
			mPhysicsLinks.add(verletPhysics()->createLink(mPhysicsPartices[i - 1], mPhysicsPartices[i], -1.0f, mLinksHardness));
	}
}

CLOSE_O2_NAMESPACE