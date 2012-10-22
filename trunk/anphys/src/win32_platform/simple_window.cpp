#include "../engine/engine_options.h"
#ifdef PLATFORM_WIN32

#include "simple_window.h"

#include <windows.h>
#include <stdio.h>

#include "windows_manager.h"


extern apWindowsManager *gWindowsManager;

LRESULT CALLBACK WinProc2(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	gWindowsManager->procWindows(hwnd, msg, lparam, wparam);

	return DefWindowProc(hwnd, msg, wparam, lparam);
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
	mInRect = inRect;


	mLeftMouseButton = mRightMouseButton = false;
	mPressedKeys[0] = mPressedKeys[1] = mPressedKeys[2] = -1;

	onCreate(inRect);

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
			int key = (int)(lParam);

			for (int i = 0; i < 3; i++)
			{
				if (mPressedKeys[i] == key) break;

				if (mPressedKeys[i] < 0) 
				{
					mPressedKeys[i] = key;
					break;
				}

			}

			onKeyDown(key);
			break;
		}
		
     case WM_KEYUP: 
		{
			int key = (int)(lParam);

			for (int i = 0; i < 3; i++)
				if (mPressedKeys[i] == key) mPressedKeys[i] = -1;

			onKeyUp(key);
			break;
		}


	 case WM_MOUSEMOVE:
		 {
			GetCursorPos(&ptt);
			ScreenToClient(mHWnd, &ptt);

			vec2 cursor((float)ptt.x, (float)ptt.y);
			onMouseMove(cursor);
			
			mCursorPos = cursor;
			break;
		 }

	 case WM_ACTIVATE:
		 {
			if (LOWORD(lParam) == WA_INACTIVE)
			{
				mActiveWindow = false;
				onDeActive();
			}
			else
			{
				mActiveWindow = true;
				onActive();
			}
		 }

	 case WM_LBUTTONDOWN:
		 {
			GetCursorPos(&ptt);
			ScreenToClient(mHWnd, &ptt);

			vec2 cursor((float)ptt.x, (float)ptt.y);
			onMouseLeftButtonDown(cursor);

			mCursorPos = cursor;
			mLeftMouseButton = true;

			break;
		 }

	 case WM_LBUTTONUP:
		 {
			GetCursorPos(&ptt);
			ScreenToClient(mHWnd, &ptt);

			vec2 cursor((float)ptt.x, (float)ptt.y);
			onMouseLeftButtonUp(cursor);

			mCursorPos = cursor;
			mLeftMouseButton = false;
			break;
		 }

	 
	 case WM_RBUTTONDOWN:
		 {
			GetCursorPos(&ptt);
			ScreenToClient(mHWnd, &ptt);

			vec2 cursor((float)ptt.x, (float)ptt.y);
			onMouseRightButtonDown(cursor);

			mCursorPos = cursor;
			mRightMouseButton = true;
			break;
		 }

	 case WM_RBUTTONUP:
		 {
			GetCursorPos(&ptt);
			ScreenToClient(mHWnd, &ptt);

			vec2 cursor((float)ptt.x, (float)ptt.y);
			onMouseRightButtonUp(cursor);

			mCursorPos = cursor;
			mRightMouseButton = false;
			break;
		 }
	 case WM_MOUSEWHEEL:
		 {
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

bool apWindow::isKeyPressed(int key)
{
	for (int i = 0; i < 3; i++)
		if (mPressedKeys[i] == key) return true;

	return false;
}

int apWindow::pressedKey(int idx)
{
	return mPressedKeys[idx];
}


#endif