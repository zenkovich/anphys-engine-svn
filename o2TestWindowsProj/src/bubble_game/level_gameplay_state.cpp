#include "level_gameplay_state.h"

#include "game_object.h"
#include "verlet_physics.h"
#include "player_bubble_object.h"
#include "app/application.h"
#include "render_system/render_system.h"
#include "pad_game_object.h"
#include "background_game_object.h"
#include "water_drop_game_object.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(LeveGameplayState);

LeveGameplayState::LeveGameplayState():
	mVerletPhysics(NULL)
{
	mVerletPhysics = mnew VeretPhysics();
	initializeObjects();
}

LeveGameplayState::~LeveGameplayState()
{
	release_array(GameObjectsArr, mGameObjects);
	safe_release(mVerletPhysics);
	safe_release(mCamera);
}

void LeveGameplayState::onActivate()
{
	renderSystem()->camera = mCamera;
}

void LeveGameplayState::onDeactivate()
{
	renderSystem()->camera = NULL;
}

void LeveGameplayState::update(float dt)
{
	foreach(GameObjectsArr, mGameObjects, objIt)
		if ((*objIt)->isActive())
			(*objIt)->update(dt);

	mVerletPhysics->update(dt);

	mCamera->mScale -= vec2f::one()*appInput()->getMouseWheelDelta()*0.0001f;	
	mCamera->mPosition = lerp(mCamera->mPosition, mPlayer->getPosition(), dt*3.0f);
}

void LeveGameplayState::draw()
{
	foreach(GameObjectsArr, mGameObjects, objIt)
		if ((*objIt)->isActive())
			(*objIt)->draw();

	//mVerletPhysics->dbgDraw();

	//renderSystem()->drawRectFrame(vec2f(), vec2f(10, 10));
}

void LeveGameplayState::initializeObjects()
{
	initPhysicsLayers();

	mCamera = mnew grCamera(vec2f(-10, 0), vec2f(0.03f, 0.03f));
	mCamera->mPivot = vec2f::one()*0.5f;
	
	mGameObjects.add(mnew BackgroundGameObject());

	mPlayer = mnew PlayerBubble();
	mPlayer->setPhysicsLayer(PL_PLAYER);
	mGameObjects.add(mPlayer);

	//test pads	
	addPadGameObject(vec2f(0.0f, 5.0f), 6, 30);
	addPadGameObject(vec2f(5.0f, 8.0f), 4, -30);
	addPadGameObject(vec2f(5.0f, 2.0f), 4, 0);
	addPadGameObject(vec2f(10.0f, -2.0f), 4, 0);
	addPadGameObject(vec2f(5.0f, 10.0f), 40, 0);

	addWaterDrop(vec2f(-3, 0));
}

void LeveGameplayState::initPhysicsLayers()
{
	bool layerMask[PL_MAX_LAYER_ID*PL_MAX_LAYER_ID] = { false, true,
	                                                    true, false };

	mVerletPhysics->setupLayerMask(layerMask, PL_MAX_LAYER_ID);
}

void LeveGameplayState::addPadGameObject( const vec2f& position, float width /*= 6.0f*/, float rotation /*= 0*/ )
{
	PadGameObject* pad = mnew PadGameObject(position, width, rotation);
	pad->setPhysicsLayer(PL_PAD);
	mGameObjects.add(pad);
}

void LeveGameplayState::addWaterDrop( const vec2f& position )
{
	WaterDropGameObject* drop = mnew WaterDropGameObject(position);
	drop->setPhysicsLayer(PL_PAD);
	mGameObjects.add(drop);
}

CLOSE_O2_NAMESPACE