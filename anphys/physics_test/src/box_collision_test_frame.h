#ifndef BOX_COLLISION_TEST_FRAME_H
#define BOX_COLLISION_TEST_FRAME_H

#include "engine/engine_options.h"

#ifdef PLATFORM_WIN32
#include "engine/frame_system/win32_render_frame/render_frame_win32_window.h"
#endif //PLATFORM_WIN32

struct grCamera3DMouse;
struct grRenderSceneBaseInterface;
struct cScene;
struct cObject;
struct grSimple3DRenderState;

struct apBoxCollisionTestFrame:public apRenderWindow
{
	grCamera3DMouse*            mCamera3dMouse;
	grSimple3DRenderState*      m3DRenderState;

	cScene*                     mMainEngineScene;

	vec3 mBoxAPos, mBoxBPos, mBoxAAngles, mBoxBAngles, mBoxASize, mBoxBSize;
	cObject* mBoxAObject, *mBoxBObject;

	bool                        mPhysicsRunning;
	bool                        mPhysicsRunByStep;

	apBoxCollisionTestFrame();
	apBoxCollisionTestFrame(const std::string& title, fRect wndRect, fRect outputRect);
	apBoxCollisionTestFrame(const std::string& title, fRect wndRect);

	~apBoxCollisionTestFrame();

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

	//void testConstraintSolve(float dt, const vec3& attachPoint);
	void testBoxCD();
};


#endif //BOX_COLLISION_TEST_FRAME_H