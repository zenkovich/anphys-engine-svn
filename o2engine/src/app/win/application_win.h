#ifndef APPLICATION_WIN_H
#define APPLICATION_WIN_H

#include <Windows.h>

#include "public.h"
#include "../application_base_interface.h"

OPEN_O2_NAMESPACE

class cApplication:public cApplicationBaseInterface
{
	HWND         mHWnd;
	unsigned int mWndStyle;
		         
	bool         mWidnowed;
	vec2i        mWindowedSize;
	vec2i        mWindowedPos;
	
	bool         mAutoAjustByScreen;
	vec2f        mAutoAjustScale;

	std::string  mWndCaption;

public:
	cApplication();
	~cApplication();

	void onUpdate(float dt);
	void onDraw();

	void setOptions(cApplicationOption::type option, ...);

	void processMessage(cApplacationMessage::type message);

	void launch();

protected:
	void initializeWindow();

	void setWindowed();
	void setFullscreen();
	void autoAjustByScreenSpace();
	void resetWnd();

	static LRESULT wndProc(HWND wnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

CLOSE_O2_NAMESPACE

#endif //APPLICATION_WIN_H