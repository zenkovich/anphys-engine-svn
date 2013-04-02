#ifndef RENDER_STATE_INTERFACE_H
#define RENDER_STATE_INTERFACE_H

struct grRenderBase;

struct grRenderState
{
	enum PolygonCullMode { PCM_NONE = 0, PCM_CLOCKWISE, PCM_COUNETRCLOCKWISE };

	grRenderBase* mRender;

//functions
	grRenderState():mRender(0) {}
	grRenderState(grRenderBase* render):mRender(render) {}
	virtual ~grRenderState() {}

	virtual void begin() {}
	virtual void finish() {}
};

#endif //RENDER_STATE_INTERFACE_H