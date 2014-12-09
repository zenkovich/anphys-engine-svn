#ifndef VERLET_PHYSICS_H
#define VERLET_PHYSICS_H

#include "public.h"
#include "util/containers/pool.h"
#include "util/singleton.h"
#include "util/type_indexation.h"

OPEN_O2_NAMESPACE

class VeretPhysics: public cSingleton<VeretPhysics>
{
public:
	class CollisionListener
	{
		friend class VeretPhysics;
	public:
		DEFINE_TYPE(CollisionListener);

		virtual void onCollide(CollisionListener* other) {}
	};

	class Collider: public CollisionListener
	{
	public:
		DEFINE_TYPE(Collider);

		Collider(const vec2f& position = vec2f(), float radius = 1.0f, int layer = 0);

		vec2f  mPosition;
		float  mRadius;
		int    mLayer;
		
		virtual void onCollide(CollisionListener* other) {}
	};
	typedef array<Collider*> CollidersArr;

	struct Particle
	{
		vec2f              mPosition;
		vec2f              mLastPosition;
		float              mRadius;
		int                mLayer;
		CollisionListener* mCollisionListener;

		Particle(const vec2f& position = vec2f(), float radius = 0.2f, int layer = 0);

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
	struct CollisionInfo
	{
		CollisionListener* mListenerA;
		CollisionListener* mListenerB;

		CollisionInfo(CollisionListener* ca = NULL, CollisionListener* cb = NULL);
		bool operator==(const CollisionInfo& other) const;
	};
	typedef array<CollisionInfo> CollisionInfosArr;

private:
	ParticlesArr   mParticles;
	LinksArr       mLinks;
	CollidersArr   mColliders;
	pool<Particle> mParticlesPool;
	pool<Link>     mLinksPool;

	vec2f          mGravity;
	float          mWorldFriction;
	int            mSolveIterations;
	float          mDeltaTime;
	float*         mLayersMask;
	int            mMaxLayer;

	float          mAccumulatedDt;
	float          mFloor;

public:
	VeretPhysics();
	~VeretPhysics();

	void setupLayerMask(bool* maskArr, int maxLayer);

	void update(float dt);
	void dbgDraw();

	void addCollider(Collider* collider);
	void removeCollider(Collider* collider);

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
