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

	cArray<int> atata(100);

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

	
	if (mPhysicsRunning) 
	{
		getRenderStuff().reset();
		testConstraintSolve(dt, vec3(0.5f, 0.5f, 0.5f));
		testConstraintSolve(dt, vec3(-0.5f, 0.5f, 0.5f));
		//testConstraintSolve(dt, vec3(0.0f, 0.5f, -0.5f));
		mPhysics->update(dt);
		
		getRenderStuff().addRedArrow(vec3(0), vec3(10, 0, 0));
		getRenderStuff().addGreenArrow(vec3(0), vec3(0, 10, 0));
		getRenderStuff().addBlueArrow(vec3(0), vec3(0, 0, 10));
	}
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

void apTestFrame::testConstraintSolve( float dt, const vec3& attachPoint )
{
	//vec3 attachPoint(0.5f, 0.5f, 0.5f);
	vec3 sceneAttachPoint(0);
	float maxLength = 4.0f;

	if (dt < 0.000001) dt = 0.001f;

	phRigidObject* rigidObject = mTestBox->getPhysicsRigidBody();
	vec3 worldAttachPoint = attachPoint*rigidObject->mOrient + rigidObject->mPosition;

	vec3 diff = worldAttachPoint - sceneAttachPoint;
	float diffLength = diff.len();
	vec3 normDiff = diff/diffLength;

	vec3 pointSpeed = rigidObject->mVelocity + (rigidObject->mAngularVelocity^(worldAttachPoint - rigidObject->mPosition));

	float pointSpeedNormalProj = pointSpeed*normDiff;

	float pen = diffLength - maxLength;

	float fr = 0;
	if (pen > 0 && pointSpeedNormalProj > 0) fr = 1.0f;

	float force = fmax(0.0f, ((pointSpeedNormalProj + pen/dt*0.3f)*rigidObject->mMass*2.0f*0.7f)*fr )/2.0f;
	vec3 forceImpulse = normDiff*-force;

	*gLog << formatStr("speed before impulse %.2f %.2f %.2f (%.2f)\n", pointSpeed.x, pointSpeed.y, pointSpeed.z, pointSpeed.len());

	rigidObject->applyImpulse(worldAttachPoint, forceImpulse);

	pointSpeed = rigidObject->mVelocity + (rigidObject->mAngularVelocity^(worldAttachPoint - rigidObject->mPosition));
	*gLog << formatStr("speed after impulse %.2f %.2f %.2f (%.2f)\n", pointSpeed.x, pointSpeed.y, pointSpeed.z, pointSpeed.len());

	*gLog << formatStr("impulse %.3f %.3f %.3f (%.3f)\n", forceImpulse.x, forceImpulse.y, forceImpulse.z, forceImpulse);

	//dbg
	
	getRenderStuff().addRedCube(worldAttachPoint);
	getRenderStuff().addRedArrow(worldAttachPoint, worldAttachPoint + forceImpulse*0.1f);
	getRenderStuff().addBlueArrow(sceneAttachPoint, worldAttachPoint);
	getRenderStuff().addGreenArrow(worldAttachPoint, worldAttachPoint + pointSpeed*dt*2.0f);
}
