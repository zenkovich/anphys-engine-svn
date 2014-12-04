#ifndef PAD_GAME_OBJECT_H
#define PAD_GAME_OBJECT_H

#include "game_object.h"
#include "verlet_physics.h"
#include "render_system/mesh.h"

OPEN_O2_NAMESPACE

class PadGameObject: public IGameObject
{
	float                      mWidth;
	float                      mLength;
	float                      mLinksHardness;
	float                      mParticlesHardness;

	grMesh*                    mMesh;
	VeretPhysics::ParticlesArr mPhysicsPartices;
	VeretPhysics::LinksArr     mPhysicsLinks;

public:
	DEFINE_TYPE(PadGameObject);

	PadGameObject(const vec2f& position = vec2f(), float width = 6, float rotation = 0);
	~PadGameObject();

	void draw();
	void update(float dt);

private:
	void initializePhysics();
};

CLOSE_O2_NAMESPACE

#endif // PAD_GAME_OBJECT_H
