#ifndef APPLICATION_BASE_INTERFACE_H
#define APPLICATION_BASE_INTERFACE_H

#include "public.h"
#include "util/singleton.h"
#include "util/input/input_message.h"
#include "util/callback.h"

OPEN_O2_NAMESPACE

class grRenderSystem;
class LogStream;
class FileSystem;
class Scheduler;
class TimeUtil;
class Timer;
class uiController;
class ProjectConfig;
class Assets;
	
/** Basic application class. Not implementing frame data. 
  * Containing input message and systems:
  * Project configs
  * Application log stream,
  * Render system,
  * File system,
  * Scheduler,
  * Time utils,
  * UI controller
 */
class cApplicationBaseInterface: public Singleton<cApplicationBaseInterface>
{
protected:
	ProjectConfig*  mProjectConfig; /**< Project config. */
	InputMessage*  mInputMessage;  /**< While application user input message. */
	LogStream*     mLog;           /**< Log stream with id "app", using only for application messages. */
	grRenderSystem* mRenderSystem;  /**< Render system. */
	FileSystem*    mFileSystem;    /**< File system. */
	Scheduler*     mScheduler;     /**< Scheduler. */
	TimeUtil*      mTimeUtils;     /**< Time utilities. */
	uiController*   mUIController;  /**< User interface controller host. */				
	Assets*         mAssets;        /**< Assets. */
						   
	Timer*         mTimer;         /**< Timer for detecting delta time for update. */

public:
	CallbackChain onActivatedEvent;   /**< On Activated event callbacks. */
	CallbackChain onDeactivatedEvent; /**< On deactivated event callbacks. */
	CallbackChain onStartedEvent;     /**< On started event callbacks. */
	CallbackChain onClosingEvent;     /**< On closing event callbacks. */
	CallbackChain onResizingEvent;    /**< On resized app window callbacks. Ignoring on mobiles/tables. */
	CallbackChain onMovingEvent;      /**< On moving app window callbacks. Ignoring on mobiles/tables. */

	/** ctor. */
	cApplicationBaseInterface();

	/** dtor. */
	virtual ~cApplicationBaseInterface();

	/** Returns pointer to input message object. */
	InputMessage* getInputMessage();

	/** Returns pointer to render system object. */
	grRenderSystem* getRenderSystem() const;
	
	/** Returns pointer to log object. */
	LogStream* getLog() const;
	
	/** Returns pointer to project config object. */
	ProjectConfig* getProjectConfig() const;
	
	/** Returns pointer to file system object. */
	FileSystem* getFileSystem() const;
	
	/** Returns pointer to scheduler object. */
	Scheduler* getScheduler() const;
	
	/** Returns pointer to time utils object. */
	TimeUtil* getTimeUtils() const;
	
	/** Returns pointer to ui controller object. */
	uiController* getUIController() const;
	
	/** Returns pointer to assets object. */
	Assets* getAssets() const;

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
	virtual vec2i getWindowSize() const { return getContentSize(); }

	/** Sets application window position. On mobiles/tablets has no effect, just ignoring. */
	virtual void setWindowPosition(const vec2i& position) {}

	/** Returns application window position. On mobiles/tablets return zero vector. */
	virtual vec2i getWindowPosition() const { return vec2i(); }

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

	/** Calls when application activated. */
	virtual void onActivated() {}

	/** Calls when application deactivated. */
	virtual void onDeactivated() {}

	/** Calls when application is starting. */
	virtual void onStarted() {}

	/** Calls when application is closing. */
	virtual void onClosing() {}

	/** Calls when application window resized. Ignoring on mobiles/tablets. */
	virtual void onResizing() {}

	/** Calls when application window moved. Ignoring on mobiles/tablets. */
	virtual void onMoved() {}
};

inline InputMessage* appInput() { return cApplicationBaseInterface::instancePtr()->getInputMessage(); }
inline grRenderSystem* renderSystem() { return cApplicationBaseInterface::instancePtr()->getRenderSystem(); }
inline ProjectConfig* projectConfig() { return cApplicationBaseInterface::instancePtr()->getProjectConfig(); }
inline FileSystem* fileSystem() { return cApplicationBaseInterface::instancePtr()->getFileSystem(); }
inline Scheduler* scheduler() { return cApplicationBaseInterface::instancePtr()->getScheduler(); }
inline TimeUtil* timeUtils() { return cApplicationBaseInterface::instancePtr()->getTimeUtils(); }
inline uiController* uiHost() { return cApplicationBaseInterface::instancePtr()->getUIController(); }
inline Assets* assets() { return cApplicationBaseInterface::instancePtr()->getAssets(); }

CLOSE_O2_NAMESPACE

#endif //APPLICATION_BASE_INTERFACE_H