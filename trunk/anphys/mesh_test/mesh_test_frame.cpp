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
	
	mMainMesh = new grRender3DObjectMesh(mMainEngineScene->mRenderScene->mObjects, 32000, 32000);
	mSecondaryMesh = new grRender3DObjectMesh(mMainEngineScene->mRenderScene->mObjects, 32000, 32000);
	
	mMainEngineScene->mRenderScene->mObjects->createObject(mMainMesh);
	mMainEngineScene->mRenderScene->mObjects->createObject(mSecondaryMesh);

	mMeshTest->generateMainPlaneMesh(vec3(100, 100, 0), 100, 100);
	mMeshTest->randomizeMainMesh(vec3(0.2f, 0.2f, 0.2f));
	mMeshTest->fillMainMeshData(mMainMesh, "mainMesh");

	mMeshTest->generateSecondaryTorusMesh(vec3(4, 2, 10), 50, 50);
	mMeshTest->randomizeSecondaryMesh(vec3(0.2f, 0.2f, 0.2f));
	mMeshTest->fillSecondaryMeshData(mSecondaryMesh, "mainMesh");
}

void apMeshTestFrame::onClose()
{
	mEngine->exitEngine();
}

float apMeshTestFrame::onTimer()
{	
	float dt = apRenderWindow::onTimer();

	updateCameraContorls();

	mWidgetsManager->mLastInputMessage = &mInputMessenger->mInputMessage;
	mWidgetsManager->processInputMessage(mInputMessenger->mInputMessage);
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

	mRender->endRender();

	return dt;
}

void apMeshTestFrame::onSize( fRect inRect )
{
	mRender->resize(inRect.getSize());
}

void apMeshTestFrame::onMouseMove( vec2 point )
{
	if (mInputMessenger->mInputMessage.isKeyDown(CURSOR_BUTTON))
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
