#ifndef TEST_FRAME_H
#define TEST_FRAME_H

#include "../../src/engine/engine_options.h"

#ifdef PLATFORM_WIN32
#include "../../src/engine/render_system/render_frame_win32_window/render_frame_win32_window.h"
#endif //PLATFORM_WIN32

struct grCamera3DMouse;
struct grRenderSceneBaseInterface;
struct cScene;

struct apTestFrame:public apRenderWindow
{
	grCamera3DMouse*            mCamera3dMouse;
	cScene*                     mMainEngineScene;

	apTestFrame();
	apTestFrame(const std::string& title, fRect wndRect, fRect outputRect);
	apTestFrame(const std::string& title, fRect wndRect);

	~apTestFrame();

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
};


#endif //TEST_FRAME_H