#ifndef PAD_GAME_OBJECT_H
#define PAD_GAME_OBJECT_H

#include "game_object.h"
#include "verlet_physics.h"
#include "render_system/mesh.h"

OPEN_O2_NAMESPACE

class PadGameObject: public IGameObject
{
	grMesh*                    mMesh;
	VeretPhysics::ParticlesArr mPhysicsPartices;
	VeretPhysics::LinksArr     mPhysicsLinks;

public:
	PadGameObject();
	~PadGameObject();

	void draw();
	void update(float dt);
};

CLOSE_O2_NAMESPACE

#endif // PAD_GAME_OBJECT_H
