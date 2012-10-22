#ifndef ENGINE_H
#define ENGINE_H


#include <vector>
#include <string>

#include "../util/math/mmath.h"

struct grRenderFrame;

struct cEngine
{
	typedef std::vector<grRenderFrame*> RenderFramesList;
	RenderFramesList mFrames;

	void initialize();
	void run();
	void exitEngine();

	grRenderFrame* addWindowFrame(const std::string& title, fRect rect);
	grRenderFrame* addFrame(grRenderFrame* newFrame);
};


#endif //ENGINE_H