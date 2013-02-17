#pragma once
#include "engine/engine_options.h"
#ifdef PLATFORM_WIN32

#include <windows.h>
#include <string>

#include "util/math/mmath.h"

#define apWindowStdIcon LoadIcon(NULL, IDI_APPLICATION)          //стандартная иконка
#define apWindowStrCursor LoadCursor(NULL, IDC_ARROW)            //стандартный курсор
#define apWindowStrBackBrush (HBRUSH)GetStockObject(GRAY_BRUSH)  //стандартный фон

struct apWindowsManager;

//базовое окно
struct apWindow
{
	HWND mHWnd;                  //окно
	fRect mRect;                 //прямоугольник окна
	fRect mInRect;               //внутренняя часть окна

	std::string mWndName;        //имя окна

	apWindowsManager *mpManager; //менеджер окон

	int mPressedKeys[3];         //индексы нажатых клавиш

	bool mLeftMouseButton;       //левая кнопка мышы
	bool mRightMouseButton;      //правая кнопка мышы

	vec2 mCursorPos;             //позиция курсора

	bool mActiveWindow;          //активировано ли окно

	//базовая инициализация
	bool initialize(WNDPROC winProc, HICON icon, HCURSOR cursor, HBRUSH backBrush, std::string className,  std::string wndName, RECT rect);

	//инициалиизация от WNDCLASSEX
	bool initialize(const WNDCLASSEX& wndClass, std::string wndName, RECT rect);

	//стандартная инициализация
	bool initializeStd(WNDPROC winProc, std::string wndName, RECT rect);

	//показать окно
	void showWindow();

	//обработка сообщений окна
	LRESULT windowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//сообщения:
	virtual void onMouseLeftButtonDown(vec2 point);
	virtual void onMouseLeftButtonUp(vec2 point);
	
	virtual void onMouseRightButtonDown(vec2 point);
	virtual void onMouseRightButtonUp(vec2 point);

	virtual void onMouseMove(vec2 point);
	virtual void onMouseWheel(float delta);
	
	virtual void onKeyDown(int key);
	virtual void onKeyUp(int key);

	virtual void onCreate(fRect inRect);
	virtual void onClose();
	virtual void onSize(fRect inRect);

	virtual void onActive();
	virtual void onDeActive();

	virtual float onTimer();
	//------------------------------//

	//нажата ли кнопка с индексом
	bool isKeyPressed(int key); 

	int pressedKey(int idx);
};

#endif