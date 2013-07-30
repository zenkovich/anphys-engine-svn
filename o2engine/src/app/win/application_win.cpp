#include "engine_settings.h"
#ifdef PLATFORM_WIN

#include "application_win.h"
#include "util/log.h"
#include "util/math/math.h"

OPEN_O2_NAMESPACE


cApplication::cApplication():
	cApplicationBaseInterface(), mHWnd(0), mWndStyle(0), mWindowed(true), mWindowedSize(800, 600), mWindowedPos(0, 0),
	mAutoAjustByScreen(false), mAutoAjustScale(1, 1)
{
	initializeWindow();
	mApplication = this;
}

cApplication::~cApplication()
{

}

void cApplication::onUpdate( float dt )
{

}

void cApplication::onDraw()
{

}

void cApplication::setOptions( cApplicationOption::type option, ... )
{
	va_list vlist;
	va_start(vlist, option);

	if (option == cApplicationOption::WINDOWED)
	{
		mWindowed = true;

		mLogStream->hout("cApplication::setOptions( WINDOWED )");

		resetWnd();
	}
	else if (option == cApplicationOption::FULLSCREEN)
	{
		mWindowed = false;

		mLogStream->hout("cApplication::setOptions( FULLSCREEN )");

		resetWnd();
	}
	else if (option == cApplicationOption::RESIZIBLE)
	{
		bool isResizible = va_arg(vlist, bool);
		if (isResizible)
			mWndStyle |= WS_OVERLAPPED | WS_MAXIMIZEBOX;
		else
			mWndStyle ^= WS_OVERLAPPED | WS_MAXIMIZEBOX;		

		mLogStream->hout("cApplication::setOptions( RESIZIBLE, %s )", ( isResizible ? "true":"false" ));

		SetWindowLong(mHWnd, GWL_STYLE, mWndStyle);
	}
	else if (option == cApplicationOption::AUTO_AJUST_BY_SCREEN_SPACE)
	{
		mAutoAjustScale = va_arg(vlist, vec2f);
		mAutoAjustByScreen = true;
		mWindowed = true;		

		mLogStream->hout("cApplication::setOptions( AUTO_AJUST_BY_SCREEN_SPACE, v2(%.3f %.3f), %s )", 
			mAutoAjustScale.x, mAutoAjustScale.y, ( mAutoAjustByScreen ? "true":"false" ));

		resetWnd();
	}
	else if (option == cApplicationOption::WND_SIZE)
	{
		mWindowedSize = va_arg(vlist, vec2i);

		mLogStream->hout("cApplication::setOptions( WND_SIZE, v2((%i %i) )", 
			mWindowedSize.x, mWindowedSize.y);

		resetWnd();
	}
	else if (option == cApplicationOption::WND_POSITION)
	{
		mWindowedPos = va_arg(vlist, vec2i);

		mLogStream->hout("cApplication::setOptions( WND_POSITION, v2(%i %i) )", 
			mWindowedPos.x, mWindowedPos.y);

		resetWnd();
	}
	else if (option == cApplicationOption::WND_CAPTION)
	{
		mWndCaption = va_arg(vlist, std::string);

		mLogStream->hout("cApplication::setOptions( WND_CAPTION, %s )", mWndCaption.c_str());

		SetWindowText(mHWnd, mWndCaption.c_str());
	}

	va_end(vlist);
}

void cApplication::processMessage( cApplacationMessage::type message )
{

}

void cApplication::launch()
{
	ShowWindow(mHWnd, SW_SHOW);
	
	mLogStream->hout("Application launched!");

	MSG msg;
	memset(&msg, 0, sizeof(msg));

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} 
		else
		{
			onUpdate(0.017f);
			onDraw();			
			mInputMessage.update();
		}
	}
}

LRESULT cApplication::wndProc( HWND wnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	POINT cursorPt;
	int key = 0;

	switch(uMsg)
	{	
	case WM_LBUTTONDOWN:
		mApplication->mInputMessage.keyPressed(VK_LBUTTON);
		break;

	case WM_LBUTTONUP:
		mApplication->mInputMessage.keyReleased(VK_LBUTTON);
		break;
	
	case WM_RBUTTONDOWN:
		mApplication->mInputMessage.keyPressed(VK_RBUTTON);
		break;

	case WM_RBUTTONUP:
		mApplication->mInputMessage.keyReleased(VK_RBUTTON);
		break;

	case WM_KEYDOWN:
		key = (int)wParam;
		mApplication->mInputMessage.keyPressed(key);
		break;

	case WM_KEYUP:
		mApplication->mInputMessage.keyReleased((int)wParam);
		break;

	case WM_MOUSEMOVE:
		GetCursorPos(&cursorPt);
		ScreenToClient(wnd, &cursorPt);

		mApplication->mInputMessage.setCursorPos(vec2f((float)cursorPt.x, (float)cursorPt.y));
		break;

	case WM_DESTROY: 
		PostQuitMessage(0);
		return 0;
		break;
	}
	return DefWindowProc(wnd, uMsg, wParam, lParam);
}

void cApplication::initializeWindow()
{
	mLogStream->hout("Initializing window..");

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
		mLogStream->out("ERROR: Can't regist class");
		return;
	}

	if (!(mHWnd = CreateWindowEx(NULL, wndClass.lpszClassName, "App test",             
						   WS_OVERLAPPEDWINDOW | WS_VISIBLE,
						   mWindowedPos.x, mWindowedPos.y, mWindowedSize.x, mWindowedSize.y,
						   NULL, NULL, NULL,  NULL))) 
	{
		
		mLogStream->out("ERROR: Can't create window (CreateWindowEx)");
		return;
	}

	mLogStream->hout("Window initialized!");
}

void cApplication::setWindowed()
{
	mLogStream->hout("Setting windowed..");

	mWindowed = true;

	RECT rt = { mWindowedPos.x, mWindowedPos.y, mWindowedPos.x + mWindowedSize.x, mWindowedPos.y + mWindowedSize.y };
	AdjustWindowRect(&rt, mWndStyle, false);
	SetWindowPos(mHWnd, HWND_NOTOPMOST, mWindowedPos.x, mWindowedPos.y, 
		         mWindowedSize.x, mWindowedSize.y, SWP_SHOWWINDOW);

	if (mAutoAjustByScreen)
	{
		autoAjustByScreenSpace();
	}
	
	mLogStream->hout("Complete");
}

void cApplication::setFullscreen()
{
	mLogStream->hout("Setting fullscreen");
}

void cApplication::autoAjustByScreenSpace()
{
	mLogStream->hout("Setting autoAjustByScreenSpace");
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

cApplication* cApplication::mApplication = NULL;

CLOSE_O2_NAMESPACE

#endif //PLATFORM_WIN