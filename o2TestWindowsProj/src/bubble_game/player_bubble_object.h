#ifndef PLAYER_BUBBLE_OBJECT_H
#define PLAYER_BUBBLE_OBJECT_H

#include "game_object.h"
#include "verlet_physics.h"
#include "render_system/mesh.h"

OPEN_O2_NAMESPACE

class PlayerBubble: public IGameObject
{
	float                      mRadius;
	float                      mInputSense;
	grMesh*                    mMesh;
	VeretPhysics::ParticlesArr mPhysicsPartices;
	VeretPhysics::LinksArr     mPhysicsLinks;
	
public:
	PlayerBubble();
	~PlayerBubble();

	void update(float dt);
	void draw();
};

CLOSE_O2_NAMESPACE

#endif // PLAYER_BUBBLE_OBJECT_H
