#include "2d_render_state_base_interface.h"

#include "render/camera/camera_2d.h"

gr2DRenderStateBaseInterface::gr2DRenderStateBaseInterface():grRenderState(), mCamera(NULL), 
	mPolygonCullMode(PCM_COUNETRCLOCKWISE), mDebugLinesCount(0)
{
}

gr2DRenderStateBaseInterface::gr2DRenderStateBaseInterface( grRenderBase* render ):grRenderState(render), mCamera(NULL), 
	mPolygonCullMode(PCM_COUNETRCLOCKWISE), mDebugLinesCount(0)
{
}

gr2DRenderStateBaseInterface::~gr2DRenderStateBaseInterface()
{
}

void gr2DRenderStateBaseInterface::pushLine( const vec2& start, const vec2& end, const color4& color )
{
	mDebugVertexBuffer[mDebugLinesCount++] = vertex2d(start.x, start.y, 1.0f, 0.0f, 0.0f, color.dwordARGB());
	mDebugVertexBuffer[mDebugLinesCount++] = vertex2d(end.x, end.y, 1.0f, 0.0f, 0.0f, color.dwordARGB());
}
