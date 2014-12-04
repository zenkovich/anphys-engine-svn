#include "verlet_physics.h"

#include "app/application.h"
#include "render_system/render_system.h"

OPEN_O2_NAMESPACE

DECLARE_SINGLETON(VeretPhysics);

VeretPhysics::Particle::Particle( const vec2f& position /*= vec2f()*/, float radius /*= 0.2f*/ ):
	mPosition(position), mLastPosition(position), mRadius(radius)
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
	mDeltaTime(1.0f/60.0f), mAccumulatedDt(0), mFloor(10.0f)
{
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
	for (int i = 0; i < mParticles.count(); i++)
	{
		Particle* pa = mParticles[i];
		for (int j = i + 1; j < mParticles.count(); j++)
		{
			Particle* pb = mParticles[j];

			vec2f dir = pb->mPosition - pa->mPosition;
			float sqrtDist = (dir).squareLength();
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
			}
		}

		if (pa->mPosition.y > mFloor)
		{
			pa->mPosition.y = mFloor;
			pa->mPosition.x += (pa->mPosition.x - pa->mLastPosition.x)*-0.4f;
		}
	}
}

VeretPhysics::Particle* VeretPhysics::createParticle( const vec2f& position /*= vec2f()*/, float radius /*= 0.2f*/ )
{
	Particle* res = mParticlesPool.take();
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

CLOSE_O2_NAMESPACE