#include "physics_test_frame.h"

#include "engine/engine_incl.h"
#include "util/debug/render_stuff.h"

#include "landscape_creator_wnd.h"
#include "vehicle_creator_wnd.h"
#include "physics/objects/vehicle/vehicle.h"
#include "physics/objects/vehicle/chassis_vehicle_component.h"
#include "physics/objects/vehicle/vehicle_wheel.h"

apPhysicsTestFrame::apPhysicsTestFrame():apRenderWindow(), mMainEngineScene(NULL), mPhysicsRunning(true), 
	mPhysicsRunByStep(false), mLandscapeCreator(NULL)
{
	onCreate(mInRect);
}

apPhysicsTestFrame::apPhysicsTestFrame(const std::string& title, fRect wndRect, fRect outputRect):
	apRenderWindow(title, wndRect, outputRect), mMainEngineScene(NULL), mPhysicsRunning(true), mPhysicsRunByStep(false), 
	mLandscapeCreator(NULL)
{
	onCreate(mInRect);
}

apPhysicsTestFrame::apPhysicsTestFrame(const std::string& title, fRect wndRect):
	apRenderWindow(title, wndRect), mMainEngineScene(NULL), mPhysicsRunning(true), mPhysicsRunByStep(false), 
	mLandscapeCreator(NULL)
{
	onCreate(mInRect);
}

apPhysicsTestFrame::~apPhysicsTestFrame()
{
	safe_release(mLandscapeCreator);

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
	createLandscapeObject();
	createVehicleObject();

	mPhysicsRunning = false;
}

float apPhysicsTestFrame::onTimer()
{
	float dt = apRenderWindow::onTimer();
	
	if (mPhysicsRunning) 
	{
		getRenderStuff().reset();	
		mMainEngineScene->mPhysicsScene->update(dt);
	}

	if (isKeyDown(key_w)) mCamera3dMouse->moveForward(isKeyDown(key_shift));
	if (isKeyDown(key_s)) mCamera3dMouse->moveBack(isKeyDown(key_shift));
	if (isKeyDown(key_a)) mCamera3dMouse->moveLeft(isKeyDown(key_shift));
	if (isKeyDown(key_d)) mCamera3dMouse->moveRight(isKeyDown(key_shift));
	if (isKeyDown(key_ctrl)) mCamera3dMouse->moveDown(isKeyDown(key_shift));
	if (isKeyDown(key_space)) mCamera3dMouse->moveUp(isKeyDown(key_shift));
	
	mLeftForwardChassis->mWheelAngle = 0;
	mRightForwardChassis->mWheelAngle = 0;

	if (isKeyDown(key_up))
	{
		mLeftRearChassis->mWheelTorque -= 500.0f*dt;
		mRightRearChassis->mWheelTorque -= 500.0f*dt;
		mLeftForwardChassis->mWheelTorque -= 500.0f*dt;
		mRightForwardChassis->mWheelTorque -= 500.0f*dt;
	}
	if (isKeyDown(key_down))
	{
		mLeftForwardChassis->mBrakeCoef1 += 2.0f*dt;
		if (mLeftForwardChassis->mBrakeCoef1 > 1.0f)
			mLeftForwardChassis->mBrakeCoef1 = 1.0f;

		mRightForwardChassis->mBrakeCoef1 += 2.0f*dt;
		if (mRightForwardChassis->mBrakeCoef1 > 1.0f)
			mRightForwardChassis->mBrakeCoef1 = 1.0f;

		mLeftRearChassis->mBrakeCoef1 += 2.0f*dt;
		if (mLeftRearChassis->mBrakeCoef1 > 1.0f)
			mLeftRearChassis->mBrakeCoef1 = 1.0f;

		mRightRearChassis->mBrakeCoef1 += 2.0f*dt;
		if (mRightRearChassis->mBrakeCoef1 > 1.0f)
			mRightRearChassis->mBrakeCoef1 = 1.0f;
	}
	else
	{
		mLeftForwardChassis->mBrakeCoef1 -= 5.0f*dt;
		if (mLeftForwardChassis->mBrakeCoef1 < 0.0f)
			mLeftForwardChassis->mBrakeCoef1 = 0.0f;

		mRightForwardChassis->mBrakeCoef1 -= 5.0f*dt;
		if (mRightForwardChassis->mBrakeCoef1 < 0.0f)
			mRightForwardChassis->mBrakeCoef1 = 0.0f;
		
		mLeftRearChassis->mBrakeCoef1 -= 5.0f*dt;
		if (mLeftRearChassis->mBrakeCoef1 < 0.0f)
			mLeftRearChassis->mBrakeCoef1 = 0.0f;

		mRightRearChassis->mBrakeCoef1 -= 5.0f*dt;
		if (mRightRearChassis->mBrakeCoef1 < 0.0f)
			mRightRearChassis->mBrakeCoef1 = 0.0f;
	}
	if (isKeyDown(key_right))
	{
		mLeftForwardChassis->mWheelAngle = rad(30.0f);
		mRightForwardChassis->mWheelAngle = rad(30.0f);
	}
	if (isKeyDown(key_left))
	{
		mLeftForwardChassis->mWheelAngle = rad(-30.0f);
		mRightForwardChassis->mWheelAngle = rad(-30.0f);
	}

	if (isKeyDown(key_numpad_0))
	{
		mLeftRearChassis->mBrakeCoef2 = 1.0f;
		mRightRearChassis->mBrakeCoef2 = 1.0f;
	}
	else
	{
		mLeftRearChassis->mBrakeCoef2 = 0.0f;
		mRightRearChassis->mBrakeCoef2 = 0.0f;
	}

	//mInputMessenger->sendInputMessage();
	mWidgetsManager->mLastInputMessage = &mInputMessenger->mInputMessage;
	mWidgetsRes = mWidgetsManager->processInputMessage(mInputMessenger->mInputMessage);
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
	if (mWidgetsRes == 0)
	{
		if (mInputMessenger->mInputMessage.isKeyDown(CURSOR_BUTTON))
			mCamera3dMouse->mouseMoved(point - mInputMessenger->mInputMessage.mCursorPosition);
	}
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
	if (key == key_e)
	{
		mMainMenuWindow->show();
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
	return;

	mMainEngineScene->addObject(
		mMainEngineScene->mSceneStuff->createStaticWoodBox(vec3(0.0f, -0.5f, 0.0f), vec3(500.0f, 1.0f, 5000.0f)));
	
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

	//create main menu wnd
	mMainMenuWindow = uiSimpleStuff::createWindow(mWidgetsManager, "MainMenuWnd", vec2(0, 0), vec2(220, 100), "Menu");
	uiSimpleStuff::createSizeEffect(mMainMenuWindow);

	uiVertLayoutWidget* verLayoutWidget = new uiVertLayoutWidget(mWidgetsManager, "verLayout");
	verLayoutWidget->mWidgetsDistance = 5.0f;
	verLayoutWidget->setPosition(vec2(5, 5));

	uiButton* landscapeCreatorBtn = uiSimpleStuff::createButton(mWidgetsManager, vec2(0, 0), vec2(200, 25), "landscapeBtn",
		"Landscape creator", new cCallback<apPhysicsTestFrame>(this, &apPhysicsTestFrame::onOpenLandscapeCreatorBtnPressed));

	uiButton* vehicleEditorBtn = uiSimpleStuff::createButton(mWidgetsManager, vec2(0, 0), vec2(200, 25), "vehEditortn",
		"Vehicle editor", new cCallback<apPhysicsTestFrame>(this, &apPhysicsTestFrame::onOpenVehicleCreatorBtnPressed));
	
	verLayoutWidget->addChild((uiWidget*)landscapeCreatorBtn);
	verLayoutWidget->addChild((uiWidget*)vehicleEditorBtn);

	mMainMenuWindow->addChild(verLayoutWidget);

	mWidgetsManager->addWidget(mMainMenuWindow);
	mMainMenuWindow->show(true);

//landscape creator
	mLandscapeCreator = new LandscapeCreatorWnd(mWidgetsManager);
}

void apPhysicsTestFrame::onOpenLandscapeCreatorBtnPressed()
{
	mLandscapeCreator->show();
}

void apPhysicsTestFrame::onOpenVehicleCreatorBtnPressed()
{
	mVehicleCreator->show();
}

void apPhysicsTestFrame::createLandscapeObject()
{
	mLandscapeObject = new cObject;

//creating physics component
	phCollisionGeometry* collisionGeom = new phCollisionGeometry;
	phLandscapeCollisionGeometry* landscapeCollisionGeometry = new phLandscapeCollisionGeometry();
	collisionGeom->addPart(landscapeCollisionGeometry);
	phStaticObject* staticObj = new phStaticObject(vec3(0,0,0), nullMatr(), collisionGeom);

	cPhysicsStaticBodyObjectComponent* physicsComponent = new cPhysicsStaticBodyObjectComponent(mLandscapeObject, staticObj);
	mLandscapeObject->addComponent(physicsComponent);

//create surface material
	grSurfaceMaterial* surfaceMaterial = mRender->mSurfaceMaterials->addSurfaceMaterial(
		new grSurfaceMaterial("grass"));
	surfaceMaterial->pushTexture(mRender->mTextures->createTexture("../data/textures/wood.jpg"));
	surfaceMaterial->setMaterial(mRender->mMaterials->getMaterial("whiteMaterial"));
	surfaceMaterial->setShadeModel(NULL);

//add to scene
	mMainEngineScene->addObject(mLandscapeObject);

	mLandscapeCreator->mLandscapeObject = mLandscapeObject;
	mLandscapeCreator->mRenderScene = mMainEngineScene->mRenderScene;
	mLandscapeCreator->recreateLandscape();
}

void apPhysicsTestFrame::createVehicleObject()
{
	mVehicleObject = new cObject;

	vec3 size(1.8f, 1.1f, 3.7f);

//physics object
	phVehicle* physicsObject = new phVehicle;
	cPhysicsRigidBodyObjectComponent* physicsComponent = new cPhysicsRigidBodyObjectComponent(physicsObject);
	
	mLeftForwardChassis = new phVehicleChassisComponent(physicsObject, "forwardLeftChassis");
	mRightForwardChassis = new phVehicleChassisComponent(physicsObject, "forwardRightChassis");
	mLeftRearChassis = new phVehicleChassisComponent(physicsObject, "rearLeftChassis");
	mRightRearChassis = new phVehicleChassisComponent(physicsObject, "rearRightChassis");
	
	physicsObject->addComponent(mLeftForwardChassis);
	physicsObject->addComponent(mRightForwardChassis);
	physicsObject->addComponent(mLeftRearChassis);
	physicsObject->addComponent(mRightRearChassis);
	
	mLeftForwardChassis->loadParametres(vec3(-0.95f, -0.2f, 1.5f), nullMatr(), 0, -0.3f, 0.33f, 70.0f, 80000.0f, 3000.0f, 1000.0f);
	mRightForwardChassis->loadParametres(vec3(0.95f, -0.2f, 1.5f), nullMatr(), 0, -0.3f, 0.33f, 70.0f, 80000.0f, 3000.0f, 1000.0f);
	mLeftRearChassis->loadParametres(vec3(-0.95f, -0.2f, -1.5f), nullMatr(), 0, -0.3f, 0.33f, 70.0f, 80000.0f, 3000.0f, 1000.0f, 100000000.0f);
	mRightRearChassis->loadParametres(vec3(0.95f, -0.2f, -1.5f), nullMatr(), 0, -0.3f, 0.33f, 70.0f, 80000.0f, 3000.0f, 1000.0f, 100000000.0f); 
	
	mMainEngineScene->mSceneStuff->addBoxCollisionGeometry(physicsObject, size);

	mMainEngineScene->mPhysicsScene->addObject(physicsObject);

	mVehicleObject->addComponent(physicsComponent);

//graphics
	grRender3DObjectMesh* boxMesh = mMainEngineScene->mSceneStuff->createMesh(128, 128);
	mMainEngineScene->mSceneStuff->addBoxMesh(boxMesh, size, 
		mMainEngineScene->mSceneStuff->createSurfaceMaterial(
			mMainEngineScene->mSceneStuff->createTexture("../data/textures/wood.jpg"), 
			mMainEngineScene->mSceneStuff->getMaterial("whiteMaterial")));
	
	cRender3DObjectComponent* boxMeshComponent = new cRender3DObjectComponent(boxMesh);
	mVehicleObject->addComponent(boxMeshComponent);

//add to scene	
	mMainEngineScene->addObject(mVehicleObject);		

//vehicle creator
	mVehicleCreator = new VehicleCreatorWidnow(mWidgetsManager, physicsObject);
}
