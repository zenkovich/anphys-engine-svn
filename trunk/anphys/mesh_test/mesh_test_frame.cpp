#include "mesh_test_frame.h"

#include "engine/engine_incl.h"
#include "util/debug/render_stuff.h"

#include "mesh_test.h"


apMeshTestFrame::apMeshTestFrame( const std::string& title, fRect wndRect, fRect outputRect ):
	apRenderWindow(title, wndRect, outputRect)
{
	onCreate(mInRect);
}

apMeshTestFrame::apMeshTestFrame():
	apRenderWindow()
{
	onCreate(mInRect);
}

apMeshTestFrame::apMeshTestFrame( const std::string& title, fRect wndRect ):
	apRenderWindow(title, wndRect)
{
	onCreate(mInRect);
}

apMeshTestFrame::~apMeshTestFrame()
{
	safe_release(m3DRenderState);
	safe_release(mCamera);
	safe_release(m2DRenderState);
	safe_release(mWidgetsManager);
}

void apMeshTestFrame::onCreate( fRect inRect )
{
	mCamera          = new grCamera3DMouse;
	m3DRenderState   = new grSimple3DRenderState(mRender);
	m2DRenderState   = new gr2DRenderState(mRender);
	mMainEngineScene = new cScene(this);

	m3DRenderState->mPolygonCullMode = grRenderState::PCM_COUNETRCLOCKWISE;

	m3DRenderState->bindCamera(mCamera);

	createMaterials();

	initializeUI();

	mMeshTest = new cMeshTest;
	
	mMainMeshes.push_back(new grRender3DObjectMesh(mMainEngineScene->mRenderScene->mObjects, 32000, 32000));
	mSecondaryMesh = new grRender3DObjectMesh(mMainEngineScene->mRenderScene->mObjects, 32000, 32000);
	
	mMainEngineScene->mRenderScene->mObjects->createObject(mMainMeshes[0]);
	mMainEngineScene->mRenderScene->mObjects->createObject(mSecondaryMesh);

	mMeshTest->generateMainPlaneMesh(vec3(100, 100, 0), 100, 100);
	mMeshTest->randomizeMainMesh(vec3(0.2f, 0.2f, 0.2f));
	mMeshTest->fillMainMeshData(mMainMeshes, "mainMesh");

	mMeshTest->generateSecondaryTorusMesh(vec3(4, 2, 10), 50, 50);
	mMeshTest->randomizeSecondaryMesh(vec3(0.2f, 0.2f, 0.2f));
	mMeshTest->fillSecondaryMeshData(mSecondaryMesh, "mainMesh");

	mMainMeshSize = vec3(100, 100, 0);
	mMainMeshRandomize = vec3(0.2f, 0.2f, 0.2f);
	mMainMeshXSegments = 10; mMainMeshZSegments = 10;
	
	mSecondaryPlaneChecked = true;
	mSecondaryTorusChecked = false;
	mSecondarySphereChecked = false;

	mSecondaryMeshSize = vec3(100, 100, 0);
	mSecondaryMeshRandomize = vec3(0.2f, 0.2f, 0.2f);
	mSecondaryMeshXSegments = 10; mSecondaryMeshZSegments = 10;

	mSecondaryTorusSize = vec3(4, 4, 10);
	mSecondaryTorusRandomize = vec3(0.2f, 0.2f, 0.2f);
	mSecondaryTorusCircles = 10;
	mSecondaryTorusCircleSegs = 10;
}

void apMeshTestFrame::onClose()
{
	mEngine->exitEngine();
}

float apMeshTestFrame::onTimer()
{	
	float dt = apRenderWindow::onTimer();

	updateCameraContorls();

	if (mInputMessenger->mInputMessage.isKeyPressed(key_t))
		mMainWindow->show();

	mWidgetsManager->mLastInputMessage = &mInputMessenger->mInputMessage;
	mWidgetsManagerRes = mWidgetsManager->processInputMessage(mInputMessenger->mInputMessage);
	mInputMessenger->mInputMessage.update();

	mMainEngineScene->update(dt);
	mRender->update(dt);
	mCamera->update(dt);
	updateSecMeshPositioning();
	mWidgetsManager->update(dt);

	mRender->beginRender();
	mRender->bindRenderState(m3DRenderState);
	mRender->render();

	mRender->bindRenderState(m2DRenderState);
	//render2D();

	mWidgetsManager->draw();

	mRender->endRender();

	return dt;
}

void apMeshTestFrame::onSize( fRect inRect )
{
	mRender->resize(inRect.getSize());
}

void apMeshTestFrame::onMouseMove( vec2 point )
{
	if (mInputMessenger->mInputMessage.isKeyDown(CURSOR_BUTTON) && mWidgetsManagerRes == 0)
		mCamera->mouseMoved(point - mInputMessenger->mInputMessage.mCursorPosition);
}

void apMeshTestFrame::onKeyDown( int key )
{

}

void apMeshTestFrame::updateCameraContorls()
{
	if (isKeyDown(key_w)) mCamera->moveForward(isKeyDown(key_shift));
	if (isKeyDown(key_s)) mCamera->moveBack(isKeyDown(key_shift));
	if (isKeyDown(key_a)) mCamera->moveLeft(isKeyDown(key_shift));
	if (isKeyDown(key_d)) mCamera->moveRight(isKeyDown(key_shift));
	if (isKeyDown(key_ctrl)) mCamera->moveDown(isKeyDown(key_shift));
	if (isKeyDown(key_space)) mCamera->moveUp(isKeyDown(key_shift));
}

void apMeshTestFrame::initializeUI()
{
	mWidgetsManager = new uiWidgetsManager(mRender);
	mInputMessenger->registInputListener(mWidgetsManager);

//main panel
	mMainWindow = uiSimpleStuff::createWindow(mWidgetsManager, "mainWnd", vec2(0, 0), vec2(300, 440), "Main panel");
	uiSimpleStuff::createSizeEffect(mMainWindow);

//main mesh
	mMainWindow->addChild( uiSimpleStuff::createLabel(mWidgetsManager, vec2(10, 10), vec2(20, 20), "", "Main mesh plane"));
	

	mMainWindow->addChild( uiSimpleStuff::createLabel(mWidgetsManager, vec2(10, 30), vec2(20, 20), "", "size x:") );
	mMainWindow->addChild( uiSimpleStuff::createTextEdit(mWidgetsManager, vec2(70, 30), vec2(50, 22), "mainSizeX") );

	mMainWindow->addChild( uiSimpleStuff::createLabel(mWidgetsManager, vec2(120, 30), vec2(20, 20), "", "z:") );
	mMainWindow->addChild( uiSimpleStuff::createTextEdit(mWidgetsManager, vec2(140, 30), vec2(50, 22), "mainSizeZ") );


	mMainWindow->addChild( uiSimpleStuff::createLabel(mWidgetsManager, vec2(10, 55), vec2(20, 20), "", "seg x:") );
	mMainWindow->addChild( uiSimpleStuff::createTextEdit(mWidgetsManager, vec2(70, 55), vec2(50, 22), "mainSegmX") );

	mMainWindow->addChild( uiSimpleStuff::createLabel(mWidgetsManager, vec2(120, 55), vec2(20, 20), "", "z:") );
	mMainWindow->addChild( uiSimpleStuff::createTextEdit(mWidgetsManager, vec2(140, 55), vec2(50, 22), "mainSegmZ") );


	mMainWindow->addChild( uiSimpleStuff::createLabel(mWidgetsManager, vec2(10, 80), vec2(20, 20), "", "rand x:") );
	mMainWindow->addChild( uiSimpleStuff::createTextEdit(mWidgetsManager, vec2(70, 80), vec2(50, 22), "mainRandX") );

	mMainWindow->addChild( uiSimpleStuff::createLabel(mWidgetsManager, vec2(120, 80), vec2(20, 20), "", "y:") );
	mMainWindow->addChild( uiSimpleStuff::createTextEdit(mWidgetsManager, vec2(140, 80), vec2(50, 22), "mainRandY") );

	mMainWindow->addChild( uiSimpleStuff::createLabel(mWidgetsManager, vec2(190, 80), vec2(20, 20), "", "z:") );
	mMainWindow->addChild( uiSimpleStuff::createTextEdit(mWidgetsManager, vec2(210, 80), vec2(50, 22), "mainRandZ") );

	mMainWindow->addChild( (uiWidget*)uiSimpleStuff::createButton(mWidgetsManager, vec2(10, 105), vec2(50, 22), "mainReset", "reset", NULL) );

//sec mesh
	mMainWindow->addChild( uiSimpleStuff::createLabel(mWidgetsManager, vec2(10, 130), vec2(20, 20), "", "Secondary mesh") );
	mMainWindow->addChild( uiSimpleStuff::createCheckbox(mWidgetsManager, "secPlaneChk", vec2(10, 155), "plane", true) );
	mMainWindow->addChild( uiSimpleStuff::createCheckbox(mWidgetsManager, "secTorusChk", vec2(80, 155), "torus", false) );
	mMainWindow->addChild( uiSimpleStuff::createCheckbox(mWidgetsManager, "secSphereChk", vec2(150, 155), "sphere", false) );
	
//plane
	uiWidget* planeSettingsWidget = new uiWidget(mWidgetsManager, "planeSettings");
	planeSettingsWidget->setPosition(vec2(0, 180));
	
	planeSettingsWidget->addChild( uiSimpleStuff::createLabel(mWidgetsManager, vec2(10, 0), vec2(20, 20), "", "size x:") );
	planeSettingsWidget->addChild( uiSimpleStuff::createTextEdit(mWidgetsManager, vec2(70, 0), vec2(50, 22), "mainSizeX") );

	planeSettingsWidget->addChild( uiSimpleStuff::createLabel(mWidgetsManager, vec2(120, 0), vec2(20, 20), "", "z:") );
	planeSettingsWidget->addChild( uiSimpleStuff::createTextEdit(mWidgetsManager, vec2(140, 0), vec2(50, 22), "mainSizeZ") );


	planeSettingsWidget->addChild( uiSimpleStuff::createLabel(mWidgetsManager, vec2(10, 25), vec2(20, 20), "", "seg x:") );
	planeSettingsWidget->addChild( uiSimpleStuff::createTextEdit(mWidgetsManager, vec2(70, 25), vec2(50, 22), "mainSegmX") );

	planeSettingsWidget->addChild( uiSimpleStuff::createLabel(mWidgetsManager, vec2(120, 25), vec2(20, 20), "", "z:") );
	planeSettingsWidget->addChild( uiSimpleStuff::createTextEdit(mWidgetsManager, vec2(140, 25), vec2(50, 22), "mainSegmZ") );


	planeSettingsWidget->addChild( uiSimpleStuff::createLabel(mWidgetsManager, vec2(10, 50), vec2(20, 20), "", "rand x:") );
	planeSettingsWidget->addChild( uiSimpleStuff::createTextEdit(mWidgetsManager, vec2(70, 50), vec2(50, 22), "mainRandX") );

	planeSettingsWidget->addChild( uiSimpleStuff::createLabel(mWidgetsManager, vec2(120, 50), vec2(20, 20), "", "y:") );
	planeSettingsWidget->addChild( uiSimpleStuff::createTextEdit(mWidgetsManager, vec2(140, 50), vec2(50, 22), "mainRandY") );

	planeSettingsWidget->addChild( uiSimpleStuff::createLabel(mWidgetsManager, vec2(190, 50), vec2(20, 20), "", "z:") );
	planeSettingsWidget->addChild( uiSimpleStuff::createTextEdit(mWidgetsManager, vec2(210, 50), vec2(50, 22), "mainRandZ") );
	
	mMainWindow->addChild( planeSettingsWidget);

//torus
	uiWidget* torusSettingsWidget = new uiWidget(mWidgetsManager, "torusSettings");
	torusSettingsWidget->setPosition(vec2(0, 180));
	
	torusSettingsWidget->addChild( uiSimpleStuff::createLabel(mWidgetsManager, vec2(10, 0), vec2(20, 20), "", "rad x:") );
	torusSettingsWidget->addChild( uiSimpleStuff::createTextEdit(mWidgetsManager, vec2(70, 0), vec2(50, 22), "radX") );

	torusSettingsWidget->addChild( uiSimpleStuff::createLabel(mWidgetsManager, vec2(120, 0), vec2(20, 20), "", "rad y:") );
	torusSettingsWidget->addChild( uiSimpleStuff::createTextEdit(mWidgetsManager, vec2(180, 0), vec2(50, 22), "radY") );

	torusSettingsWidget->addChild( uiSimpleStuff::createLabel(mWidgetsManager, vec2(10, 25), vec2(20, 20), "", "rad c:") );
	torusSettingsWidget->addChild( uiSimpleStuff::createTextEdit(mWidgetsManager, vec2(70, 25), vec2(50, 22), "radC") );


	torusSettingsWidget->addChild( uiSimpleStuff::createLabel(mWidgetsManager, vec2(10, 50), vec2(20, 20), "", "circles") );
	torusSettingsWidget->addChild( uiSimpleStuff::createTextEdit(mWidgetsManager, vec2(70, 50), vec2(50, 22), "circles") );

	torusSettingsWidget->addChild( uiSimpleStuff::createLabel(mWidgetsManager, vec2(120, 50), vec2(20, 20), "", "segs") );
	torusSettingsWidget->addChild( uiSimpleStuff::createTextEdit(mWidgetsManager, vec2(180, 50), vec2(50, 22), "segments") );


	torusSettingsWidget->addChild( uiSimpleStuff::createLabel(mWidgetsManager, vec2(10, 75), vec2(20, 20), "", "rand x:") );
	torusSettingsWidget->addChild( uiSimpleStuff::createTextEdit(mWidgetsManager, vec2(70, 75), vec2(50, 22), "mainRandX") );

	torusSettingsWidget->addChild( uiSimpleStuff::createLabel(mWidgetsManager, vec2(120, 75), vec2(20, 20), "", "y:") );
	torusSettingsWidget->addChild( uiSimpleStuff::createTextEdit(mWidgetsManager, vec2(140, 75), vec2(50, 22), "mainRandY") );

	torusSettingsWidget->addChild( uiSimpleStuff::createLabel(mWidgetsManager, vec2(190, 75), vec2(20, 20), "", "z:") );
	torusSettingsWidget->addChild( uiSimpleStuff::createTextEdit(mWidgetsManager, vec2(210, 75), vec2(50, 22), "mainRandZ") );

	mMainWindow->addChild( torusSettingsWidget);
	
//main binding
	mMainWindow->getWidgetByType<uiTextEdit>("mainSizeX")->bindValue(&mMainMeshSize.x);
	mMainWindow->getWidgetByType<uiTextEdit>("mainSizeZ")->bindValue(&mMainMeshSize.y);
	
	mMainWindow->getWidgetByType<uiTextEdit>("mainSegmX")->bindValue(&mMainMeshXSegments);
	mMainWindow->getWidgetByType<uiTextEdit>("mainSegmZ")->bindValue(&mMainMeshZSegments);
	
	mMainWindow->getWidgetByType<uiTextEdit>("mainRandX")->bindValue(&mMainMeshRandomize.x);
	mMainWindow->getWidgetByType<uiTextEdit>("mainRandY")->bindValue(&mMainMeshRandomize.y);
	mMainWindow->getWidgetByType<uiTextEdit>("mainRandZ")->bindValue(&mMainMeshRandomize.z);

	mMainWindow->getWidgetByType<uiButton>("mainReset")->setCallback(new cCallback<apMeshTestFrame>(this, &apMeshTestFrame::resetMainMesh));
	
//sec binding plane
	planeSettingsWidget->getWidgetByType<uiTextEdit>("mainSizeX")->bindValue(&mSecondaryMeshSize.x);
	planeSettingsWidget->getWidgetByType<uiTextEdit>("mainSizeZ")->bindValue(&mSecondaryMeshSize.y);
	
	planeSettingsWidget->getWidgetByType<uiTextEdit>("mainSegmX")->bindValue(&mSecondaryMeshXSegments);
	planeSettingsWidget->getWidgetByType<uiTextEdit>("mainSegmZ")->bindValue(&mSecondaryMeshZSegments);
	
	planeSettingsWidget->getWidgetByType<uiTextEdit>("mainRandX")->bindValue(&mSecondaryMeshRandomize.x);
	planeSettingsWidget->getWidgetByType<uiTextEdit>("mainRandY")->bindValue(&mSecondaryMeshRandomize.y);
	planeSettingsWidget->getWidgetByType<uiTextEdit>("mainRandZ")->bindValue(&mSecondaryMeshRandomize.z);

//sec binding torus
	torusSettingsWidget->getWidgetByType<uiTextEdit>("radX")->bindValue(&mSecondaryTorusSize.x);
	torusSettingsWidget->getWidgetByType<uiTextEdit>("radY")->bindValue(&mSecondaryTorusSize.y);
	torusSettingsWidget->getWidgetByType<uiTextEdit>("radC")->bindValue(&mSecondaryTorusSize.z);
	torusSettingsWidget->getWidgetByType<uiTextEdit>("circles")->bindValue(&mSecondaryTorusCircles);
	torusSettingsWidget->getWidgetByType<uiTextEdit>("segments")->bindValue(&mSecondaryTorusCircleSegs);
	torusSettingsWidget->getWidgetByType<uiTextEdit>("mainRandX")->bindValue(&mSecondaryTorusRandomize.x);
	torusSettingsWidget->getWidgetByType<uiTextEdit>("mainRandY")->bindValue(&mSecondaryTorusRandomize.y);
	torusSettingsWidget->getWidgetByType<uiTextEdit>("mainRandZ")->bindValue(&mSecondaryTorusRandomize.z);	

	mMainWindow->addChild( uiSimpleStuff::createButton(mWidgetsManager, vec2(10, 300), vec2(50, 22), "", "reset", 
		new cCallback<apMeshTestFrame>(this, &apMeshTestFrame::resetSecondaryMesh)) );
	
	mMainWindow->getWidgetByType<uiCheckBox>("secPlaneChk")->bindValue(&mSecondaryPlaneChecked)->setChangeValueCallback(
		new cCallback<apMeshTestFrame>(this, &apMeshTestFrame::planeChecked));
	mMainWindow->getWidgetByType<uiCheckBox>("secTorusChk")->bindValue(&mSecondaryTorusChecked)->setChangeValueCallback(
		new cCallback<apMeshTestFrame>(this, &apMeshTestFrame::torusChecked));
	mMainWindow->getWidgetByType<uiCheckBox>("secSphereChk")->bindValue(&mSecondarySphereChecked)->setChangeValueCallback(
		new cCallback<apMeshTestFrame>(this, &apMeshTestFrame::sphereChecked));

	mMainWindow->addChild( uiSimpleStuff::createButton(mWidgetsManager, vec2(10, 330), vec2(280, 22), "", "merge", 
		new cCallback<apMeshTestFrame>(this, &apMeshTestFrame::mergeMeshes)) );

	mWidgetsManager->addWidget( mMainWindow );
	mMainWindow->show();
}

void apMeshTestFrame::createMaterials()
{
	grSurfaceMaterial* surfaceMaterial = mRender->mSurfaceMaterials->addSurfaceMaterial(
		new grSurfaceMaterial("mainMesh"));
	surfaceMaterial->pushTexture(mRender->mTextures->createTexture("../data/textures/wood.jpg"));
	surfaceMaterial->setMaterial(mRender->mMaterials->getMaterial("whiteMaterial"));
	surfaceMaterial->setShadeModel(NULL);
}

void apMeshTestFrame::updateSecMeshPositioning()
{
	vec3 camYV(0, 1, 0);
	vec3 camZV = mCamera->mDirection;
	vec3 camXV = mCamera->mDirection^camYV;
	camYV = camXV^camZV;
	
	camXV = camXV.normalize();
	camYV = camYV.normalize();
	camZV = camZV.normalize();

	mat3x3 cameraOrient(camXV.x, camXV.y, camXV.z,
		                camYV.x, camYV.y, camYV.z,
						camZV.x, camZV.y, camZV.z);

	vec3 cameraPos = mCamera->mPosition;

	if (mInputMessenger->mInputMessage.isKeyDown(RM_BUTTON))
	{
		mat3x3 orientDiff = mLastCameraOrient.inverse()*cameraOrient;
		vec3 posDiff = cameraPos - mLastCameraPos;

		vec3 pd = (mSecondaryMesh->mPosition - cameraPos)*orientDiff + cameraPos;

		mSecondaryMesh->mPosition = pd + posDiff;
		mSecondaryMesh->mOrient = mSecondaryMesh->mOrient*orientDiff;
	}

	mLastCameraPos = cameraPos;
	mLastCameraOrient = cameraOrient;
}

void apMeshTestFrame::resetMainMesh()
{
	mMeshTest->generateMainPlaneMesh(mMainMeshSize, mMainMeshXSegments, mMainMeshZSegments);
	mMeshTest->randomizeMainMesh(mMainMeshRandomize);
	mMeshTest->fillMainMeshData(mMainMeshes, "mainMesh");
}

void apMeshTestFrame::planeChecked()
{
	if (mSecondaryPlaneChecked)
		mSecondarySphereChecked = mSecondaryTorusChecked = false;

	if (!mSecondaryPlaneChecked && !mSecondarySphereChecked && !mSecondaryTorusChecked)
		mSecondaryPlaneChecked = true;

	updateVisibleSecMeshSettingsWidget();
}

void apMeshTestFrame::torusChecked()
{
	if (mSecondaryTorusChecked)
		mSecondarySphereChecked = mSecondaryPlaneChecked = false;
	
	if (!mSecondaryPlaneChecked && !mSecondarySphereChecked && !mSecondaryTorusChecked)
		mSecondaryTorusChecked = true;

	updateVisibleSecMeshSettingsWidget();
}

void apMeshTestFrame::sphereChecked()
{
	if (mSecondarySphereChecked)
		mSecondaryPlaneChecked = mSecondaryTorusChecked = false;
	
	if (!mSecondaryPlaneChecked && !mSecondarySphereChecked && !mSecondaryTorusChecked)
		mSecondarySphereChecked = true;

	updateVisibleSecMeshSettingsWidget();
}

void apMeshTestFrame::updateVisibleSecMeshSettingsWidget()
{
	if (mSecondaryPlaneChecked)
	{
		mMainWindow->getWidget("planeSettings")->show();
		mMainWindow->getWidget("torusSettings")->hide();
	}
	if (mSecondarySphereChecked)
	{
		mMainWindow->getWidget("planeSettings")->hide();
		mMainWindow->getWidget("torusSettings")->hide();
	}
	if (mSecondaryTorusChecked)
	{
		mMainWindow->getWidget("planeSettings")->hide();
		mMainWindow->getWidget("torusSettings")->show();
	}
}

void apMeshTestFrame::resetSecondaryMesh()
{
	if (mSecondaryPlaneChecked)
	{
		mMeshTest->generateSecondaryPlaneMesh(mSecondaryMeshSize, mSecondaryMeshXSegments, mSecondaryMeshZSegments);
		mMeshTest->randomizeSecondaryMesh(mSecondaryMeshRandomize);
	}

	if (mSecondaryTorusChecked)
	{
		mMeshTest->generateSecondaryTorusMesh(mSecondaryTorusSize, mSecondaryTorusCircleSegs, mSecondaryTorusCircles);
		mMeshTest->randomizeSecondaryMesh(mSecondaryTorusRandomize);
	}

	mMeshTest->fillSecondaryMeshData(mSecondaryMesh, "mainMesh");
}

void apMeshTestFrame::mergeMeshes()
{
	mMeshTest->processMeshMerge(mSecondaryMesh->mPosition, mSecondaryMesh->mOrient);
	
	mMeshTest->fillMainMeshData(mMainMeshes, "mainMesh");
	
	mSecondaryMesh->mPosition = vec3(0, 0, 0);
	mSecondaryMesh->mOrient = nullMatr();
}
