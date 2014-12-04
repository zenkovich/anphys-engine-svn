#ifndef VERLET_PHYSICS_H
#define VERLET_PHYSICS_H

#include "public.h"
#include "util/containers/pool.h"

OPEN_O2_NAMESPACE

class VeretPhysics
{
public:
	struct Particle
	{
		vec2f mPosition;
		vec2f mLastPosition;
		float mRadius;

		Particle(const vec2f& position = vec2f());

		void integrate();
	};
	typedef array<Particle*> ParticlesArr;

	struct Link
	{
		Particle* mParticleA;
		Particle* mParticleB;
		float     mTargetDistance;
		float     mHardness;

		Link(Particle* particleA, Particle* particleB, float distance, float hardness = 1.0f);

		void resolve();
	};
	typedef array<Link*> LinksArr;

private:
	ParticlesArr   mPartices;
	LinksArr       mLinks;
	pool<Particle> mParticlesPool;
	pool<Link>     mLinksPool;

	vec2f          mGravity;
	float          mWorldFriction;

public:
	VeretPhysics();
	~VeretPhysics();

	void update(float dt);

	Particle* createParticle(const vec2f& position = vec2f());
	void removeParticle(Particle* particle);

	Link* createLink(Particle* particleA, Particle* particleB, float distance, float hardness = 1.0f);
	void removeLink(Link* link);

	void clear();
};

CLOSE_O2_NAMESPACE

#endif // VERLET_PHYSICS_H
