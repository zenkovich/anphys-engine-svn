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

	mOffClutchTime = 0;
	mOffClutchDelay = 0.5f;
	mControlsReactDelay = 0.1f;
	mGearOffset = 0;

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

	//dt = 1.0f/60.0f;

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
	
	if (key ==(key_b))
	{
		//mVehicle.mCurrentGear--;
		mOffClutchTime = mOffClutchDelay;
		mGearOffset = -1;
	}
	if (key ==(key_g))
	{
		//mVehicle.mCurrentGear++;
		mGearOffset = 1;
		mOffClutchTime = mOffClutchDelay;
	}
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
	mMainMenuWindow = uiSimpleStuff::createWindow(mWidgetsManager, "MainMenuWnd", vec2(0, 0), vec2(300, 400), "Menu");
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

	uiSimpleStuff::addScrollProperty<float>(verLayoutWidget, "RPM", &mVehicle.mEngineRpm, 0, 8000);
	uiSimpleStuff::addScrollProperty<float>(verLayoutWidget, "Throttle", &mVehicle.mThrottleCoef);
	uiSimpleStuff::addScrollProperty<float>(verLayoutWidget, "Brake", &mVehicle.mBrakeCoef);
	uiSimpleStuff::addScrollProperty<float>(verLayoutWidget, "Hand BR", &mVehicle.mHandBrakeCoef);
	uiSimpleStuff::addScrollProperty<float>(verLayoutWidget, "Clutch", &mVehicle.mClutchCoef);
	uiSimpleStuff::addScrollProperty<float>(verLayoutWidget, "Drive Coef", &mVehicle.mResDriveCoef, -15.0f, 15.0f);
	
	uiSimpleStuff::addProperty<std::string>(verLayoutWidget, "Gear", &mVehicleGearName);
	uiSimpleStuff::addProperty<float>(verLayoutWidget, "Velocty", &mVelocity);
	uiSimpleStuff::addProperty<float>(verLayoutWidget, "Wh Velocty", &mWVelocity);
	uiSimpleStuff::addProperty<float>(verLayoutWidget, "Accel", &mAcceleration);
	uiSimpleStuff::addScrollProperty<float>(verLayoutWidget, "Slip", &mSlipFact, -15.0f, 15.0f);

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
	float peakValue = 252024;
	float frictionValues[] = {1.0f, 1.0f, 0.9f, 0.8f, 0.8f};
	
	mVehicle.mFrontLeftChassis->loadParametres(physics::vec3(-0.95f, -0.2f, 1.5f), physics::mat3x3(), 0, -0.3f, 0.33f, 70.0f, 80000.0f, 3000.0f, 1000.0f);
	mVehicle.mFrontRightChassis->loadParametres(physics::vec3(0.95f, -0.2f, 1.5f), physics::mat3x3(), 0, -0.3f, 0.33f, 70.0f, 80000.0f, 3000.0f, 1000.0f);
	mVehicle.mRearLeftChassis->loadParametres(physics::vec3(-0.95f, -0.2f, -1.5f), physics::mat3x3(), 0, -0.3f, 0.33f, 70.0f, 80000.0f, 3000.0f, 1000.0f, 100000000.0f);
	mVehicle.mRearRightChassis->loadParametres(physics::vec3(0.95f, -0.2f, -1.5f), physics::mat3x3(), 0, -0.3f, 0.33f, 70.0f, 80000.0f, 3000.0f, 1000.0f, 100000000.0f); 
	
	mVehicle.mFrontLeftChassis->loadFrictionGraphic(frictionValues, 5, 0, peakValue*2.5f);
	mVehicle.mFrontRightChassis->loadFrictionGraphic(frictionValues, 5, 0, peakValue*2.5f);
	mVehicle.mRearLeftChassis->loadFrictionGraphic(frictionValues, 5, 0, peakValue*2.5f);
	mVehicle.mRearRightChassis->loadFrictionGraphic(frictionValues, 5, 0, peakValue*2.5f);

	const int torqueValuesCount = 8;
	float torqueGraphic[torqueValuesCount] = { 0, 0.4f, 0.64f, 0.8f, 0.96f, 1.0f, 0.92f, 0.2f };
	float maxTorque = 800.0f;
	float maxRpm = 7000.0f;
	float engineFriction = 0.04f;
	for (int i = 0; i < torqueValuesCount; i++)
	{
		torqueGraphic[i] = torqueGraphic[i]*maxTorque + maxRpm/(float)(torqueValuesCount - 1)*(float)(i)*engineFriction;
	}

	mVehicle.setEngineParams(torqueGraphic, torqueValuesCount, maxRpm, 1000.0f, 0.6f, engineFriction);

	const int gearsCount = 7;
	float gears[gearsCount] = { -4.0f, 0.0f, 3.64f, 1.95f, 1.36f, 0.94f, 0.78f };
	float topgear = 3.9f;

	mVehicle.setGearBoxParametres(gears, gearsCount, topgear, physics::Vehicle::WD_REAR);
	
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
	mVehicle.update(dt);

	float lastVel = mVelocity;

	mVelocity = mVehicle.mVelocity.len()*3.6f;

	float maxWheelRpm = -mVehicle.mRearLeftChassis->mWheelAngVelocity;
	/*maxWheelRpm = fmax(maxWheelRpm, -mVehicle.mFrontLeftChassis->mWheelAngVelocity);
	maxWheelRpm = fmax(maxWheelRpm, -mVehicle.mFrontRightChassis->mWheelAngVelocity);
	maxWheelRpm = fmax(maxWheelRpm, -mVehicle.mRearRightChassis->mWheelAngVelocity);*/

	mWVelocity = maxWheelRpm*2.0f*3.1415926f*mVehicle.mRearLeftChassis->mWheelRadius*3.6f;

	mSlipFact = mWVelocity - mVelocity;

	mAcceleration = (mVelocity - lastVel)*100.0f;

	std::string gearNames[] = { "R", "N", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
	mVehicleGearName = gearNames[mVehicle.mCurrentGear];

	float fbuf[9];

	mVehicle.getPosition(fbuf);
	vec3 vehiclePos(fbuf[0], fbuf[1], fbuf[2]);

	mVehicle.getOrientation(fbuf);
	mat3x3 vehicleOrient( fbuf[0], fbuf[1], fbuf[2],
		                  fbuf[3], fbuf[4], fbuf[5],
						  fbuf[6], fbuf[7], fbuf[8] );

	AABB vehicleAabb(vehiclePos + vec3(-3.5f, -3.5f, -3.5f),
		             vehiclePos + vec3(3.5f, 3.5f, 3.5f) );

	mTestLandscapeGeom.getPolygons(mVehicle.mPosition + physics::vec3(-2.5f, -2.5f, -2.5f),
		                           mVehicle.mPosition + physics::vec3(2.5f, 2.5f, 2.5f) );

	mVehicle.setPolygonsBuffer(mTestLandscapeGeom.mTestPolygonsBuffer, mTestLandscapeGeom.mVertexBuffer,
		mTestLandscapeGeom.mTestPolygonsBufferCount);

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

	//gLog->fout(1, "TH = %.2f\n", mVehicle.mThrottleCoef);
	
	if (isKeyDown(key_up))
		mVehicle.mThrottleCoef += 1.0f/mControlsReactDelay*dt;
	else
		mVehicle.mThrottleCoef -= 1.0f/mControlsReactDelay*dt;

	mVehicle.mThrottleCoef = fclamp(mVehicle.mThrottleCoef, 0.0f, 1.0f);
	
	if (isKeyDown(key_down))
		mVehicle.mBrakeCoef += 1.0f/mControlsReactDelay*dt;
	else
		mVehicle.mBrakeCoef -= 1.0f/mControlsReactDelay*dt;

	mVehicle.mBrakeCoef = fclamp(mVehicle.mBrakeCoef, 0.0f, 1.0f);
	
	if (isKeyDown(key_numpad_0))
		mVehicle.mHandBrakeCoef += 1.0f/mControlsReactDelay*dt;
	else
		mVehicle.mHandBrakeCoef -= 1.0f/mControlsReactDelay*dt;
	
	mVehicle.mHandBrakeCoef = fclamp(mVehicle.mHandBrakeCoef, 0.0f, 1.0f);

	if (isKeyDown(key_right))
	{
		mVehicle.mSteerWheelAngle += rad(30.0f)*dt/mControlsReactDelay;
	}
	else if (isKeyDown(key_left))
	{
		mVehicle.mSteerWheelAngle -= rad(30.0f)*dt/mControlsReactDelay;
	}
	else mVehicle.mSteerWheelAngle -= sign(mVehicle.mSteerWheelAngle)*
		fmin(rad(30.0f)*dt/mControlsReactDelay, fabs(mVehicle.mSteerWheelAngle));

	mVehicle.mSteerWheelAngle = fclamp(mVehicle.mSteerWheelAngle, rad(-30), rad(30));

	float clutchSmoothCoef = 0.1f;

	mVehicle.mClutchCoef = 0.0f;

	mOffClutchTime -= dt;

	if (isKeyDown(key_c))
	{
		mVehicle.mClutchCoef -= 1.0f/mControlsReactDelay*dt;
		if (mVehicle.mClutchCoef < 0)
			mVehicle.mClutchCoef = 0;

		mOffClutchTime = clutchSmoothCoef;
	}
	else
	{
		if (mOffClutchTime < clutchSmoothCoef)
			mVehicle.mClutchCoef = fclamp(1.0f - mOffClutchTime/clutchSmoothCoef, 0.0f, 1.0f);

		if (mGearOffset != 0 && mOffClutchTime < mOffClutchDelay - clutchSmoothCoef)
		{
			mVehicle.mCurrentGear += mGearOffset;
			mGearOffset = 0;
		}

		if (mOffClutchTime > mOffClutchDelay - clutchSmoothCoef)
			mVehicle.mClutchCoef = fclamp(1.0f - (mOffClutchDelay - mOffClutchTime)/clutchSmoothCoef, 0.0f, 1.0f);
	}

	//mVehicle.mClutchCoef *= mVehicle.mThrottleCoef;
		
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
		getRenderStuff().addGreenArrow(wheelPos, wheelPos + vec3(chassis->mnimp1.x, chassis->mnimp1.y, chassis->mnimp1.z));

		int segmentsCount = 5;

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

	for (int i = 0; i < (int)mPolygonsCount; i++)
	{
		physics::lPolygon* poly = &mPolygonsBuffer[i];
		physics::AABB polyAABB = physics::AABB( 
			physics::vec3( fmin(fmin(mVertexBuffer[poly->a].mPosition.x, mVertexBuffer[poly->b].mPosition.x), mVertexBuffer[poly->c].mPosition.x),
			               fmin(fmin(mVertexBuffer[poly->a].mPosition.y, mVertexBuffer[poly->b].mPosition.y), mVertexBuffer[poly->c].mPosition.y),
				           fmin(fmin(mVertexBuffer[poly->a].mPosition.z, mVertexBuffer[poly->b].mPosition.z), mVertexBuffer[poly->c].mPosition.z)),
		    
		    physics::vec3( fmax(fmax(mVertexBuffer[poly->a].mPosition.x, mVertexBuffer[poly->b].mPosition.x), mVertexBuffer[poly->c].mPosition.x),
			               fmax(fmax(mVertexBuffer[poly->a].mPosition.y, mVertexBuffer[poly->b].mPosition.y), mVertexBuffer[poly->c].mPosition.y),
				           fmax(fmax(mVertexBuffer[poly->a].mPosition.z, mVertexBuffer[poly->b].mPosition.z), mVertexBuffer[poly->c].mPosition.z)));

		if (polyAABB.isIntersect(aabb))
		{
			mTestPolygonsBuffer[mTestPolygonsBufferCount++] = poly;

			getRenderStuff().addBlueArrow(*((vec3*)(&mVertexBuffer[poly->a].mPosition)),
				                          *((vec3*)(&mVertexBuffer[poly->b].mPosition)) );

			getRenderStuff().addBlueArrow(*((vec3*)(&mVertexBuffer[poly->b].mPosition)),
				                          *((vec3*)(&mVertexBuffer[poly->c].mPosition)) );

			getRenderStuff().addBlueArrow(*((vec3*)(&mVertexBuffer[poly->c].mPosition)),
				                          *((vec3*)(&mVertexBuffer[poly->a].mPosition)) );

			if (mTestPolygonsBufferCount == mTestPolygonsBufferSize)
				break;
		}
	}
}
