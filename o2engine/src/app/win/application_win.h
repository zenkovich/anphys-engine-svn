#ifndef APPLICATION_WIN_H
#define APPLICATION_WIN_H

#include <Windows.h>

#include "public.h"
#include "../application_base_interface.h"

OPEN_O2_NAMESPACE

class Timer;

/** Windows applacation. 
  * Contains window handle, options
  */
class cApplication:public cApplicationBaseInterface
{
protected:
	friend class grRenderSystem;
	friend class cDeviceInfo;

	HWND           mHWnd;                /**< Window handle. */
	unsigned int   mWndStyle;            /**< Window styles data. */
		           					   
	bool           mWindowed;            /**< True if app in windowed mode, false if in fullscreen mode. */
	bool           mWindowResizible;     /**< True, if window can be sized by user. */
	vec2i          mWindowedSize;        /**< Size of window. */
	vec2i          mWindowedPos;         /**< Position of window. */				   				   
	string         mWndCaption;          /**< Window caption. */				   				   
	bool           mActive;              /**< True, if window is active. */

	static cApplication* mApplication;   /**< Static ptr for wndProc func. */

public:
	cApplication();
	~cApplication();

	/** Launching application cycle. */
	void launch();

	/** Called on updating. */
	void onUpdate(float dt);

	/** Called on drawing. */ 
	void onDraw();

	/** Makes application windowed. On mobiles/tablets has no effect, just ignoring. */
	void setWindowed();
	
	/** Makes application fullscreen. On mobiles/tablets has no effect, just ignoring. */
	void setFullscreen();

	/** Return true, if application is fullscreen On mobiles/tables always true. */
	bool isFullScreen() const;

	/** Sets application window as resizible. On mobiles/tablets has no effect, just ignoring. */
	void setResizible(bool resizible);

	/** Returns true, if application is resizible. On mobiles/tablets always returns false. */
	bool isResizible() const;

	/** Sets application window size. On mobiles/tablets has no effect, just ignoring. */
	void setWindowSize(const vec2i& size);

	/** Returns application window size. On mobiles/tablets returns content size. */
	vec2i getWindowSize() const;

	/** Sets application window position. On mobiles/tablets has no effect, just ignoring. */
	void setWindowPosition(const vec2i& position);

	/** Returns application window position. On mobiles/tablets return zero vector. */
	vec2i getWindowPosition() const;

	/** Sets application window caption. On mobiles/tablets has no effect, just ignoring. */
	void setWindowCaption(const string& caption);

	/** Returns application window caption. On mobiles/tablets returns empty string. */
	string getWindowCaption() const;

	/** Sets inside content size. */
	void setContentSize(const vec2i& size);

	/** Returns inside content size. */
	vec2i getContentSize() const;

protected:
	/** Initializing window. */
	void initializeWindow();

	/** Reset window by window parameters. */
	void resetWnd();

	/** Window proc func. */
	static LRESULT wndProc(HWND wnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

CLOSE_O2_NAMESPACE

#endif //APPLICATION_WIN_H