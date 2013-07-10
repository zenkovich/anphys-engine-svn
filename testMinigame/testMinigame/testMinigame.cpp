#include <windows.h>
#include <string>

#include "math/vector2.h"

#include "render_system/render_system.h"
#include "render_system/sprite.h"

#include "minigame/puzzle_pic_minigame.h"

HWND          gHWnd;    
RenderSystem  gRenderSystem;
MiniGame*     gTestMinigame;
unsigned long gLastTickCount;
vec2i         gWindowSize;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

void createWindow(const std::string& wndName, HWND& hwnd)
{
    WNDCLASSEX   wndclassex = {0};
    wndclassex.cbSize        = sizeof(WNDCLASSEX);
    wndclassex.style         = CS_HREDRAW | CS_VREDRAW;
    wndclassex.lpfnWndProc   = WndProc;
    wndclassex.cbClsExtra    = 0;
    wndclassex.cbWndExtra    = 0;
    wndclassex.hInstance     = NULL;
    wndclassex.hIcon         = LoadIcon (NULL, IDI_APPLICATION);
    wndclassex.hCursor       = LoadCursor (NULL, IDC_ARROW);
    wndclassex.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
    wndclassex.lpszMenuName  = NULL;
    wndclassex.lpszClassName = wndName.c_str();
    wndclassex.hIconSm       = wndclassex.hIcon;
	
    if (!RegisterClassEx (&wndclassex))
    {
        MessageBox (NULL, TEXT ("RegisterClassEx failed!"), wndName.c_str(), MB_ICONERROR);
        return;
    }
    hwnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, 
		                  wndName.c_str(), 
        		          wndName.c_str(),
                		  WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
		                  0, 
        		          0, 
                		  gWindowSize.x, 
		                  gWindowSize.y, 
        		          NULL, 
                		  NULL, 
		                  NULL,
        		          NULL); 
}

void initializeRenderSystem(HWND hwnd, RenderSystem& renderSystem)
{
	RECT wndClientRect;
	GetClientRect(hwnd, &wndClientRect);

	renderSystem.initialize(hwnd, vec2i(wndClientRect.right - wndClientRect.left, wndClientRect.bottom - wndClientRect.top));
}

void render(RenderSystem& renderSystem)
{
	renderSystem.beginRender();
	
	//draw minigame
	gTestMinigame->render();

	renderSystem.endRender();
}

void update()
{
	unsigned long currentTickCount = GetTickCount();

	const float minFPS = 10.0f;
	const float maxFPS = 1000.0f;
	float dt = max(min((float)(currentTickCount - gLastTickCount)*0.001f, 1.0f/minFPS), 1.0f/maxFPS);

	gLastTickCount = currentTickCount;

	gTestMinigame->update(dt);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	POINT point;

    switch (message)
    {

	case WM_MOUSEMOVE:
		GetCursorPos(&point);
		ScreenToClient(hwnd, &point);

		gTestMinigame->cursorMove((float)point.x, (float)point.y);

		break;

	case WM_LBUTTONUP:
		GetCursorPos(&point);
		ScreenToClient(hwnd, &point);

		gTestMinigame->click((float)point.x, (float)point.y);

		break;

    case WM_DESTROY:
        PostQuitMessage (0);
        return (0);
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

int main(int argc, char* argv[])
{
	gWindowSize = vec2i(1024, 700);

	createWindow("TestMinigame", gHWnd);
						  
    ShowWindow (gHWnd, SW_SHOWDEFAULT);
    UpdateWindow (gHWnd);

	initializeRenderSystem(gHWnd, gRenderSystem);

	gTestMinigame = new PuzzlePictureMinigame(&gRenderSystem);
	gTestMinigame->initialize();

	gLastTickCount = GetTickCount();
	
	MSG msg = tagMSG();
    while ( msg.message != WM_QUIT )
    {
		if(PeekMessage( &msg, NULL,0,0,PM_REMOVE ))
		{
			TranslateMessage (&msg);
			DispatchMessage (&msg);
		}
		else
		{
			update();
			render(gRenderSystem);
		}
    }

	return 0;
}

