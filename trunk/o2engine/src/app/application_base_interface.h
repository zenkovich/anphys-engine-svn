#ifndef APPLICATION_BASE_INTERFACE_H
#define APPLICATION_BASE_INTERFACE_H

#include "public.h"
#include "util/input/input_message.h"

OPEN_O2_NAMESPACE

struct cApplicationOption
{
	enum type {
		WINDOWED = 0, 
		FULLSCREEN, 
		RESIZIBLE_ON, 
		RESIZIBLE_OFF, 
		AUTO_AJUST_BY_SCREEN_SPACE,
		WND_SIZE,
		WND_CAPTION
	};
};

struct cApplacationMessage
{
	enum type {
		ON_ACTIVATED = 0,
		ON_DEACTIVATED,
		ON_STARTED,
		ON_CLOSING
	};
};

class grRenderSystem;
class cLogStream;
class cFileSystem;
	
class cApplicationBaseInterface
{
protected:
	cInputMessage   mInputMessage;

	cLogStream*     mLogStream;
	grRenderSystem* mRenderSystem;
	cFileSystem*    mFileSystem;

public:
	cApplicationBaseInterface();
	virtual ~cApplicationBaseInterface();

	virtual void onUpdate(float dt) {}
	virtual void onDraw() {}

	virtual void setOptions(cApplicationOption::type option, ...) {}

	virtual void processMessage(cApplacationMessage::type message) {}

	virtual void launch() {}

protected:
	void initalizeSystems();
	void deinitializeSystems();
};

CLOSE_O2_NAMESPACE

#endif //APPLICATION_BASE_INTERFACE_H