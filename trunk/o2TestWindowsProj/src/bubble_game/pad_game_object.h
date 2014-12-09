#ifndef PAD_GAME_OBJECT_H
#define PAD_GAME_OBJECT_H

#include "game_object.h"
#include "verlet_physics.h"

OPEN_O2_NAMESPACE

class grSprite;

class PadGameObject: public IGameObject, public VeretPhysics::CollisionListener
{
	float                      mWidth;
	float                      mLength;
	float                      mLinksHardness;
	float                      mParticlesHardness;

	grSprite*                  mParticleSprite;
	VeretPhysics::ParticlesArr mPhysicsPartices;
	VeretPhysics::LinksArr     mPhysicsLinks;

public:
	DEFINE_TYPE(PadGameObject);

	PadGameObject(const vec2f& position = vec2f(), float width = 6, float rotation = 0);
	~PadGameObject();

	void draw();
	void update(float dt);
	void setPhysicsLayer(int layer);

	SERIALIZBLE_INHERITED_METHODS(PadGameObject, IGameObject);

private:
	void onLoad();
	void onActivate();
	void onDeactivate();
	void initializePhysics();
	void deinitializePhysics();
};

CLOSE_O2_NAMESPACE

#endif // PAD_GAME_OBJECT_H
