#ifndef PLAYER_BUBBLE_OBJECT_H
#define PLAYER_BUBBLE_OBJECT_H

#include "game_object.h"
#include "verlet_physics.h"
#include "render_system/mesh.h"

OPEN_O2_NAMESPACE

class PlayerBubble: public IGameObject, public VeretPhysics::CollisionListener
{
	float                      mRadius;
	vec2f                      mInputSense;
	int                        mSegmentsCount;
	float                      mRootHardness;
	float                      mPressureCoef;
	float                      mShellHardness;
	float                      mControlTime;

	grMesh*                    mMesh;

	VeretPhysics::Particle*    mPhysicsRootParticle;
	VeretPhysics::ParticlesArr mPhysicsShellParticles;
	VeretPhysics::LinksArr     mPhysicsLinks;

	float                      mInitialSquare;
	float                      mLastCollisionTime;

	int                        mScore;
	
public:
	PlayerBubble(const vec2f& position = vec2f());
	~PlayerBubble();

	void update(float dt);
	void draw();
	void setPhysicsLayer(int layer);

	int getScore() const;

protected:
	void onLoad();
	void onActivate();
	void onDeactivate();

	void onCollide(CollisionListener* other);

	void initPhysicsModel();
	void deinitPhysicsModel();
	void initGraphics();

	void updateControl(float dt);
	void updateVolumePressure(float dt);
	void updateStabilityEuristic(float dt);
	void updateMesh();

	void checkGameOver();

	float getSquare();
};

CLOSE_O2_NAMESPACE

#endif // PLAYER_BUBBLE_OBJECT_H
