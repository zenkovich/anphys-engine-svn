#ifndef RENDER_FRAME_WIN32_WINDOW_H
#define RENDER_FRAME_WIN32_WINDOW_H

#include "engine/engine_options.h"

#ifdef PLATFORM_WIN32

#include <string>

#include "../render_frame.h"
#include "engine/frame_system/win32_framesystem/simple_window.h"
#include "engine/frame_system/win32_framesystem/windows_manager.h"

struct apRenderWindow: public grRenderFrame
{
	float mTime;
	DWORD mLastTickCount;

//functions
	apRenderWindow();
	apRenderWindow(const std::string& title, fRect wndRect, fRect outputRect);
	apRenderWindow(const std::string& title, fRect wndRect);
	virtual ~apRenderWindow();

	virtual void onCreate(fRect inRect);
	virtual void onClose();
	virtual float onTimer();
	virtual void onSize(fRect inRect);
	
	virtual void onMouseLeftButtonDown(vec2 point);
	virtual void onMouseLeftButtonUp(vec2 point);
	
	virtual void onMouseRightButtonDown(vec2 point);
	virtual void onMouseRightButtonUp(vec2 point);

	virtual void onMouseMove(vec2 point);
	virtual void onMouseWheel(float delta);
	
	virtual void onKeyDown(int key);
	virtual void onKeyUp(int key);

	virtual void onActive();
	virtual void onDeActive();
};

#endif //PLATFORM_WIN32

#endif //RENDER_FRAME_WIN32_WINDOW_H