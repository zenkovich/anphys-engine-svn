#include "verlet_physics.h"

#include "app/application.h"
#include "render_system/render_system.h"

OPEN_O2_NAMESPACE

DECLARE_SINGLETON(VeretPhysics);

typedef VeretPhysics::CollisionListener VeretPhysicsCollisionListener;

UniqueType VeretPhysics::CollisionListener::CollisionListenerType = TypeIdxContainer::index++;
UniqueType VeretPhysics::Collider::ColliderType = TypeIdxContainer::index++;

VeretPhysics::Collider::Collider( const vec2f& position /*= vec2f()*/, float radius /*= 1.0f*/, int layer /*= 0*/ ):
	mPosition(position), mRadius(radius), mLayer(layer)
{
}


VeretPhysics::Particle::Particle( const vec2f& position /*= vec2f()*/, float radius /*= 0.2f*/, int layer /*= 0*/ ):
	mPosition(position), mLastPosition(position), mRadius(radius), mLayer(layer), mCollisionListener(NULL)
{
}

void VeretPhysics::Particle::integrate(float friction)
{
	vec2f delta = mPosition - mLastPosition;
	mLastPosition = mPosition;
	mPosition += delta*friction;
}


VeretPhysics::Link::Link():
	mParticleA(NULL), mParticleB(NULL), mTargetDistance(0.0f), mHardness(1.0f)
{
}

VeretPhysics::Link::Link( Particle* particleA, Particle* particleB, float distance, float hardness /*= 1.0f*/ ):
	mParticleA(particleA), mParticleB(particleB), mTargetDistance(distance), mHardness(hardness)
{
}

void VeretPhysics::Link::resolve()
{	
	vec2f point1 = mParticleA->mPosition;
	vec2f point2 = mParticleB->mPosition;

	vec2f norm = (point2 - point1).normalize();

	vec2f goal1 = (point1 + point2) * 0.5f - norm * mTargetDistance * 0.5f;
	vec2f goal2 = (point1 + point2) * 0.5f + norm * mTargetDistance * 0.5f;

	mParticleA->mPosition = mParticleA->mPosition + (goal1 - mParticleA->mPosition) * mHardness;
	mParticleB->mPosition = mParticleB->mPosition + (goal2 - mParticleB->mPosition) * mHardness;
}


VeretPhysics::VeretPhysics():
	mGravity(0, 9.8f), mParticlesPool(100, 20), mLinksPool(100, 20), mWorldFriction(0.6f), mSolveIterations(10),
	mDeltaTime(1.0f/60.0f), mAccumulatedDt(0), mFloor(20.0f), mLayersMask(NULL), mMaxLayer(0)
{
	bool initLayerMask[1] = { true };
	setupLayerMask(initLayerMask, 1);
}

VeretPhysics::~VeretPhysics()
{
	release_array(ParticlesArr, mParticles);
	release_array(LinksArr, mLinks);
}

void VeretPhysics::update( float dt )
{
	mAccumulatedDt += dt;
	while(mAccumulatedDt > mDeltaTime) 
	{
		mAccumulatedDt -= mDeltaTime;
		updateFixedDeltaTime();
	}
}

void VeretPhysics::updateFixedDeltaTime()
{
	vec2f gravityOffset = mGravity*mDeltaTime*mDeltaTime;
	float friction = 1.0f - mWorldFriction*mDeltaTime;
	foreach(ParticlesArr, mParticles, particleIt)
	{
		if ((*particleIt)->mLayer > mMaxLayer - 1)
			(*particleIt)->mLayer = mMaxLayer - 1;

		(*particleIt)->mPosition += gravityOffset;
		(*particleIt)->integrate(friction);
	}

	checkCollisions();

	for (int i = 0; i < mSolveIterations; i++)
	{
		foreach(LinksArr, mLinks, linkIt)
			(*linkIt)->resolve();
	}
}

void VeretPhysics::checkCollisions()
{
	static CollisionInfosArr collisions;
	collisions.clear();

	for (int i = 0; i < mParticles.count(); i++)
	{
		Particle* pa = mParticles[i];
		for (int j = i + 1; j < mParticles.count(); j++)
		{
			Particle* pb = mParticles[j];

			vec2f dir = pb->mPosition - pa->mPosition;
			float sqrtDist = (dir).squareLength() + mLayersMask[pa->mLayer*mMaxLayer + pb->mLayer];
			float radSumm = pa->mRadius + pb->mRadius;
			if (sqrtDist < radSumm*radSumm)
			{
				float dist = sqrt(sqrtDist);
				vec2f norm = dir/dist;
				float depth = radSumm - dist;

				vec2f fnorm(-norm.y, norm.x);
				vec2f dv = (pb->mPosition - pb->mLastPosition) - (pa->mPosition - pa->mLastPosition);
				
				vec2f d = norm*depth*0.5f - fnorm*(dv*fnorm)*0.4f;

				pa->mPosition -= d;
				pb->mPosition += d;

				if (pa->mCollisionListener && pb->mCollisionListener)
					collisions.add(CollisionInfo(pa->mCollisionListener, pb->mCollisionListener));
			}
		}

		foreach(CollidersArr, mColliders, colliderIt)
		{
			vec2f dir = (*colliderIt)->mPosition - pa->mPosition;
			float sqrtDist = (dir).squareLength() + mLayersMask[pa->mLayer*mMaxLayer + (*colliderIt)->mLayer];
			float radSumm = pa->mRadius + (*colliderIt)->mRadius;
			if (sqrtDist < radSumm*radSumm)
			{
				if (pa->mCollisionListener)
					collisions.add(CollisionInfo(pa->mCollisionListener, *colliderIt));
			}
		}

		if (pa->mPosition.y > mFloor)
		{
			pa->mPosition.y = mFloor;
			pa->mPosition.x += (pa->mPosition.x - pa->mLastPosition.x)*-0.4f;
		}
	}

	foreach(CollisionInfosArr, collisions, collIt)
	{
		collIt->mListenerA->onCollide(collIt->mListenerB);
		collIt->mListenerB->onCollide(collIt->mListenerA);
	}
}

VeretPhysics::Particle* VeretPhysics::createParticle( const vec2f& position /*= vec2f()*/, float radius /*= 0.2f*/ )
{
	Particle* res = mParticlesPool.take();
	*res = Particle();
	res->mPosition = position;
	res->mLastPosition = position;
	res->mRadius = radius;
	mParticles.add(res);
	mParticles.sort();
	return res;
}

void VeretPhysics::removeParticle( Particle* particle )
{
	mParticles.remove(particle);
	mParticlesPool.free(particle);
}

VeretPhysics::Link* VeretPhysics::createLink( Particle* particleA, Particle* particleB, float distance /*= -1.0f*/, 
	                                          float hardness /*= 1.0f*/ )
{
	Link* res = mLinksPool.take();
	*res = Link();

	res->mParticleA = particleA;
	res->mParticleB = particleB;
	res->mHardness = hardness;

	if (distance > 0)
		res->mTargetDistance = distance;
	else
		res->mTargetDistance = (particleA->mPosition - particleB->mPosition).length();


	mLinks.add(res);
	mLinks.sort();
	return res;
}

void VeretPhysics::removeLink( Link* link )
{
	mLinks.remove(link);
	mLinksPool.free(link);
}

void VeretPhysics::clear()
{
	foreach(ParticlesArr, mParticles, particleIt)
		mParticlesPool.free(*particleIt);

	mParticles.clear();

	foreach(LinksArr, mLinks, linkIt)
		mLinksPool.free(*linkIt);

	mLinks.clear();
}

void VeretPhysics::dbgDraw()
{
	foreach(LinksArr, mLinks, linkIt)
		renderSystem()->drawLine((*linkIt)->mParticleA->mPosition, (*linkIt)->mParticleB->mPosition);

	foreach(ParticlesArr, mParticles, particleIt)
		renderSystem()->drawCircle((*particleIt)->mPosition, (*particleIt)->mRadius);
}

void VeretPhysics::setupLayerMask( bool* maskArr, int maxLayer )
{
	safe_release_arr(mLayersMask);
	mMaxLayer = maxLayer;
	mLayersMask = mnew float[maxLayer*maxLayer];
	for (int i = 0; i < maxLayer*maxLayer; i++)
		mLayersMask[i] = maskArr[i] ? 0.0f:1000000000000.0f;
}

void VeretPhysics::addCollider( Collider* collider )
{
	mColliders.add(collider);
}

void VeretPhysics::removeCollider( Collider* collider )
{
	mColliders.remove(collider);
}


VeretPhysics::CollisionInfo::CollisionInfo( CollisionListener* ca /*= NULL*/, CollisionListener* cb /*= NULL*/ ):
	mListenerA(ca), mListenerB(cb)
{
}

bool VeretPhysics::CollisionInfo::operator==( const CollisionInfo& other ) const
{
	return mListenerA == other.mListenerA && mListenerB == other.mListenerB;
}

CLOSE_O2_NAMESPACE