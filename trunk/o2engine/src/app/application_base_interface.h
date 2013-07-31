#ifndef APPLICATION_BASE_INTERFACE_H
#define APPLICATION_BASE_INTERFACE_H

#include "public.h"
#include "util/input/input_message.h"

OPEN_O2_NAMESPACE

/** Applacation options. */
struct cApplicationOption
{
	enum type {
		WINDOWED = 0,               /**< Setting frame in windowed mode. Available on WIN, MAC. */
		FULLSCREEN,                 /**< Setting frame in fulscreen mode. Available on WIN, MAC. */
		RESIZIBLE,                  /**< Setting frame as resizible by user. Available on WIN, MAC. */
		AUTO_AJUST_BY_SCREEN_SPACE, /**< Setting frame as auto ajusting by available screen space. Available on WIN, MAC. */
		WND_SIZE,                   /**< Setting frame size. Available on WIN, MAC. */
		WND_POSITION,               /**< Setting frame position. Available on WIN, MAC. */
		WND_CAPTION                 /**< Setting frame caption. Available on WIN, MAC. */
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
	
/** Basic application class. Not implementing frame data. 
  * Containing input message and systems:
  * Application log stream
  * Render system
  * File system. 
 */
class cApplicationBaseInterface
{
protected:
	cInputMessage   mInputMessage; /**< While application user input message. */

	cLogStream*     mLogStream;    /**< Log stream with id "app", using only for applacation messages. */
	grRenderSystem* mRenderSystem; /**< Render system. */
	cFileSystem*    mFileSystem;   /**< File system. */

public:
	cApplicationBaseInterface();
	virtual ~cApplicationBaseInterface();

	/** Launching application cycle. */
	virtual void launch() {}

	/** Called on updating. */
	virtual void onUpdate(float dt) {}
	
	/** Called on drawing. */ 
	virtual void onDraw() {}

	/** Sets option for application. */
	virtual void setOption(cApplicationOption::type option, ...) {}

	/** Called on message processing. */
	virtual void processMessage(cApplacationMessage::type message) {}

protected:
	/** Initializing all systems and log. Call it when creating applications*/
	void initalizeSystems();

	/** Deinitializing systems. */
	void deinitializeSystems();
};

CLOSE_O2_NAMESPACE

#endif //APPLICATION_BASE_INTERFACE_H