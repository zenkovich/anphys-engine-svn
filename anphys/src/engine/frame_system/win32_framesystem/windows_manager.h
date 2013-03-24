#pragma once
#include "engine/engine_options.h"
#ifdef PLATFORM_WIN32

#include <windows.h>
#include <vector>
#include <string>
#include "util/math/mmath.h"

struct apWindow;
struct cLogStream;

/** Менеджер окон. */
struct apWindowsManager
{
	typedef std::vector<apWindow*> WindowsList;

	WindowsList mWindows; /**< Список окон. */
	cLogStream* mLog;     /**< Лог. */

//functions
	apWindowsManager();
	~apWindowsManager();

	void initialize();

	/** Добавить стандартное окно. */
	apWindow* addStdWindow(fRect windowRect, std::string title);

	/** Добавить окно. */
	apWindow* addWindow(apWindow* window);

	/** Апдейт всех окон. */
	void allUpdate();

	/** Обработка сообщений окон. */
	LRESULT procWindows(HWND wnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif