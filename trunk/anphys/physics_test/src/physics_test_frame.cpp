#include "physics_test_frame.h"

#include "engine/engine_incl.h"
#include "util/debug/render_stuff.h"

#include "landscape_creator_wnd.h"
#include "vehicle_creator_wnd.h"
#include "vehicle/vehicle_chassis.h"

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
	mFollowCameraVehicle = true;

	mCamera3dMouse = new grCamera3DMouse;
	m2DCamera      = new grCamera2D;
	m3DRenderState = new grSimple3DRenderState(mRender);
	m2DRenderState = new gr2DRenderState(mRender);
	mVehicleCamera = new grCamera3D;

	m3DRenderState->bindCamera(mVehicleCamera);
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

	updateCameraControls();
	
	if (mPhysicsRunning) 
	{
		getRenderStuff().reset();	
		mMainEngineScene->mPhysicsScene->update(dt);
		updateVehicle(dt);
	}

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

	if (mPhysicsRunByStep)
		mPhysicsRunning = false;

	return dt;
}

void apPhysicsTestFrame::onClose()
{
	mEngine->exitEngine();
}

void apPhysicsTestFrame::onSize(fRect inRect)
{
	mRender->resize(inRect.getSize());
}

void apPhysicsTestFrame::onMouseMove(vec2 point)
{
	if (mWidgetsRes == 0)
	{
		if (mInputMessenger->mInputMessage.isKeyDown(CURSOR_BUTTON))
			mCamera3dMouse->mouseMoved(point - mInputMessenger->mInputMessage.mCursorPosition);
	}
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
			mCamera3dMouse->mPosition + mCamera3dMouse->mDirection*2.0f, 
			vec3(0.2f + random(0.0f, 2.0f), 0.2f + random(0.0f, 2.0f), 0.2f + random(0.0f, 2.0f)), 
			vectorOrient(mCamera3dMouse->mDirection));

		mMainEngineScene->addObject(newObj);

		newObj->getPhysicsRigidBody()->mVelocity = mCamera3dMouse->mDirection*30.0f;
	}
	if (key == key_e)
	{
		mMainMenuWindow->show();

		mFollowCameraVehicle = !mFollowCameraVehicle;
		if (mFollowCameraVehicle)
		{
			m3DRenderState->bindCamera(mVehicleCamera);
			mVehicleCamera->mPosition = mCamera3dMouse->mPosition;
		}
		else 
		{
			m3DRenderState->bindCamera(mCamera3dMouse);
			mCamera3dMouse->mPosition = mVehicleCamera->mPosition;
		}
	}
}

void apPhysicsTestFrame::setupScene1()
{
	return;

	mMainEngineScene->addObject(
		mMainEngineScene->mSceneStuff->createStaticWoodBox(vec3(0.0f, -0.5f, 0.0f), vec3(500.0f, 1.0f, 5000.0f)));
	
	for (int i = 0; i < 10; i++)
	{
		mMainEngineScene->addObject(
			mMainEngineScene->mSceneStuff->createRigidWoodBox(vec3(0.0f, 1.5f + i*1.1f, 0.5f), vec3(10.0f - 1.0f*i, 1.0f, 10.0f - 1.0f*i)));
	}

	mMainEngineScene->addObject(
		mMainEngineScene->mSceneStuff->createRigidWoodBox(vec3(0.0f, 0.7f, 0.5f), vec3(1.0f, 1.0f, 1.0f), 
			RotatedMatrix(rad(45.0f), rad(25.0f), rad(45.0f))));
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

//create surface material
	grSurfaceMaterial* surfaceMaterial = mRender->mSurfaceMaterials->addSurfaceMaterial(
		new grSurfaceMaterial("grass"));
	surfaceMaterial->pushTexture(mRender->mTextures->createTexture("../data/textures/wood.jpg"));
	surfaceMaterial->setMaterial(mRender->mMaterials->getMaterial("whiteMaterial"));
	surfaceMaterial->setShadeModel(NULL);

//add to scene
	mMainEngineScene->addObject(mLandscapeObject);

	mTestLandscapeGeom.resizeTestPolyBuffer(100);

	mLandscapeCreator->mLandscapeObject = mLandscapeObject;
	mLandscapeCreator->mTestLandscapeGeom = &mTestLandscapeGeom;
	mLandscapeCreator->mRenderScene = mMainEngineScene->mRenderScene;
	mLandscapeCreator->recreateLandscape();
}

void apPhysicsTestFrame::createVehicleObject()
{
	mVehicleObject = new cObject;

	vec3 size(1.8f, 1.1f, 3.7f);

//physics object
	
	mVehicle.mFrontLeftChassis->loadParametres(physics::vec3(-0.95f, -0.2f, 1.5f), physics::mat3x3(), 0, -0.3f, 0.33f, 70.0f, 80000.0f, 3000.0f, 1000.0f);
	mVehicle.mFrontRightChassis->loadParametres(physics::vec3(0.95f, -0.2f, 1.5f), physics::mat3x3(), 0, -0.3f, 0.33f, 70.0f, 80000.0f, 3000.0f, 1000.0f);
	mVehicle.mRearLeftChassis->loadParametres(physics::vec3(-0.95f, -0.2f, -1.5f), physics::mat3x3(), 0, -0.3f, 0.33f, 70.0f, 80000.0f, 3000.0f, 1000.0f, 100000000.0f);
	mVehicle.mRearRightChassis->loadParametres(physics::vec3(0.95f, -0.2f, -1.5f), physics::mat3x3(), 0, -0.3f, 0.33f, 70.0f, 80000.0f, 3000.0f, 1000.0f, 100000000.0f); 
	
//collision points
	physics::vec3 halsSize(size.x*0.5f, size.y*0.5f, size.z*0.5f);
	physics::vec3 collisionPoints[] = { physics::vec3(-halsSize.x, -halsSize.y, -halsSize.z), 
	                                    physics::vec3(halsSize.x, -halsSize.y, -halsSize.z), 
	                                    physics::vec3(halsSize.x, halsSize.y, -halsSize.z), 
	                                    physics::vec3(-halsSize.x, halsSize.y, -halsSize.z), 
							            
							            physics::vec3(-halsSize.x, -halsSize.y, halsSize.z), 
	                                    physics::vec3(halsSize.x, -halsSize.y, halsSize.z), 
	                                    physics::vec3(halsSize.x, halsSize.y, halsSize.z), 
	                                    physics::vec3(-halsSize.x, halsSize.y, halsSize.z) };
	int pointsCount = 8;
	for (int i = 0; i < pointsCount; i++)
	{
		mVehicle.mCollisionGeometryPoints.push_back(physics::CollisionGeometryVertex(collisionPoints[i]));
	}

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
	mVehicleCreator = new VehicleCreatorWidnow(mWidgetsManager, &mVehicle);
}

void apPhysicsTestFrame::updateVehicle( float dt )
{
	float fbuf[9];

	mVehicle.getPosition(fbuf);
	vec3 vehiclePos(fbuf[0], fbuf[1], fbuf[2]);

	mVehicle.getOrientation(fbuf);
	mat3x3 vehicleOrient( fbuf[0], fbuf[1], fbuf[2],
		                  fbuf[3], fbuf[4], fbuf[5],
						  fbuf[6], fbuf[7], fbuf[8] );

	if (mFollowCameraVehicle)
	{
		vec3 targetCamPos = vehiclePos + vec3(0, 5, -9)*vehicleOrient;
		vec3 targetCamTargetPos = vehiclePos + vec3(0, 0, 30)*vehicleOrient;
		
		mVehicleCamPos += (targetCamPos - mVehicleCamPos)*dt*1.0f;
		mVehicleCamTargetPos += (targetCamTargetPos - mVehicleCamTargetPos)*dt*1.0f;

		mVehicleCamera->mPosition = mVehicleCamPos; 
		mVehicleCamera->mLookPoint = mVehicleCamTargetPos; 
	}
	
	mVehicle.mFrontLeftChassis->mWheelAngle = 0;
	mVehicle.mFrontRightChassis->mWheelAngle = 0;

	if (isKeyDown(key_up))
	{
		mVehicle.mRearLeftChassis->mWheelTorque -= 250.0f*dt;
		mVehicle.mRearRightChassis->mWheelTorque -= 250.0f*dt;
		/*mLeftForwardChassis->mWheelTorque -= 50.0f*dt;
		mRightForwardChassis->mWheelTorque -= 50.0f*dt;*/
	}
	if (isKeyDown(key_down))
	{
		mVehicle.mFrontLeftChassis->mBrakeCoef1 += 2.0f*dt;
		if (mVehicle.mFrontLeftChassis->mBrakeCoef1 > 1.0f)
			mVehicle.mFrontLeftChassis->mBrakeCoef1 = 1.0f;

		mVehicle.mFrontRightChassis->mBrakeCoef1 += 2.0f*dt;
		if (mVehicle.mFrontRightChassis->mBrakeCoef1 > 1.0f)
			mVehicle.mFrontRightChassis->mBrakeCoef1 = 1.0f;

		mVehicle.mRearLeftChassis->mBrakeCoef1 += 2.0f*dt;
		if (mVehicle.mRearLeftChassis->mBrakeCoef1 > 1.0f)
			mVehicle.mRearLeftChassis->mBrakeCoef1 = 1.0f;

		mVehicle.mRearRightChassis->mBrakeCoef1 += 2.0f*dt;
		if (mVehicle.mRearRightChassis->mBrakeCoef1 > 1.0f)
			mVehicle.mRearRightChassis->mBrakeCoef1 = 1.0f;
	}
	else
	{
		mVehicle.mFrontLeftChassis->mBrakeCoef1 -= 5.0f*dt;
		if (mVehicle.mFrontLeftChassis->mBrakeCoef1 < 0.0f)
			mVehicle.mFrontLeftChassis->mBrakeCoef1 = 0.0f;

		mVehicle.mFrontRightChassis->mBrakeCoef1 -= 5.0f*dt;
		if (mVehicle.mFrontRightChassis->mBrakeCoef1 < 0.0f)
			mVehicle.mFrontRightChassis->mBrakeCoef1 = 0.0f;
		
		mVehicle.mRearLeftChassis->mBrakeCoef1 -= 5.0f*dt;
		if (mVehicle.mRearLeftChassis->mBrakeCoef1 < 0.0f)
			mVehicle.mRearLeftChassis->mBrakeCoef1 = 0.0f;

		mVehicle.mRearRightChassis->mBrakeCoef1 -= 5.0f*dt;
		if (mVehicle.mRearRightChassis->mBrakeCoef1 < 0.0f)
			mVehicle.mRearRightChassis->mBrakeCoef1 = 0.0f;
	}
	if (isKeyDown(key_right))
	{
		mVehicle.mFrontLeftChassis->mWheelAngle = rad(30.0f);
		mVehicle.mFrontRightChassis->mWheelAngle = rad(30.0f);
	}
	if (isKeyDown(key_left))
	{
		mVehicle.mFrontLeftChassis->mWheelAngle = rad(-30.0f);
		mVehicle.mFrontRightChassis->mWheelAngle = rad(-30.0f);
	}

	if (isKeyDown(key_numpad_0))
	{
		mVehicle.mRearLeftChassis->mBrakeCoef2 = 1.0f;
		mVehicle.mRearRightChassis->mBrakeCoef2 = 1.0f;
	}
	else
	{
		mVehicle.mRearLeftChassis->mBrakeCoef2 = 0.0f;
		mVehicle.mRearRightChassis->mBrakeCoef2 = 0.0f;
	}

	AABB vehicleAabb(vehiclePos + vec3(-2.5f, -2.5f, -2.5f),
		             vehiclePos + vec3(2.5f, 2.5f, 2.5f) );

	mTestLandscapeGeom.getPolygons(mVehicle.mPosition + physics::vec3(-2.5f, -2.5f, -2.5f),
		                           mVehicle.mPosition + physics::vec3(2.5f, 2.5f, 2.5f) );

	mVehicle.setPolygonsBuffer(mTestLandscapeGeom.mTestPolygonsBuffer, 
		mTestLandscapeGeom.mTestPolygonsBufferCount);
	
	mVehicle.update(dt);
	
	mVehicleObject->getComponent<cRender3DObjectComponent>()->mRender3DObject->mPosition = vehiclePos;
	mVehicleObject->getComponent<cRender3DObjectComponent>()->mRender3DObject->mOrient = vehicleOrient;

	physics::VehicleChassis* chassisArray[4] = { mVehicle.mFrontLeftChassis, mVehicle.mFrontRightChassis,
												 mVehicle.mRearLeftChassis, mVehicle.mRearRightChassis };

	for (int i = 0; i < 4; i++)
	{
		physics::VehicleChassis* chassis = chassisArray[i];		

		chassis->getPosition(fbuf);
		vec3 wheelPos(fbuf[0], fbuf[1], fbuf[2]);

		chassis->getOrientation(fbuf);
		mat3x3 wheelOrient( fbuf[0], fbuf[1], fbuf[2],
							fbuf[3], fbuf[4], fbuf[5],
							fbuf[6], fbuf[7], fbuf[8] );

		getRenderStuff().addRedArrow(wheelPos, wheelPos + vec3(chassis->myaxisNorm.x, chassis->myaxisNorm.y, chassis->myaxisNorm.z));

		int segmentsCount = 10;

		for (int j = 0; j < segmentsCount; j++)
		{
			float ang = rad(360.0f/(float)segmentsCount*(float)j);
			float cs = cosf(ang);
			float sn = sinf(ang);
			getRenderStuff().addBlueArrow(wheelPos, wheelPos + wheelOrient.getYVector()*cs*chassis->mWheelRadius + 
				                                               wheelOrient.getZVector()*sn*chassis->mWheelRadius );
		}
	}
}

void apPhysicsTestFrame::updateCameraControls()
{
	if (isKeyDown(key_w)) mCamera3dMouse->moveForward(isKeyDown(key_shift));
	if (isKeyDown(key_s)) mCamera3dMouse->moveBack(isKeyDown(key_shift));
	if (isKeyDown(key_a)) mCamera3dMouse->moveLeft(isKeyDown(key_shift));
	if (isKeyDown(key_d)) mCamera3dMouse->moveRight(isKeyDown(key_shift));
	if (isKeyDown(key_ctrl)) mCamera3dMouse->moveDown(isKeyDown(key_shift));
	if (isKeyDown(key_space)) mCamera3dMouse->moveUp(isKeyDown(key_shift));
}

TestLandscape::TestLandscape()
{
	mVertexBuffer = NULL;
	mPolygonsBuffer = NULL;
	mTestPolygonsBuffer = NULL;
	mVertexCount = mPolygonsCount = mTestPolygonsBufferSize = mTestPolygonsBufferCount = 0;
}

TestLandscape::~TestLandscape()
{
	safe_release_arr(mVertexBuffer);
	safe_release_arr(mPolygonsBuffer);
	safe_release_arr(mTestPolygonsBuffer);
}

void TestLandscape::resizeBuffers( unsigned int vertexCount, unsigned int polygonsCount )
{	
	safe_release_arr(mVertexBuffer);
	safe_release_arr(mPolygonsBuffer);

	mVertexBuffer = new physics::lVertex[vertexCount];
	mPolygonsBuffer = new physics::lPolygon[polygonsCount];

	mVertexCount = vertexCount;
	mPolygonsCount = polygonsCount;
}

void TestLandscape::resizeTestPolyBuffer( unsigned int size )
{
	safe_release_arr(mTestPolygonsBuffer);

	mTestPolygonsBuffer = new physics::lPolygon*[size];
	mTestPolygonsBufferCount = 0;
	mTestPolygonsBufferSize = size;
}

void TestLandscape::getPolygons( const physics::vec3& minv, const physics::vec3& maxv )
{
	mTestPolygonsBufferCount = 0;
	physics::AABB aabb(minv, maxv);

	for (int i = 0; i < mPolygonsCount; i++)
	{
		physics::lPolygon* poly = &mPolygonsBuffer[i];
		if (poly->aabb.isIntersect(aabb))
		{
			mTestPolygonsBuffer[mTestPolygonsBufferCount++] = poly;

			getRenderStuff().addBlueArrow(vec3(poly->pa->mPosition.x, poly->pa->mPosition.y, poly->pa->mPosition.z),
				                          vec3(poly->pb->mPosition.x, poly->pb->mPosition.y, poly->pb->mPosition.z) );

			getRenderStuff().addBlueArrow(vec3(poly->pc->mPosition.x, poly->pc->mPosition.y, poly->pc->mPosition.z),
				                          vec3(poly->pb->mPosition.x, poly->pb->mPosition.y, poly->pb->mPosition.z) );

			getRenderStuff().addBlueArrow(vec3(poly->pa->mPosition.x, poly->pa->mPosition.y, poly->pa->mPosition.z),
				                          vec3(poly->pc->mPosition.x, poly->pc->mPosition.y, poly->pc->mPosition.z) );

			if (mTestPolygonsBufferCount == mTestPolygonsBufferSize)
				break;
		}
	}
}
