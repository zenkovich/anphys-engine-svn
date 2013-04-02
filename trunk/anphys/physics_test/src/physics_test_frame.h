#ifndef PHYSICS_TEST_FRAME_H
#define PHYSICS_TEST_FRAME_H

#include "engine/engine_options.h"

#ifdef PLATFORM_WIN32
#include "engine/frame_system/win32_render_frame/render_frame_win32_window.h"
#endif //PLATFORM_WIN32

struct grCamera3DMouse;
struct grRenderSceneBaseInterface;
struct cScene;
struct cObject;
struct grRender2DObjectMesh;
struct grTexture;
struct grTextureRenderTarget;
struct grSimple3DRenderState;
struct gr2DRenderState;
struct grCamera2D;

struct apPhysicsTestFrame:public apRenderWindow
{
	grCamera3DMouse*            mCamera3dMouse;
	grCamera2D*                 m2DCamera;
	grSimple3DRenderState*      m3DRenderState;
	gr2DRenderState*            m2DRenderState;

	cScene*                     mMainEngineScene;
	
	bool                        mPhysicsRunning;
	bool                        mPhysicsRunByStep;

	//tests
	grRender2DObjectMesh*       mTest2DMesh;
	grRender2DObjectMesh*       mTest2DMesh2;

	grTexture*                  mRenderTexture;
	grTextureRenderTarget*      mTextureRenderTarget;

//functions
	apPhysicsTestFrame();
	apPhysicsTestFrame(const std::string& title, fRect wndRect, fRect outputRect);
	apPhysicsTestFrame(const std::string& title, fRect wndRect);

	~apPhysicsTestFrame();

	void onCreate(fRect inRect);
	void onClose();
	float onTimer();
	void onSize(fRect inRect);
	
	void onMouseLeftButtonDown(vec2 point);
	void onMouseLeftButtonUp(vec2 point);
	
	void onMouseRightButtonDown(vec2 point);
	void onMouseRightButtonUp(vec2 point);

	void onMouseMove(vec2 point);
	void onMouseWheel(float delta);
	
	void onKeyDown(int key);
	void onKeyUp(int key);

	void onActive();
	void onDeActive();

	void render2D();

	void setupScene1();
};


#endif //PHYSICS_TEST_FRAME_H