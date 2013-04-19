#include "engine/engine_options.h"
#ifdef PLATFORM_WIN32

#include <windows.h>
#include <stdio.h>
#include "simple_window.h"
#include "windows_manager.h"
#include "input/input_messenger.h"

extern apWindowsManager *gWindowsManager;

LRESULT CALLBACK WinProc2(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	gWindowsManager->procWindows(hwnd, msg, lparam, wparam);

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

apWindow::apWindow()
{
	mInputMessenger = new cInputMessenger;
}

apWindow::~apWindow()
{
	safe_release(mInputMessenger);
}

bool apWindow::initialize(WNDPROC winProc, HICON icon, HCURSOR cursor, HBRUSH backBrush, std::string className,  std::string wndName, RECT rect)
{
	WNDCLASSEX wndClass;
	wndClass.cbSize         = sizeof(WNDCLASSEX);
	wndClass.style			= CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc	= (WNDPROC)WinProc2;
	wndClass.cbClsExtra		= 0;
	wndClass.cbWndExtra		= 0;
	wndClass.hInstance		= NULL;
	wndClass.hIcon			= icon;
	wndClass.hCursor		= cursor;
	wndClass.hbrBackground	= backBrush;
	wndClass.lpszMenuName	= NULL;
	wndClass.lpszClassName	= className.c_str();
	wndClass.hIconSm        = icon;
	
	return initialize(wndClass, wndName, rect);
}

bool apWindow::initialize(const WNDCLASSEX& wndClass, std::string wndName, RECT rect)
{
	if (!RegisterClassEx(&wndClass)) 
	{
		printf("can't register window class (RegisterClassEx)\n");
		return false;
	}

	if (!(mHWnd = CreateWindowEx(NULL, wndClass.lpszClassName, wndName.c_str(),             
						   WS_OVERLAPPEDWINDOW | WS_VISIBLE,
						   rect.left,rect.top, rect.right-rect.left, rect.bottom-rect.top,
						   NULL, NULL, NULL,  NULL))) 
	{
		printf("can't create window (CreateWindowEx)\n");
		return false;
	}

	mWndName = wndName;

	mRect = rect;
	RECT rt;
	GetClientRect(mHWnd, &rt);
	fRect inRect(rt);
	mInRect = rt;
	
	onCreate(inRect);

	mActiveWindow = true;

	return true;
}

bool apWindow::initializeStd(WNDPROC winProc, std::string wndName, RECT rect)
{
	std::string className = wndName + "Class";
	return initialize(winProc, apWindowStdIcon, apWindowStrCursor, apWindowStrBackBrush, className, wndName, rect);
}

void apWindow::showWindow()
{
	ShowWindow( mHWnd, SW_SHOWDEFAULT );
}

LRESULT apWindow::windowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT ptt;
	switch(uMsg)
	{	
     case WM_DESTROY: 
		{
			//PostQuitMessage(0);
			return(0);
		} 
	 case WM_CLOSE:
		 {
			 onClose();
			 break;
		 }
		
     case WM_KEYDOWN: 
		 {
			 if (!mActiveWindow) break;

			int key = (int)(lParam);

			mInputMessenger->mInputMessage.keyPressed(key);

			onKeyDown(key);
			break;
		}
		
     case WM_KEYUP: 
		{
			if (!mActiveWindow) break;

			int key = (int)(lParam);

			mInputMessenger->mInputMessage.keyReleased(key);

			onKeyUp(key);
			break;
		}


	 case WM_MOUSEMOVE:
		 {
			 if (!mActiveWindow) break;

			GetCursorPos(&ptt);
			ScreenToClient(mHWnd, &ptt);

			vec2 cursor((float)ptt.x, (float)ptt.y);
			onMouseMove(cursor);
			
			mInputMessenger->mInputMessage.mCursorPosition = cursor;

			break;
		 }

	 case WM_LBUTTONDBLCLK:

	 case WM_LBUTTONDOWN:
		 {
			 if (!mActiveWindow) break;

			GetCursorPos(&ptt);
			ScreenToClient(mHWnd, &ptt);

			vec2 cursor((float)ptt.x, (float)ptt.y);
			onMouseLeftButtonDown(cursor);

			mInputMessenger->mInputMessage.keyPressed(CURSOR_BUTTON);
			mInputMessenger->mInputMessage.mCursorPosition = cursor;

			break;
		 }

	 case WM_LBUTTONUP:
		 {
			 if (!mActiveWindow) break;

			GetCursorPos(&ptt);
			ScreenToClient(mHWnd, &ptt);

			vec2 cursor((float)ptt.x, (float)ptt.y);
			onMouseLeftButtonUp(cursor);

			mInputMessenger->mInputMessage.keyReleased(CURSOR_BUTTON);
			mInputMessenger->mInputMessage.mCursorPosition = cursor;

			break;
		 }

	 
	 case WM_RBUTTONDOWN:
		 {			 
			 if (!mActiveWindow) break;

			GetCursorPos(&ptt);
			ScreenToClient(mHWnd, &ptt);

			vec2 cursor((float)ptt.x, (float)ptt.y);
			onMouseRightButtonDown(cursor);

			mInputMessenger->mInputMessage.keyPressed(RM_BUTTON);
			mInputMessenger->mInputMessage.mCursorPosition = cursor;

			break;
		 }

	 case WM_RBUTTONUP:
		 {
			 if (!mActiveWindow) break;

			GetCursorPos(&ptt);
			ScreenToClient(mHWnd, &ptt);

			vec2 cursor((float)ptt.x, (float)ptt.y);
			onMouseRightButtonUp(cursor);

			mInputMessenger->mInputMessage.keyReleased(RM_BUTTON);
			mInputMessenger->mInputMessage.mCursorPosition = cursor;

			break;
		 }

	 case WM_ACTIVATE:
		 {
			 if (LOWORD(lParam) == WA_INACTIVE)
			 {
				 mActiveWindow = false;
				 onDeActive();
				 *gLog << "deactivate window\n";
			 }
			 else
			 {
				 mActiveWindow = true;
				 onActive();
				 *gLog << "activate window\n";
			 }
		 }
	 case WM_MOUSEWHEEL:
		 {
			 if (!mActiveWindow) break;

			 float delta = GET_WHEEL_DELTA_WPARAM(lParam);
			 onMouseWheel(delta);

			 break;
		 }
	 case WM_SIZE:
		 {
			RECT rt;
			GetClientRect(mHWnd, &rt);

			fRect inRect(rt);
			mInRect = inRect;

			onSize(inRect);

			break;
		 }
	 case WM_TIMER:
		 {
			 if (!mActiveWindow) break;

			 onTimer();

			 break;
		 }
    } 

	return DefWindowProc(NULL, uMsg, wParam, lParam);
}

void apWindow::onMouseLeftButtonDown(vec2 point) {}

void apWindow::onMouseLeftButtonUp(vec2 point) {}
	
void apWindow::onMouseRightButtonDown(vec2 point) {}

void apWindow::onMouseRightButtonUp(vec2 point) {}

void apWindow::onMouseMove(vec2 point) {}

void apWindow::onMouseWheel(float delta) {}
	
void apWindow::onKeyDown(int key) {}

void apWindow::onKeyUp(int key) {}

void apWindow::onCreate(fRect inRect) { }

void apWindow::onSize(fRect inRect) {}

void apWindow::onActive() {}

void apWindow::onClose() {}

void apWindow::onDeActive() {}

float apWindow::onTimer() { return 0; }

bool apWindow::isKeyDown(int key)
{
	return mInputMessenger->mInputMessage.isKeyDown(key);
}


#endif