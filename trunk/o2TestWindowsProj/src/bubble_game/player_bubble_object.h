#ifndef PLAYER_BUBBLE_OBJECT_H
#define PLAYER_BUBBLE_OBJECT_H

#include "game_object.h"
#include "verlet_physics.h"
#include "render_system/mesh.h"

OPEN_O2_NAMESPACE

class PlayerBubble: public IGameObject
{
	float                      mRadius;
	vec2f                      mInputSense;
	int                        mSegmentsCount;
	float                      mRootHardness;
	float                      mPressureCoef;
	float                      mShellHardness;

	grMesh*                    mMesh;

	VeretPhysics::Particle*    mPhysicsRootParticle;
	VeretPhysics::ParticlesArr mPhysicsShellParticles;
	VeretPhysics::LinksArr     mPhysicsLinks;

	float                      mInitialSquare;
	
public:
	PlayerBubble(const vec2f& position = vec2f());
	~PlayerBubble();

	void update(float dt);
	void draw();

protected:
	void initPhysicsModel();
	void initGraphics();

	void updateControl(float dt);
	void updateVolumePressure(float dt);
	void updateStabilityEuristic(float dt);
	void updateMesh();

	float getSquare();
};

CLOSE_O2_NAMESPACE

#endif // PLAYER_BUBBLE_OBJECT_H