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
	mMainMeshXSegments = 100; mMainMeshZSegments = 100;
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

	mMainWindow = uiSimpleStuff::createWindow(mWidgetsManager, "mainWnd", vec2(0, 0), vec2(300, 300), "Main panel");
	uiSimpleStuff::createSizeEffect(mMainWindow);

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
	
	mMainWindow->getWidgetByType<uiTextEdit>("mainSizeX")->bindValue(&mMainMeshSize.x);
	mMainWindow->getWidgetByType<uiTextEdit>("mainSizeZ")->bindValue(&mMainMeshSize.y);
	
	mMainWindow->getWidgetByType<uiTextEdit>("mainSegmX")->bindValue(&mMainMeshXSegments);
	mMainWindow->getWidgetByType<uiTextEdit>("mainSegmZ")->bindValue(&mMainMeshZSegments);
	
	mMainWindow->getWidgetByType<uiTextEdit>("mainRandX")->bindValue(&mMainMeshRandomize.x);
	mMainWindow->getWidgetByType<uiTextEdit>("mainRandY")->bindValue(&mMainMeshRandomize.y);
	mMainWindow->getWidgetByType<uiTextEdit>("mainRandZ")->bindValue(&mMainMeshRandomize.z);
	
	mMainWindow->getWidgetByType<uiButton>("mainReset")->setCallback(new cCallback<apMeshTestFrame>(this, &apMeshTestFrame::resetMainMesh));

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
