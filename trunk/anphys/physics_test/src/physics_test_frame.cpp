#include "physics_test_frame.h"

#include "engine/engine_incl.h"
#include "util/debug/render_stuff.h"

#include "physics/CD/collision_detection/box_box_cd.h"
#include "physics/CD/collision_geometry/box_collision_geometry.h"
#include "physics/CD/collision_geometry/collision_geometry.h"
#include "physics/CD/collision_geometry/collision.h"
#include "physics/CD/collision_geometry/collision_point.h"

#include "render/render_objects/2d/sprite.h"
#include "ui/ui_widget.h"
#include "ui/ui_manager.h"
#include "ui/ui_state.h"
#include "ui/ui_sprite.h"
#include "ui/ui_font.h"
#include "ui/ui_label.h"
#include "ui/ui_simple_stuff.h"
#include "ui/ui_scrollbar.h"
#include "ui/ui_scroll_area.h"
#include "ui/ui_text_edit.h"
#include "ui/ui_lines_geometry.h"
#include "ui/ui_border.h"
#include "ui/ui_window.h"

#include "util/other/callback.h"

#include "input/input_messenger.h"

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
	safe_release(mTestWidgetsManager);
}

void apPhysicsTestFrame::onCreate(fRect inRect)
{
	mCamera3dMouse = new grCamera3DMouse;
	m2DCamera = new grCamera2D;
	m3DRenderState = new grSimple3DRenderState(mRender);
	m2DRenderState = new gr2DRenderState(mRender);

	mStencilTest1 = new grStencilBufferRenderTarget(mRender);
	mStencilTest2 = new grStencilBufferRenderTarget(mRender, vec2(200, 200));

	mTestSprite = new grSprite(mRender, NULL);
	mTestSprite->setSize(vec2(100, 200)).setPosition(vec2(30, 50));

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

	createTestWidgets();

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

	mTestLabel->setText(formatStr("progress = %.3f", mScrollbar->mCurrentValue));

	mMainEngineScene->update(dt);
	mRender->update(dt);
	mCamera3dMouse->update(dt);
	mTestWidgetsManager->update(dt);

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
		mTestWindow->show();
	}
	if (key == key_y)
	{
		mPhysicsRunByStep = !mPhysicsRunByStep;
		mTestWindow->hide();
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

	/*if (key == 'Q') mTestFont->setHorAlign(uiFont::AL_LEFT);
	if (key == 'W') mTestFont->setHorAlign(uiFont::AL_CENTER);
	if (key == 'E') mTestFont->setHorAlign(uiFont::AL_RIGHT);

	if (key == 'A') mTestFont->setVerAlign(uiFont::AL_TOP);
	if (key == 'S') mTestFont->setVerAlign(uiFont::AL_MIDDLE);
	if (key == 'D') mTestFont->setVerAlign(uiFont::AL_BOTTOM);

	if (key == 'Z') mTestFont->setScale(vec2(mTestFont->getScale().x + 0.01f, mTestFont->getScale().y));
	if (key == 'X') mTestFont->setScale(vec2(mTestFont->getScale().x - 0.01f, mTestFont->getScale().y));
	if (key == 'C') mTestFont->setScale(vec2(mTestFont->getScale().x, mTestFont->getScale().y + 0.01f));
	if (key == 'V') mTestFont->setScale(vec2(mTestFont->getScale().x, mTestFont->getScale().y - 0.01f));

	if (key == 'G') mTestFont->setDistCoef(vec2(mTestFont->getDistCoef().x + 0.01f, mTestFont->getDistCoef().y));
	if (key == 'H') mTestFont->setDistCoef(vec2(mTestFont->getDistCoef().x - 0.01f, mTestFont->getDistCoef().y));
	if (key == 'J') mTestFont->setDistCoef(vec2(mTestFont->getDistCoef().x, mTestFont->getDistCoef().y + 0.01f));
	if (key == 'K') mTestFont->setDistCoef(vec2(mTestFont->getDistCoef().x, mTestFont->getDistCoef().y - 0.01f));
	*/
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

	mRender->bindStencilBuffer(mStencilTest2);
	mTestWidgetsManager->draw();
	mRender->unbindStencilBuffer();

	/*mTestFont->draw();
	m2DRenderState->pushLine(mTestFont->getTextArea().getltCorner(), mTestFont->getTextArea().getrtCorner());
	m2DRenderState->pushLine(mTestFont->getTextArea().getrtCorner(), mTestFont->getTextArea().getrdCorner());
	m2DRenderState->pushLine(mTestFont->getTextArea().getrdCorner(), mTestFont->getTextArea().getldCorner());
	m2DRenderState->pushLine(mTestFont->getTextArea().getldCorner(), mTestFont->getTextArea().getltCorner());*/
}

void click()
{
	gLog->fout(1, "click\n");
}

void apPhysicsTestFrame::createTestWidgets()
{
	mTestWidgetsManager = new uiWidgetsManager(mRender);
	mInputMessenger->registInputListener(mTestWidgetsManager);

	mTestWidget = uiSimpleStuff::createSpriteWidget(mTestWidgetsManager, uiSimpleStuff::mColor1, 
		                                            vec2(20, 30), vec2(300, 500), "main");
	
	uiButton* testButton = uiSimpleStuff::createButton(mTestWidgetsManager, vec2(30, 40), vec2(200, 50),
		"button1", "Test button", new cCallback<>(&click));

	mScrollbar = uiSimpleStuff::createScrollbar(mTestWidgetsManager, vec2(10, 150),
		vec2(200, 15), "scroller", (int)uiScrollbar::ST_HORISONTAL, -10.0f, 10.0f);

	
	/*mScrollbar = uiSimpleStuff::createScrollbar(mTestWidgetsManager, vec2(250, 150),
	vec2(15, 200), "scroller", (int)uiScrollbar::ST_VERTICAL, -10.0f, 10.0f);*/
	

	mScrollbar->setScrollerSize(5.0f);

	mTestLabel = uiSimpleStuff::createLabel(mTestWidgetsManager, vec2(10, 200), vec2(200, 30), "tl", "no text");

	mTestTextEdit = uiSimpleStuff::createTextEdit(mTestWidgetsManager, vec2(10, 240), vec2(200, 22), "testEditText");

	mTestScrollarea = uiSimpleStuff::createScrollarea(mTestWidgetsManager, vec2(10, 10), vec2(280, 150), 
		"testScrollarea");

	uiLinesGeometry* testLinesGeometry = new uiLinesGeometry(mTestWidgetsManager, "nn");
	testLinesGeometry->addLine(vec2(10, 10), vec2(20, 20));
	testLinesGeometry->addLine(vec2(20, 10), vec2(10, 20));

	/*uiBorder* testBorder1 = uiSimpleStuff::createBorder(mTestWidgetsManager, "b1", vec2(10, 210), vec2(280, 200),
		(int)uiBorder::LT_SINGLE, "123");
*/
	mTestWindow = uiSimpleStuff::createWindow(mTestWidgetsManager, "SimpleWindow", vec2(10, 10), vec2(150, 150),
		"Simple Window");

	mTestWindow->addChild((uiWidget*)testButton);
	mTestWindow->addChild((uiWidget*)mScrollbar);
	mTestWindow->addChild((uiWidget*)mTestLabel);
	mTestWindow->addChild((uiWidget*)mTestTextEdit);
	mTestWindow->addChild((uiWidget*)testLinesGeometry);
	mTestWidget->addChild((uiWidget*)mTestScrollarea);
	//mTestWidget->addChild((uiWidget*)testBorder1);
	uiSimpleStuff::createSizeEffect(mTestWindow, 1.0f);
	
	mTestWidgetsManager->addWidget(mTestWindow);

	mTestWindow->show(true);

	/*mTestWidget = new uiWidget(mTestWidgetsManager, "testWidget");
	mTestWidgetsManager->addWidget(mTestWidget);

	mTestWidget->setPosition(vec2(10, 10));
	mTestWidget->setSize(vec2(100, 100));

	grTexture* tex1 = mRender->mTextures->createTexture("textures/pngtest");
	grSprite* testSprite = new grSprite(mRender, tex1);
	uiSpriteWidget* spriteWidget = new uiSpriteWidget(mTestWidgetsManager, testSprite, true, "spriteWidget");

	mTestWidget->addChild(spriteWidget);

	spriteWidget->setPosition(vec2(20, 20));
	spriteWidget->show(true);*/

	/*mTestFont = new uiFont(mRender);
	mTestFont->loadWelloreFormat("fonts/test_font.txt");
	/*mTestFont->load("fonts/system_font", "font");

	mTestFont->setTextArea(fRect(100, 100, 300, 300));
	mTestFont->setText("Some text\nwith\nmany\nlines\na\na\n\na\n12345678.8765432234523423452342352345\n1231231\n12123123");
	mTestFont->setHorAlign(uiFont::AL_LEFT);
	mTestFont->setVerAlign(uiFont::AL_MIDDLE);*/
}

void apPhysicsTestFrame::addScrollareaWidget()
{

}
