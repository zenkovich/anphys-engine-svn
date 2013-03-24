#pragma once
#include "engine/engine_options.h"
#ifdef PLATFORM_WIN32

#include <windows.h>
#include <vector>
#include <string>
#include "util/math/mmath.h"

struct apWindow;
struct cLogStream;

/** �������� ����. */
struct apWindowsManager
{
	typedef std::vector<apWindow*> WindowsList;

	WindowsList mWindows; /**< ������ ����. */
	cLogStream* mLog;     /**< ���. */

//functions
	apWindowsManager();
	~apWindowsManager();

	void initialize();

	/** �������� ����������� ����. */
	apWindow* addStdWindow(fRect windowRect, std::string title);

	/** �������� ����. */
	apWindow* addWindow(apWindow* window);

	/** ������ ���� ����. */
	void allUpdate();

	/** ��������� ��������� ����. */
	LRESULT procWindows(HWND wnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif