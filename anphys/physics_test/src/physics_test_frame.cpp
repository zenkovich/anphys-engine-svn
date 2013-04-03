#include "physics_test_frame.h"

#include "engine/engine_incl.h"
#include "util/debug/render_stuff.h"

#include "physics/CD/collision_detection/box_box_cd.h"
#include "physics/CD/collision_geometry/box_collision_geometry.h"
#include "physics/CD/collision_geometry/collision_geometry.h"
#include "physics/CD/collision_geometry/collision.h"
#include "physics/CD/collision_geometry/collision_point.h"

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
	safe_release(mTextureRenderTarget);
	safe_release(mCamera3dMouse);
	safe_release(m2DCamera);
	safe_release(mMainEngineScene);
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
	light->initialize(grLightBaseInterface::light_directional_type, color4(1.0f, 1.0f, 1.0f, 1.0f), color4(1.0f, 1.0f, 1.0f, 1.0f),
		color4(0.5f,0.5f,0.5f,1.0f), vec3(0,0,0), vec3(0,-1,0), 0, 0, 0, 0, 0, 0, 0);
	light->setLightActive(true);

	setupScene1();

	grTexture* tex = mRender->mTextures->createTexture("textures/pngtest");
	mTest2DMesh = new grRender2DObjectMesh(4, 2);
	mTest2DMesh->mVertexBuffer[0] = vertex2d(10.0f, 10.0f, 1.0f, 0.0f, 0.0f, color4(1.0f, 0.0f, 0.0f, 1.0f).dwordARGB());
	mTest2DMesh->mVertexBuffer[1] = vertex2d(300.0f, 10.0f, 1.0f, 1.0f, 0.0f, color4(0.0f, 1.0f, 0.0f, 1.0f).dwordARGB());
	mTest2DMesh->mVertexBuffer[2] = vertex2d(300.0f, 300.0f, 1.0f, 1.0f, 1.0f, color4(0.0f, 0.0f, 1.0f, 1.0f).dwordARGB());
	mTest2DMesh->mVertexBuffer[3] = vertex2d(10.0f, 300.0f, 1.0f, 0.0f, 1.0f, color4(0.0f, 1.0f, 1.0f, 1.0f).dwordARGB());
	mTest2DMesh->mPolygonsBuffer[0] = poly3(0, 1, 2);
	mTest2DMesh->mPolygonsBuffer[1] = poly3(0, 2, 3);
	mTest2DMesh->pushTexture(tex);
	mTest2DMesh->mRenderObjectsManager = mMainEngineScene->mRenderScene->mObjects;

	mRenderTexture = mRender->mTextures->createRenderTexture(inRect.getSize());
	mTest2DMesh2 = new grRender2DObjectMesh(4, 2);
	mTest2DMesh2->mVertexBuffer[0] = vertex2d(100.0f, 100.0f, 1.0f, 0.0f, 0.0f, color4(1.0f, 0.0f, 0.0f, 1.0f).dwordARGB());
	mTest2DMesh2->mVertexBuffer[1] = vertex2d(200.0f, 100.0f, 1.0f, 1.0f, 0.0f, color4(0.0f, 1.0f, 0.0f, 1.0f).dwordARGB());
	mTest2DMesh2->mVertexBuffer[2] = vertex2d(200.0f, 200.0f, 1.0f, 1.0f, 1.0f, color4(0.0f, 0.0f, 1.0f, 1.0f).dwordARGB());
	mTest2DMesh2->mVertexBuffer[3] = vertex2d(100.0f, 200.0f, 1.0f, 0.0f, 1.0f, color4(0.0f, 1.0f, 1.0f, 1.0f).dwordARGB());
	mTest2DMesh2->mPolygonsBuffer[0] = poly3(0, 1, 2);
	mTest2DMesh2->mPolygonsBuffer[1] = poly3(0, 2, 3);
	mTest2DMesh2->pushTexture(tex);
	mTest2DMesh2->mRenderObjectsManager = mMainEngineScene->mRenderScene->mObjects;

	mTextureRenderTarget = new grTextureRenderTarget(mRender, mRenderTexture);

	mPhysicsRunning = false;
}

float apPhysicsTestFrame::onTimer()
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
		mMainEngineScene->mPhysicsScene->update(dt);
	}

	mMainEngineScene->update(dt);
	mRender->update(dt);
	mCamera3dMouse->update(dt);

	mRender->beginRender();

	//mRender->bindRenderState(m3DRenderState);

	//mRender->render();

	mRender->bindRenderState(m2DRenderState);

	render2D();

	mRender->endRender();
	/*mRender->update(dt);
	mMainEngineScene->update(dt);
	mRender->preRender();
	mRender->render();
	render2D();
	mRender->postRender();

	/*
	 mRender->update(dt);
	 mMainEngineScene->update(dt);
	 mCamera1->update(dt);
	 mCamera2->update(dt);
	 m2DCamera->update(dt);

	 mRender->beginRender();

	 mRender->bindRenderState(m3DRenderState);
	 mRender->bindRenderTarget(mCamera1RenderTarget);

	 m3DRenderState->bindCamera(mCamera1);

	 mMainEngineScene->draw();

	 mRender->unbindRenderTarget(mCamera1RenderTarget);
	 mRender->bindRenderTarget(mCamera2RenderTarget);

	 m3DRenderState->bindCamera(mCamera2);

	 mMainEngineScene->draw();

	 mRender->unbindRenderTarget(mCamera2RenderTarget);

	 mRender->bindRenderState(m2DRenderState);

	 call2DDrawing();  //where drawing two cameras render targets and other 2D Geometry

	 mRender->endRender();
	 
	 **/


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
	if (mLeftMouseButton) mCamera3dMouse->mouseMoved(point - mCursorPos);
}

void apPhysicsTestFrame::onMouseWheel(float delta)
{
}

void apPhysicsTestFrame::onKeyDown(int key)
{
	*gLog << formatStr("key = %i\n", key).c_str();
	
	if (key == key_t) mPhysicsRunning = !mPhysicsRunning;
	if (key == key_y) mPhysicsRunByStep = !mPhysicsRunByStep;
	if (key == key_f)
	{		
		mMainEngineScene->addObject(
			mMainEngineScene->mSceneStuff->createRigidWoodBox(
				mCamera3dMouse->mPosition + mCamera3dMouse->mDirection*2.0f, vec3(1.0f, 1.0f, 1.0f), 
				vectorOrient(mCamera3dMouse->mDirection)));
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
	gLog->fout(1, "drawing mesh 1\n");
	//assert(mRender->bindRenderTarget(mTextureRenderTarget), "");
	mTest2DMesh->draw();
	//assert(mRender->unbindRenderTarget(mTextureRenderTarget), "");

	gLog->fout(1, "drawing mesh 2\n");
	mTest2DMesh2->draw();
}
