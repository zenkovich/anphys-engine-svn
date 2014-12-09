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
	mVerletPhysics(NULL), mEditing(false)
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
}

void LeveGameplayState::onDeactivate()
{
	renderSystem()->camera = NULL;
}

void LeveGameplayState::update(float dt)
{
	if (appInput()->isKeyPressed('S'))
		saveLevel("level");

	if (appInput()->isKeyPressed('L'))
		loadLevel("level");

	if (appInput()->isKeyPressed('E'))
		mEditing = !mEditing;

// 	if (!appInput()->isKeyRepeating(VK_SPACE) && !appInput()->isKeyPressed(VK_SPACE))
// 		return;

	if (!mEditing)
		mCamera->mPosition = lerp(mCamera->mPosition, mPlayer->getPosition(), dt*3.0f);
	else
	{	
		if (appInput()->isKeyPressed('P'))
			addPadGameObject(mCamera->mPosition);
	
		if (appInput()->isKeyPressed('W'))
			addWaterDrop(mCamera->mPosition);
	
		mCamera->mScale -= vec2f::one()*appInput()->getMouseWheelDelta()*0.0001f;	
		if (appInput()->isCursorDown())
			mCamera->mPosition -= appInput()->getCursorDelta().scale(mCamera->mScale);
	}

	foreach(GameObjectsArr, mGameObjects, objIt)
		if ((*objIt)->isActive())
			(*objIt)->update(dt);

	mVerletPhysics->update(dt);
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
	renderSystem()->camera = mCamera;
	
	addObject(mnew BackgroundGameObject());

	mPlayer = mnew PlayerBubble();
	addObject(mPlayer);
	mPlayer->setPhysicsLayer(PL_PLAYER);
}

void LeveGameplayState::clearObjects()
{
	foreach(GameObjectsArr, mGameObjects, objIt)
		(*objIt)->onDeactivate();

	release_array(GameObjectsArr, mGameObjects);
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
	addObject(pad);
	pad->setPhysicsLayer(PL_PAD);
}

void LeveGameplayState::addWaterDrop( const vec2f& position )
{
	WaterDropGameObject* drop = mnew WaterDropGameObject(position);
	addObject(drop);
	drop->setPhysicsLayer(PL_PAD);
}

void LeveGameplayState::loadLevel(const string& filePath)
{
	clearObjects();	
	initializeObjects();

	GameObjectsArr newObjects;

	cSerializer serializer;
	if (!serializer.load(filePath))
	{
		logError("Failed to load level: %s", filePath.c_str());
		return;
	}

	serializer.serialize(newObjects, "objects");

	foreach(GameObjectsArr, newObjects, objIt)
	{
		addObject(*objIt);

		if ((*objIt)->getType() == PadGameObject::getStaticType())
			(*objIt)->setPhysicsLayer(PL_PAD);

		if ((*objIt)->getType() == WaterDropGameObject::getStaticType())
			(*objIt)->setPhysicsLayer(PL_PAD);
	}
}

void LeveGameplayState::saveLevel(const string& filePath)
{
	const int saveObjectsTypesCount = 2;
	UniqueType saveObjectsTypes[saveObjectsTypesCount] = { 
		PadGameObject::getStaticType(), WaterDropGameObject::getStaticType() };

	GameObjectsArr saveObjects;
	foreach(GameObjectsArr, mGameObjects, objIt)
	{
		UniqueType tp = (*objIt)->getType();
		for (int i = 0; i < saveObjectsTypesCount; i++)
		{
			if (tp == saveObjectsTypes[i])
			{
				saveObjects.add(*objIt);
				break;
			}
		}
	}

	cSerializer serializer;
	serializer.serialize(saveObjects, "objects");
	serializer.save(filePath);
}

void LeveGameplayState::addObject(IGameObject* object)
{
	object->onLoad();
	object->onActivate();
	mGameObjects.add(object);
}

CLOSE_O2_NAMESPACE