#include "level_gameplay_state.h"

#include "game_object.h"
#include "verlet_physics.h"
#include "player_bubble_object.h"
#include "app/application.h"
#include "render_system/render_system.h"
#include "pad_game_object.h"
#include "background_game_object.h"

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
		(*objIt)->update(dt);

	mVerletPhysics->update(dt);

	//mCamera->mPosition = interpolate(mCamera->mPosition, mPlayer->position + vec2f(-5, -5), dt);
}

void LeveGameplayState::draw()
{
	mVerletPhysics->dbgDraw();

	foreach(GameObjectsArr, mGameObjects, objIt)
		(*objIt)->draw();
}

void LeveGameplayState::initializeObjects()
{
	mCamera = mnew grCamera(vec2f(-10, 0), vec2f(0.03f, 0.03f));
	
	mGameObjects.add(mnew BackgroundGameObject());

	mPlayer = mnew PlayerBubble();
	mGameObjects.add(mPlayer);

	//test pads	
	mGameObjects.add(mnew PadGameObject(vec2f(0.0f, 5.0f), 6, 30));
	mGameObjects.add(mnew PadGameObject(vec2f(5.0f, 8.0f), 4, -30));
}

void LeveGameplayState::addPlayerObject()
{
}

void LeveGameplayState::addTestPad()
{
}

CLOSE_O2_NAMESPACE