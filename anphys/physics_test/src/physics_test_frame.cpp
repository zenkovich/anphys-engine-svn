#include "physics_test_frame.h"

#include "engine/engine_incl.h"
#include "util/debug/render_stuff.h"

#include "physics/CD/collision_detection/box_box_cd.h"
#include "physics/CD/collision_geometry/box_collision_geometry.h"
#include "physics/CD/collision_geometry/collision_geometry.h"
#include "physics/CD/collision_geometry/collision.h"
#include "physics/CD/collision_geometry/collision_point.h"

#include "render/render_objects/2d/sprite.h"

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
	safe_release(mTestSprite);
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
	grTexture* tex2 = mRender->mTextures->createTexture("textures/pngtest2");
	mTest2DMesh = new grRender2DObjectMesh(mRender, 4, 2);
	mTest2DMesh->mVertexBuffer[0] = vertex2d(10.0f, 10.0f, 1.0f, 0.0f, 0.0f, color4(1.0f, 1.0f, 1.0f, 1.0f).dwordARGB());
	mTest2DMesh->mVertexBuffer[1] = vertex2d(100.0f, 10.0f, 1.0f, 1.0f, 0.0f, color4(1.0f, 1.0f, 1.0f, 1.0f).dwordARGB());
	mTest2DMesh->mVertexBuffer[2] = vertex2d(100.0f, 100.0f, 1.0f, 1.0f, 1.0f, color4(1.0f, 1.0f, 1.0f, 1.0f).dwordARGB());
	mTest2DMesh->mVertexBuffer[3] = vertex2d(10.0f, 100.0f, 1.0f, 0.0f, 1.0f, color4(1.0f, 1.0f, 1.0f, 1.0f).dwordARGB());
	mTest2DMesh->mPolygonsBuffer[0] = poly3(0, 1, 2);
	mTest2DMesh->mPolygonsBuffer[1] = poly3(0, 2, 3);
	mTest2DMesh->pushTexture(tex);
	mTest2DMesh->mRenderObjectsManager = mMainEngineScene->mRenderScene->mObjects;

	mRenderTexture = mRender->mTextures->createRenderTexture(inRect.getSize());
	mTest2DMesh2 = new grRender2DObjectMesh(mRender, 4, 2);
	mTest2DMesh2->mVertexBuffer[0] = vertex2d(110.0f, 10.0f, 1.0f, 0.5f, 0.5f, color4(1.0f, 1.0f, 1.0f, 1.0f).dwordARGB());
	mTest2DMesh2->mVertexBuffer[1] = vertex2d(200.0f, 10.0f, 1.0f, 1.0f, 0.5f, color4(1.0f, 1.0f, 1.0f, 1.0f).dwordARGB());
	mTest2DMesh2->mVertexBuffer[2] = vertex2d(200.0f, 100.0f, 1.0f, 1.0f, 1.0f, color4(1.0f, 1.0f, 1.0f, 1.0f).dwordARGB());
	mTest2DMesh2->mVertexBuffer[3] = vertex2d(110.0f, 100.0f, 1.0f, 0.5f, 1.0f, color4(1.0f, 1.0f, 1.0f, 1.0f).dwordARGB());
	mTest2DMesh2->mPolygonsBuffer[0] = poly3(0, 1, 2);
	mTest2DMesh2->mPolygonsBuffer[1] = poly3(0, 2, 3);
	mTest2DMesh2->pushTexture(tex2);
	mTest2DMesh2->mRenderObjectsManager = mMainEngineScene->mRenderScene->mObjects;

	mTextureRenderTarget = new grTextureRenderTarget(mRender, mRenderTexture);

	mTestSprite = new grSprite(mRender, tex);
	mTestSprite->setSize(vec2(100, 200)).setPosition(vec2(300, 400)).setRotationCenter(vec2(50, 100));

	cDataObject dataObj;
	mTestSprite->serialize(dataObj, AT_OUTPUT, "sprite");

	getDataObjectsManager().saveDataObject("../spriteTest.xml", cDataObjectsManager::DOT_XML, dataObj);

	safe_release(mTestSprite);

	mTestSprite = new grSprite(mRender, "../spriteTest", "sprite");

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
	//gLog->fout(1, "drawing mesh 1\n");
	//assert(mRender->bindRenderTarget(mTextureRenderTarget), "");
	//mTest2DMesh->draw();
	//assert(mRender->unbindRenderTarget(mTextureRenderTarget), "");

	//gLog->fout(1, "drawing mesh 2\n");
	//mTest2DMesh2->draw();
	//mTest2DMesh2->draw();
	//
	/*vec2 dist(4, 4), size(30, 30), pos(10, 10), pos2(20, 10);
	for (int x = 0; x < 1000; x++)
	{
		drawMesh(vec2(rand()%500, rand()%500), size, mTest2DMesh);
	}
	for (int x = 0; x < 1000; x++)
	{
		drawMesh(vec2(rand()%500, rand()%500), size, mTest2DMesh2);
	}*/
	/*for (int i = 0; i < 1000; i++)
	{
		mTestSprite->setPosition(vec2(rand()%500, rand()%500)).setAngle(rand()%360).setSize(vec2(10)).draw();
	}*/

	mTestSprite->setAngle(mTestSprite->getAngle() + 360.0f/50.0f*0.017f).draw();
}

void apPhysicsTestFrame::drawMesh( vec2& pos, vec2& size, grRender2DObjectMesh* mesh )
{
	vec2 halfSize = size*0.5f;

	mesh->mVertexBuffer[0].x = pos.x - halfSize.x; mesh->mVertexBuffer[0].y = pos.y - halfSize.y;
	mesh->mVertexBuffer[1].x = pos.x + halfSize.x; mesh->mVertexBuffer[1].y = pos.y - halfSize.y;
	mesh->mVertexBuffer[2].x = pos.x + halfSize.x; mesh->mVertexBuffer[2].y = pos.y + halfSize.y;
	mesh->mVertexBuffer[3].x = pos.x - halfSize.x; mesh->mVertexBuffer[3].y = pos.y + halfSize.y;

	mesh->draw();
}
