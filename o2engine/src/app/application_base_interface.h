#ifndef APPLICATION_BASE_INTERFACE_H
#define APPLICATION_BASE_INTERFACE_H

#include "public.h"
#include "util/singleton.h"
#include "util/input/input_message.h"

OPEN_O2_NAMESPACE

/** Applacation options. */
struct cApplicationOption
{
	enum type {
		WINDOWED = 0,               /**< Frame in windowed mode. Available on WIN, MAC. */
		FULLSCREEN,                 /**< Frame in fulscreen mode. Available on WIN, MAC. */
		RESIZIBLE,                  /**< Frame as resizible by user. Available on WIN, MAC. */
		AUTO_AJUST_BY_SCREEN_SPACE, /**< Frame as auto ajusting by available screen space. Available on WIN, MAC. */
		WND_SIZE,                   /**< Frame size. Available on WIN, MAC. */
		WND_POSITION,               /**< Frame position. Available on WIN, MAC. */
		WND_CAPTION,                /**< Frame caption. Available on WIN, MAC. */
		CLIENT_RECT                 /**< Frame client area rect. */
	};
};

/** Application message. */
struct cApplacationMessage
{
	enum type {
		ON_ACTIVATED = 0, /**< Application activated. */
		ON_DEACTIVATED,   /**< Application deactivated. */
		ON_STARTED,       /**< Application started. */
		ON_CLOSING,       /**< Application closing. */
		ON_SIZING,        /**< Application frame changed size. Available on WIN, MAC. */
		ON_MOVING         /**< Application frame changed position. Available on WIN, MAC. */
	};
};

class grRenderSystem;
class cLogStream;
class cFileSystem;
class cScheduler;
class cTimeUtil;
class cTimer;
class uiController;
	
/** Basic application class. Not implementing frame data. 
  * Containing input message and systems:
  * Application log stream,
  * Render system,
  * File system,
  * Scheduler,
  * Time utils,
  * UI controller
 */
class cApplicationBaseInterface: public cSingleton<cApplicationBaseInterface>
{
protected:
	shared<cInputMessage>  mInputMessage; /**< While application user input message. */
	shared<cLogStream>     mLog;          /**< Log stream with id "app", using only for application messages. */
	shared<grRenderSystem> mRenderSystem; /**< Render system. */
	shared<cFileSystem>    mFileSystem;   /**< File system. */
	shared<cScheduler>     mScheduler;    /**< Scheduler. */
	shared<cTimeUtil>      mTimeUtils;    /**< Time utilities. */
	shared<uiController>   mUIController; /**< User interface controller host. */									   
	shared<cTimer>         mTimer;        /**< Timer for detecting delta time for update. */

public:
	/** ctor. */
	cApplicationBaseInterface();

	/** dtor. */
	virtual ~cApplicationBaseInterface();

	/** Returns pointer to input message object. */
	shared<cInputMessage> getInputMessage();

	/** Returns pointer to input message object. */
	shared<grRenderSystem> getRenderSystem() const;

	/** Launching application cycle. */
	virtual void launch() {}

	/** Called on updating. */
	virtual void onUpdate(float dt) {}
	
	/** Called on drawing. */ 
	virtual void onDraw() {}

	/** Makes application windowed. On mobiles/tablets has no effect, just ignoring. */
	virtual void setWindowed() {}
	
	/** Makes application fullscreen. On mobiles/tablets has no effect, just ignoring. */
	virtual void setFullscreen() {}

	/** Return true, if application is fullscreen On mobiles/tables always true. */
	virtual bool isFullScreen() const { return true; }

	/** Sets application window as resizible. On mobiles/tablets has no effect, just ignoring. */
	virtual void setResizible(bool resizible) {}

	/** Returns true, if application is resizible. On mobiles/tablets always returns false. */
	virtual bool isResizible() const { return false; }

	/** Sets application window size. On mobiles/tablets has no effect, just ignoring. */
	virtual void setWindowSize(const vec2i& size) {}

	/** Returns application window size. On mobiles/tablets returns content size. */
	virtual vec2i getWindowSize() const { getContentSize(); }

	/** Sets application window caption. On mobiles/tablets has no effect, just ignoring. */
	virtual void setWindowCaption(const string& caption) {}

	/** Returns application window caption. On mobiles/tablets returns empty string. */
	virtual string getWindowCaption() const { return ""; }

	/** Sets inside content size. */
	virtual void setContentSize(const vec2i& size) {}

	/** Returns inside content size. */
	virtual vec2i getContentSize() const {return vec2i(); }

protected:
	/** Initializing all systems and log. Call it when creating applications*/
	void initalizeSystems();

	/** Deinitializing systems. */
	void deinitializeSystems();

	/** Processing frame update, drawing and input messages. */
	void processFrame();

	/** Called on message processing. */
	virtual void processMessage(cApplacationMessage::type message) {}

	virtual void onActivated() {}
	virtual void onDeactivated() {}
};

CLOSE_O2_NAMESPACE

#endif //APPLICATION_BASE_INTERFACE_H