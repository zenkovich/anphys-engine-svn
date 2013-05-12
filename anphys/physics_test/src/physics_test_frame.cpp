#include "physics_test_frame.h"

#include "engine/engine_incl.h"
#include "util/debug/render_stuff.h"


apPhysicsTestFrame::apPhysicsTestFrame():apRenderWindow(), mMainEngineScene(NULL), mPhysicsRunning(true), mPhysicsRunByStep(false)
{
	onCreate(mInRect);
}

apPhysicsTestFrame::apPhysicsTestFrame(const std::string& title, fRect wndRect, fRect outputRect):
	apRenderWindow(title, wndRect, outputRect), mMainEngineScene(NULL), mPhysicsRunning(true), mPhysicsRunByStep(false)
{
	onCreate(mInRect);
}

apPhysicsTestFrame::apPhysicsTestFrame(const std::string& title, fRect wndRect):
	apRenderWindow(title, wndRect), mMainEngineScene(NULL), mPhysicsRunning(true), mPhysicsRunByStep(false)
{
	onCreate(mInRect);
}

apPhysicsTestFrame::~apPhysicsTestFrame()
{
	safe_release(m2DRenderState);
	safe_release(m3DRenderState);
	safe_release(mCamera3dMouse);
	safe_release(m2DCamera);
	safe_release(mMainEngineScene);
	safe_release(mWidgetsManager);
}

void apPhysicsTestFrame::onCreate(fRect inRect)
{
	mCamera3dMouse = new grCamera3DMouse;
	m2DCamera = new grCamera2D;
	m3DRenderState = new grSimple3DRenderState(mRender);
	m2DRenderState = new gr2DRenderState(mRender);

	m3DRenderState->bindCamera(mCamera3dMouse);
	m2DRenderState->bindCamera(m2DCamera);

	//create main scene
	mMainEngineScene = new cScene(this);

	grLight* light = mRender->mLights->addLight(new grLight(NULL));
	light->initialize(grLightBaseInterface::light_directional_type, color4(0.5f, 0.5f, 0.5f, 1.0f), 
		color4(1.0f, 1.0f, 1.0f, 1.0f),
		color4(0.5f,0.5f,0.5f,1.0f), vec3(0,0,0), vec3(0,-1,1), 0, 0, 0, 0, 0, 0, 0);
	//light->setLightActive(true);

	setupScene1();

	createUIWidgets();

	mPhysicsRunning = false;
}

float apPhysicsTestFrame::onTimer()
{
	float dt = apRenderWindow::onTimer();

	if (isKeyDown(key_w)) mCamera3dMouse->moveForward(isKeyDown(key_shift));
	if (isKeyDown(key_s)) mCamera3dMouse->moveBack(isKeyDown(key_shift));
	if (isKeyDown(key_a)) mCamera3dMouse->moveLeft(isKeyDown(key_shift));
	if (isKeyDown(key_d)) mCamera3dMouse->moveRight(isKeyDown(key_shift));
	if (isKeyDown(key_ctrl)) mCamera3dMouse->moveDown(isKeyDown(key_shift));
	if (isKeyDown(key_space)) mCamera3dMouse->moveUp(isKeyDown(key_shift));
	
	if (mPhysicsRunning) 
	{
		getRenderStuff().reset();	
		mMainEngineScene->mPhysicsScene->update(dt);
	}

	mInputMessenger->sendInputMessage();
	mInputMessenger->mInputMessage.update();

	mMainEngineScene->update(dt);
	mRender->update(dt);
	mCamera3dMouse->update(dt);
	mWidgetsManager->update(dt);

	mRender->beginRender();

	mRender->bindRenderState(m3DRenderState);

	mRender->render();

	mRender->bindRenderState(m2DRenderState);

	render2D();

	mRender->endRender();

	if (mPhysicsRunByStep) mPhysicsRunning = false;

	return dt;
}

void apPhysicsTestFrame::onClose()
{
	mEngine->exitEngine();
}

void apPhysicsTestFrame::onSize(fRect inRect)
{
	//mCamera3dMouse->mScreenSize = inRect.getSize();
	mRender->resize(inRect.getSize());
}

void apPhysicsTestFrame::onMouseLeftButtonDown(vec2 point)
{
}

void apPhysicsTestFrame::onMouseLeftButtonUp(vec2 point)
{
}

void apPhysicsTestFrame::onMouseRightButtonDown(vec2 point)
{
}

void apPhysicsTestFrame::onMouseRightButtonUp(vec2 point)
{
}

void apPhysicsTestFrame::onMouseMove(vec2 point)
{
	if (mInputMessenger->mInputMessage.isKeyDown(CURSOR_BUTTON))
		mCamera3dMouse->mouseMoved(point - mInputMessenger->mInputMessage.mCursorPosition);
}

void apPhysicsTestFrame::onMouseWheel(float delta)
{
}

void apPhysicsTestFrame::onKeyDown(int key)
{
	*gLog << formatStr("key = %i\n", key).c_str();
	
	if (key == key_t) 
	{
		mPhysicsRunning = !mPhysicsRunning;
	}
	if (key == key_y)
	{
		mPhysicsRunByStep = !mPhysicsRunByStep;
	}
	if (key == key_f)
	{		
		cObject* newObj = 
			mMainEngineScene->mSceneStuff->createRigidWoodBox(
			mCamera3dMouse->mPosition + mCamera3dMouse->mDirection*2.0f, vec3(1.0f, 1.0f, 1.0f), 
			vectorOrient(mCamera3dMouse->mDirection));

		mMainEngineScene->addObject(newObj);

		newObj->getPhysicsRigidBody()->mVelocity = mCamera3dMouse->mDirection*10.0f;
	}
}

void apPhysicsTestFrame::onKeyUp(int key)
{
}

void apPhysicsTestFrame::onActive()
{
}

void apPhysicsTestFrame::onDeActive()
{
}

void apPhysicsTestFrame::setupScene1()
{
	mMainEngineScene->addObject(
		mMainEngineScene->mSceneStuff->createStaticWoodBox(vec3(0.0f, 0.0f, 0.0f), vec3(50.0f, 1.0f, 50.0f)));
	
	/*for (int i = 0; i < 10; i++)
	{
		mMainEngineScene->addObject(
			mMainEngineScene->mSceneStuff->createRigidWoodBox(vec3(0.0f, 1.5f + i*1.1f, 0.5f), vec3(10.0f - 1.0f*i, 1.0f, 10.0f - 1.0f*i)));
	}*/
	mMainEngineScene->addObject(
		mMainEngineScene->mSceneStuff->createRigidWoodBox(vec3(0.0f, 0.7f, 0.5f), vec3(1.0f, 1.0f, 1.0f), 
			RotatedMatrix(rad(45.0f), rad(25.0f), rad(45.0f))));
	/*for (int i = 0; i < 10; i++)
	{
		mMainEngineScene->addObject(
			mMainEngineScene->mSceneStuff->createRigidWoodBox(vec3(0.0f, 1.5f + i*1.1f, 0.5f), vec3(1.0f, 1.0f, 1.0f)));

	}*/
}

void apPhysicsTestFrame::render2D()
{
	mWidgetsManager->draw();
}

void apPhysicsTestFrame::createUIWidgets()
{
	mWidgetsManager = new uiWidgetsManager(mRender);
	mInputMessenger->registInputListener(mWidgetsManager);
}
