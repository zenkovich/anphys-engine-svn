#include "engine_settings.h"
#ifdef PLATFORM_WIN

#include "application_win.h"
#include "other/device_info.h"
#include "render_system/render_system.h"
#include "util/log.h"
#include "util/math/math.h"
#include "util/scheduler.h"
#include "util/time_utils.h"
#include "util/timer.h"
#include "util/build_system/build_system.h"

OPEN_O2_NAMESPACE


cApplication::cApplication():
	cApplicationBaseInterface(), mHWnd(0), mWndStyle(0), mWindowed(true), mWindowedSize(800, 600), mWindowedPos(0, 0),
	mWindowResizible(true), mActive(false)
{
	mnew cBuildSystem("../../../");
	cBuildSystem::instance().rebuildAssets(true);

	initializeWindow();

	mApplication = this;

	mRenderSystem = mnew grRenderSystem();

	cDeviceInfo::initializeSingleton();
	deviceInfo()->initialize( this );
}

cApplication::~cApplication()
{	
	safe_release(mUIController);
	safe_release(mRenderSystem);
	safe_release(mTimer);

	cDeviceInfo::deinitializeSingleton();
}

void cApplication::initializeWindow()
{
	mLog->hout("Initializing window..");

	mWndStyle = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

	WNDCLASSEX wndClass;
	wndClass.cbSize         = sizeof(WNDCLASSEX);
	wndClass.style			= mWndStyle;
	wndClass.lpfnWndProc	= (WNDPROC)wndProc;
	wndClass.cbClsExtra		= 0;
	wndClass.cbWndExtra		= 0;
	wndClass.hInstance		= NULL;
	wndClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground	= (HBRUSH)GetStockObject(GRAY_BRUSH);
	wndClass.lpszMenuName	= NULL;
	wndClass.lpszClassName	= "o2App";
	wndClass.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wndClass)) 
	{
		mLog->error("Can't register class");
		return;
	}

	if (!(mHWnd = CreateWindowEx(NULL, wndClass.lpszClassName, "App test",             
						   WS_OVERLAPPEDWINDOW | WS_VISIBLE,
						   mWindowedPos.x, mWindowedPos.y, mWindowedSize.x, mWindowedSize.y,
						   NULL, NULL, NULL,  NULL))) 
	{
		
		mLog->error("Can't create window (CreateWindowEx)");
		return;
	}

	mLog->hout("Window initialized!");
}

void cApplication::launch()
{
	ShowWindow(mHWnd, SW_SHOW);
	
	mLog->hout("Application launched!");

	onStarted();
	onStartedEvent.call();

	MSG msg;
	memset(&msg, 0, sizeof(msg));

	mTimer->reset();

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} 
		else
		{
			processFrame();
		}
	}

	onClosing();
	onClosingEvent.call();
}

LRESULT cApplication::wndProc( HWND wnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	POINT pt;
	RECT rt;
	int key = 0;
	vec2i size, pos;
	GetCursorPos(&pt);
	ScreenToClient(wnd, &pt);
	vec2f cursorPos = vec2f((float)pt.x, (float)pt.y);
	float wheelDelta;

	switch(uMsg)
	{	
	case WM_LBUTTONDOWN:
		mApplication->mInputMessage->cursorPressed(cursorPos);
		break;

	case WM_LBUTTONUP:
		mApplication->mInputMessage->cursorReleased();
		break;
	
	case WM_RBUTTONDOWN:
		mApplication->mInputMessage->altCursorPressed(cursorPos);
		break;

	case WM_RBUTTONUP:
		mApplication->mInputMessage->altCursorReleased();
		break;

	case WM_MBUTTONDOWN:
		mApplication->mInputMessage->alt2CursorPressed(cursorPos);
		break;

	case WM_MBUTTONUP:
		mApplication->mInputMessage->alt2CursorReleased();
		break;

	case WM_KEYDOWN:
		key = (int)wParam;
		mApplication->mInputMessage->keyPressed(key);
		break;

	case WM_KEYUP:
		mApplication->mInputMessage->keyReleased((int)wParam);
		break;

	case WM_MOUSEMOVE:
		mApplication->mInputMessage->setCursorPos(cursorPos);
		break;

	case WM_MOUSEWHEEL:
		wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		mApplication->mInputMessage->setMouseWheelDelta(wheelDelta);
		break;

	case WM_ACTIVATE:
		if ((HWND)lParam == mApplication->mHWnd || true)
		{
			//hlog("LOWORD(wParam) = %i %i %i", LOWORD(wParam), mApplication->mHWnd, lParam);

			if (LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE)
			{
				mApplication->mActive = true;
				mApplication->onActivated();
				mApplication->onActivatedEvent.call();
			}
			else
			{
				mApplication->mActive = false;
				mApplication->onDeactivated();
				mApplication->onDeactivatedEvent.call();
			}
		}
		break;

	case WM_SIZE:
		GetWindowRect(mApplication->mHWnd, &rt);
		size.x = rt.right - rt.left; size.y = rt.bottom - rt.top;

		if (size.x > 0 && size.y > 0 && size != mApplication->mWindowedSize)
		{
			mApplication->mWindowedSize = size;
			mApplication->onResizing();
			mApplication->mRenderSystem->frameResized();
			mApplication->onResizingEvent.call();
		}
		mApplication->processFrame();

		break;

	case WM_MOVE:
		GetWindowRect(mApplication->mHWnd, &rt);
		pos.x = rt.left; pos.y = rt.top;

		if (pos.x < 10000 && pos.y < 10000 && pos != mApplication->mWindowedPos)
		{
			mApplication->mWindowedPos = pos;
			//hlog("WND POS %i %i", pos.x, pos.y);
			mApplication->onMoved();
			mApplication->onMovingEvent.call();
		}
		break;

	case WM_DESTROY: 
		PostQuitMessage(0);
		return 0;
		break;
	}
	return DefWindowProc(wnd, uMsg, wParam, lParam);
}

void cApplication::resetWnd()
{
	if (mWindowed)
	{
		setWindowed();
	}
	else
	{
		setFullscreen();
	}
}

void cApplication::setWindowed()
{
	mLog->hout("Setting windowed..");

	mWindowed = true;

	RECT rt = { mWindowedPos.x, mWindowedPos.y, mWindowedPos.x + mWindowedSize.x, mWindowedPos.y + mWindowedSize.y };
	AdjustWindowRect(&rt, mWndStyle, false);
	SetWindowPos(mHWnd, HWND_NOTOPMOST, mWindowedPos.x, mWindowedPos.y, 
		         mWindowedSize.x, mWindowedSize.y, SWP_SHOWWINDOW);

	mRenderSystem->frameResized();		
	mLog->hout("Complete");
}

void cApplication::setFullscreen()
{
	mRenderSystem->frameResized();		
	mLog->hout("Setting full screen");
}

void cApplication::onUpdate( float dt )
{
}

void cApplication::onDraw()
{
}

bool cApplication::isFullScreen() const
{
	return !mWindowed;
}

void cApplication::setResizible(bool resizible)
{
	if (resizible == mWindowResizible)
		return;

	mWindowResizible = resizible;

	if (mWindowResizible)
		mWndStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX;
	else
		mWndStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX;

	mLog->hout("set resizible: %s ", ( mWindowResizible ? "true":"false" ));

	SetWindowLong(mHWnd, GWL_STYLE, mWndStyle);
}

bool cApplication::isResizible() const
{
	return mWindowResizible;
}

void cApplication::setWindowSize(const vec2i& size)
{
	mWindowedSize = size;
	mLog->hout("setWindowSize: %ix%i", mWindowedSize.x, mWindowedSize.y);
	resetWnd();
}

vec2i cApplication::getWindowSize() const
{
	return mWindowedSize;
}

void cApplication::setWindowPosition(const vec2i& position)
{
	mWindowedPos = position;
	mLog->hout("set Window Position: %i, %i", mWindowedPos.x, mWindowedPos.y);
	resetWnd();
}

vec2i cApplication::getWindowPosition() const
{
	return mWindowedPos;
}

void cApplication::setWindowCaption(const string& caption)
{
	mWndCaption = caption;
	SetWindowText(mHWnd, mWndCaption.c_str());
}

ENGINE_NAMESPACE_NAME::string cApplication::getWindowCaption() const
{
	return mWndCaption;
}

void cApplication::setContentSize(const vec2i& size)
{
	vec2i clientRectSize = size;

	RECT clientRect;
	GetClientRect(mHWnd, &clientRect);
	clientRect.right = clientRect.left + size.x;
	clientRect.bottom = clientRect.top + size.y;

	AdjustWindowRect(&clientRect, mWndStyle, false);

	mWindowedPos = vec2i(clientRect.left, clientRect.top);
	mWindowedSize = vec2i(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	mLog->hout("set Content Size: %ix%i", size.x, size.y);

	resetWnd();
}

vec2i cApplication::getContentSize() const
{
	RECT clientRect;
	GetClientRect(mHWnd, &clientRect);
	return vec2i(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
}

cApplication* cApplication::mApplication = NULL;

CLOSE_O2_NAMESPACE

#endif //PLATFORM_WIN