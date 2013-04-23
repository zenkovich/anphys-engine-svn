#include "../../engine_options.h"

#ifdef PLATFORM_WIN32

#include "render_frame_win32_window.h"
#include "../../../render/render.h"
#include "../../../util/log/log_stream_console_and_file.h"

extern cLogStreamInConsoleAndFile* gLog;

apRenderWindow::apRenderWindow():grRenderFrame() {}

apRenderWindow::apRenderWindow(const std::string& title, fRect wndRect, fRect outputRect):grRenderFrame()
{
	initializeStd(NULL, title, wndRect.getWinRect());

	mRender = new grRender(this, outputRect);

	onCreate(mInRect);
}

apRenderWindow::apRenderWindow(const std::string& title, fRect wndRect):grRenderFrame()
{
	initializeStd(NULL, title, wndRect.getWinRect());

	mRender = new grRender(this, mInRect);

	onCreate(mInRect);
}

apRenderWindow::~apRenderWindow()
{
	safe_release(mRender);
}

void apRenderWindow::onCreate(fRect inRect)
{
	SetTimer(mHWnd, 0, 1, NULL);
	mTime = 0;
	mLastTickCount = GetTickCount();
}

float apRenderWindow::onTimer()
{
	DWORD currTickCount = GetTickCount();
	DWORD dt = currTickCount - mLastTickCount;

	float fdt = (float)dt*0.001f;
	if (fdt < 1.0f/1000.0f) fdt = 1.0f/1000.0f;
	if (fdt > 1.0f/10.0f) fdt = 1.0f/10.0f;

	//fdt = 1.0f/60.0f;

	mTime += fdt;

	mLastTickCount = currTickCount;

	return fdt;
}

void apRenderWindow::onSize(fRect inRect)
{
	//*gLog << formatStr("onSize(%f %f %f %f) in %s\n", inRect.leftTop.x, inRect.leftTop.y, inRect.rightDown.x, inRect.rightDown.y, mWndName.c_str());
}

void apRenderWindow::onMouseLeftButtonDown(vec2 point)
{
	//*gLog << formatStr("onMouseLeftButtonDown(%f %f) in %s\n", point.x, point.y, mWndName.c_str());
}

void apRenderWindow::onMouseLeftButtonUp(vec2 point)
{
	//*gLog << formatStr("onMouseLeftButtonUp(%f %f) in %s\n", point.x, point.y, mWndName.c_str());
}

void apRenderWindow::onMouseRightButtonDown(vec2 point)
{
	//*gLog << formatStr("onMouseRightButtonDown(%f %f) in %s\n", point.x, point.y, mWndName.c_str());
}

void apRenderWindow::onMouseRightButtonUp(vec2 point)
{
	//*gLog << formatStr("onMouseRightButtonUp(%f %f) in %s\n", point.x, point.y, mWndName.c_str());
}

void apRenderWindow::onMouseMove(vec2 point)
{
	//*gLog << formatStr("onMouseMove(%f %f) in %s\n", point.x, point.y, mWndName.c_str());
}

void apRenderWindow::onMouseWheel(float delta)
{
	//*gLog << formatStr("onMouseWheel(%f) in %s\n", delta, mWndName.c_str());
}

void apRenderWindow::onKeyDown(int key)
{
	//*gLog << formatStr("onKeyDown(%i) in %s\n", key, mWndName.c_str());
}

void apRenderWindow::onKeyUp(int key)
{
	//*gLog << formatStr("onKeyUp(%i) in %s\n", key, mWndName.c_str());
}

void apRenderWindow::onActive()
{
	//*gLog << formatStr("onActive() in %s\n", mWndName.c_str());
}

void apRenderWindow::onDeActive()
{
	//*gLog << formatStr("onDeActive() in %s\n", mWndName.c_str());
}

void apRenderWindow::onClose() {}


#endif //PLATFORM_WIN32