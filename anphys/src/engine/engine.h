#ifndef ENGINE_H
#define ENGINE_H


#include <vector>
#include <string>

#include "util/utils.h"

struct grRenderFrame;

struct cEngine:public cSingleton<cEngine>
{
	typedef std::vector<grRenderFrame*> RenderFramesList;

	RenderFramesList mFrames;
	cFileSystem*     mFileSystem;
	cLogStream*      mLog;

//functions
	virtual ~cEngine();

	void initialize();
	void run();
	void exitEngine();

	grRenderFrame* addWindowFrame(const std::string& title, fRect rect);
	grRenderFrame* addFrame(grRenderFrame* newFrame);
};

#define getEngine() cEngine::instance()

#endif //ENGINE_H