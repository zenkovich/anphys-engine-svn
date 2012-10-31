#include "test_frame.h"

#include "../../src/engine/engine_incl.h"
#include "../../src/util/debug/render_stuff.h"

apTestFrame::apTestFrame():apRenderWindow(), mMainEngineScene(NULL), mPhysicsRunning(true), mPhysicsRunByStep(false)
{
	mCamera3dMouse = static_cast<grCamera3DMouse*>(mRender->mCameras->addCamera(new grCamera3DMouse(vec2(0), mRender)));
	mRender->mCameras->setActiveCamera(mCamera3dMouse);

	onCreate(mInRect);
}

apTestFrame::apTestFrame(const std::string& title, fRect wndRect, fRect outputRect):
	apRenderWindow(title, wndRect, outputRect), mMainEngineScene(NULL), mPhysicsRunning(true), mPhysicsRunByStep(false)
{
	mCamera3dMouse = static_cast<grCamera3DMouse*>(mRender->mCameras->addCamera(new grCamera3DMouse(outputRect.getSize(), mRender)));
	mRender->mCameras->setActiveCamera(mCamera3dMouse);

	onCreate(mInRect);
}

apTestFrame::apTestFrame(const std::string& title, fRect wndRect):
	apRenderWindow(title, wndRect), mMainEngineScene(NULL), mPhysicsRunning(true), mPhysicsRunByStep(false)
{
	mCamera3dMouse = static_cast<grCamera3DMouse*>(mRender->mCameras->addCamera(new grCamera3DMouse(mInRect.getSize(), 
		                                                                                            mRender)));
	mRender->mCameras->setActiveCamera(mCamera3dMouse);

	onCreate(mInRect);
}

apTestFrame::~apTestFrame()
{
	mRender->mCameras->removeCamera(mCamera3dMouse);

	safe_release(mMainEngineScene);
}

void apTestFrame::onCreate(fRect inRect)
{
	//create main scene
	mMainEngineScene = new cScene(this);

	grLight* light = mRender->mLights->addLight(new grLight(NULL));
	light->initialize(grLightBaseInterface::light_directional_type, color4(1.0f, 1.0f, 1.0f, 1.0f), color4(1.0f, 1.0f, 1.0f, 1.0f),
		color4(0.5f,0.5f,0.5f,1.0f), vec3(0,0,0), vec3(0,-1,0), 0, 0, 0, 0, 0, 0, 0);
	light->setLightActive(true);

	mTestBox = mMainEngineScene->addObject(mMainEngineScene->mSceneStuff->createRigidWoodBox(vec3(0, 0, 0), vec3(1, 1, 1)));
	//mTestBox->getPhysicsRigidBody()->applyImpulse(vec3(0, 0, 1), vec3(0, 10, 0));

	mPhysicsRunning = false;
}

float apTestFrame::onTimer()
{
	float dt = apRenderWindow::onTimer();

	if (isKeyPressed(key_w)) mCamera3dMouse->moveForward(isKeyPressed(key_shift));
	if (isKeyPressed(key_s)) mCamera3dMouse->moveBack(isKeyPressed(key_shift));
	if (isKeyPressed(key_a)) mCamera3dMouse->moveLeft(isKeyPressed(key_shift));
	if (isKeyPressed(key_d)) mCamera3dMouse->moveRight(isKeyPressed(key_shift));
	if (isKeyPressed(key_ctrl)) mCamera3dMouse->moveDown(isKeyPressed(key_shift));
	if (isKeyPressed(key_space)) mCamera3dMouse->moveUp(isKeyPressed(key_shift));

	testConstraintSolve(dt);
	if (mPhysicsRunning) mPhysics->update(dt);
	mRender->update(dt);
	mMainEngineScene->update(dt);
	mRender->preRender();
	mRender->render();
	mRender->postRender();

	if (mPhysicsRunByStep) mPhysicsRunning = false;

	return dt;
}

void apTestFrame::onClose()
{
	mEngine->exitEngine();
}

void apTestFrame::onSize(fRect inRect)
{
	mCamera3dMouse->mScreenSize = inRect.getSize();
}

void apTestFrame::onMouseLeftButtonDown(vec2 point)
{
}

void apTestFrame::onMouseLeftButtonUp(vec2 point)
{
}

void apTestFrame::onMouseRightButtonDown(vec2 point)
{
}

void apTestFrame::onMouseRightButtonUp(vec2 point)
{
}

void apTestFrame::onMouseMove(vec2 point)
{
	if (mLeftMouseButton) mCamera3dMouse->mouseMove(point - mCursorPos);
}

void apTestFrame::onMouseWheel(float delta)
{
}

void apTestFrame::onKeyDown(int key)
{
	*gLog << formatStr("key = %i\n", key).c_str();
	
	if (isKeyPressed(key_t)) mPhysicsRunning = !mPhysicsRunning;
	if (isKeyPressed(key_y)) mPhysicsRunByStep = !mPhysicsRunByStep;
}

void apTestFrame::onKeyUp(int key)
{
}

void apTestFrame::onActive()
{
}

void apTestFrame::onDeActive()
{
}

void apTestFrame::testConstraintSolve( float dt )
{
	vec3 attachPoint(1, 1.5f, 0);
	vec3 sceneAttachPoint(0);
	float maxLength = 2.0f;

	if (dt < 0.000001) dt = 0.000001f;

	phRigidObject* rigidObject = mTestBox->getPhysicsRigidBody();
	vec3 worldAttachPoint = attachPoint*rigidObject->mOrient + rigidObject->mPosition;

	vec3 diff = worldAttachPoint - attachPoint;
	float diffLength = diff.len();

	float force = fmax(0.0f, diffLength - maxLength);
	vec3 forceImpulse = diff.normalize()*-force*0.7f*rigidObject->mMass;

	rigidObject->applyImpulse(worldAttachPoint, forceImpulse);

	*gLog << formatStr("impulse %.3f %.3f %.3f (%.3f)\n", forceImpulse.x, forceImpulse.y, forceImpulse.z, forceImpulse);

	//dbg
	
	getRenderStuff().addRedCube(worldAttachPoint);
}
