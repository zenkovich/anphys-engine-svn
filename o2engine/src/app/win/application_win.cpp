#include "engine_settings.h"
#ifdef PLATFORM_WIN

#include <varargs.h>

#include "application_win.h"
#include "util/log.h"

OPEN_O2_NAMESPACE


cApplication::cApplication():
	cApplicationBaseInterface() 
{
	initializeWindow();
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
		mWidnowed = true;
		resetWnd();
	}
	else if (option == cApplicationOption::FULLSCREEN)
	{
		mWidnowed = false;
		resetWnd();
	}
	else if (option == cApplicationOption::RESIZIBLE)
	{
		bool isResizible = va_arg(vlist, bool);
		if (isResizible)
			mWndStyle |= WS_OVERLAPPED | WS_MAXIMIZEBOX;
		else
			mWndStyle ^= WS_OVERLAPPED | WS_MAXIMIZEBOX;
		SetWindowLong(mHWnd, GWL_STYLE, mWndStyle);
	}
	else if (option == cApplicationOption::AUTO_AJUST_BY_SCREEN_SPACE)
	{
		mAutoAjustScale = va_arg(vlist, vec2f);
		mAutoAjustByScreen = true;
		mWidnowed = true;
		resetWnd();
	}
	else if (option == cApplicationOption::WND_SIZE)
	{
		mWindowedSize = va_arg(vlist, vec2i);
		resetWnd();
	}
	else if (option == cApplicationOption::WND_POSITION)
	{
		mWindowedPos = va_arg(vlist, vec2i);
		resetWnd();
	}
	else if (option == cApplicationOption::WND_CAPTION)
	{
		mWndCaption = va_arg(vlist, std::string);
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

	MSG msg;
	memset(&msg, 0, sizeof(msg));

	while(msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else
		{
			onUpdate(0.017f);
			onDraw();
		}
	}
}

LRESULT cApplication::wndProc( HWND wnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return DefWindowProc(wnd, uMsg, wParam, lParam);
}

void cApplication::initializeWindow()
{
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
		printf("can't register window class (RegisterClassEx)\n");
		return;
	}

	if (!(mHWnd = CreateWindowEx(NULL, wndClass.lpszClassName, "App test",             
						   WS_OVERLAPPEDWINDOW | WS_VISIBLE,
						   0, 0, 100, 100,
						   NULL, NULL, NULL,  NULL))) 
	{
		printf("can't create window (CreateWindowEx)\n");
		return;
	}
}

void cApplication::setWindowed()
{
	mWidnowed = true;

	RECT rt = { mWindowedPos.x, mWindowedPos.y, mWindowedPos.x + mWindowedSize.x, mWindowedPos.y + mWindowedSize.y };
	AdjustWindowRect(&rt, mWndStyle, false);
	SetWindowPos(mHWnd, HWND_NOTOPMOST, mWindowedPos.x, mWindowedPos.y, 
		         mWindowedPos.x + mWindowedSize.x, mWindowedPos.y + mWindowedSize.y, SWP_SHOWWINDOW);

	if (mAutoAjustByScreen)
	{
		autoAjustByScreenSpace();
	}
}

void cApplication::setFullscreen()
{

}

void cApplication::autoAjustByScreenSpace()
{

}

void cApplication::resetWnd()
{
	if (mWidnowed)
	{
		setWindowed();
	}
	else
	{
		setFullscreen();
	}
}

CLOSE_O2_NAMESPACE

#endif //PLATFORM_WIN