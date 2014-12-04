#ifndef VERLET_PHYSICS_H
#define VERLET_PHYSICS_H

#include "public.h"
#include "util/containers/pool.h"
#include "util/singleton.h"

OPEN_O2_NAMESPACE

class VeretPhysics: public cSingleton<VeretPhysics>
{
public:
	struct Particle
	{
		vec2f mPosition;
		vec2f mLastPosition;
		float mRadius;

		Particle(const vec2f& position = vec2f(), float radius = 0.2f);

		void integrate(float friction);
	};
	typedef array<Particle*> ParticlesArr;

	struct Link
	{
		Particle* mParticleA;
		Particle* mParticleB;
		float     mTargetDistance;
		float     mHardness;

		Link();
		Link(Particle* particleA, Particle* particleB, float distance, float hardness = 1.0f);

		void resolve();
	};
	typedef array<Link*> LinksArr;

private:
	ParticlesArr   mParticles;
	LinksArr       mLinks;
	pool<Particle> mParticlesPool;
	pool<Link>     mLinksPool;

	vec2f          mGravity;
	float          mWorldFriction;
	int            mSolveIterations;
	float          mDeltaTime;

	float          mAccumulatedDt;
	float          mFloor;

public:
	VeretPhysics();
	~VeretPhysics();

	void update(float dt);
	void dbgDraw();

	Particle* createParticle(const vec2f& position = vec2f(), float radius = 0.2f);
	void removeParticle(Particle* particle);

	Link* createLink(Particle* particleA, Particle* particleB, float distance = -1.0f, float hardness = 1.0f);
	void removeLink(Link* link);

	void clear();

public:
	void updateFixedDeltaTime();
	void checkCollisions();
};

#define verletPhysics() VeretPhysics::instancePtr()

CLOSE_O2_NAMESPACE

#endif // VERLET_PHYSICS_H
