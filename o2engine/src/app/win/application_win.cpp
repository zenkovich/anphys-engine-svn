#include "engine_settings.h"
#ifdef PLATFORM_WIN

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

}

void cApplication::processMessage( cApplacationMessage::type message )
{

}

void cApplication::launch()
{
	ShowWindow(mHWnd, 0);
}

LRESULT cApplication::wndProc( HWND wnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return DefWindowProc(wnd, uMsg, wParam, lParam);
}

void cApplication::initializeWindow()
{
	WNDCLASSEX wndClass;
	wndClass.cbSize         = sizeof(WNDCLASSEX);
	wndClass.style			= CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
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

CLOSE_O2_NAMESPACE

#endif //PLATFORM_WIN