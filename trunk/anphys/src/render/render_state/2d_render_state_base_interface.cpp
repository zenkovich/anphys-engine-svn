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

void gr2DRenderStateBaseInterface::pushRect( const vec2& leftTop, const vec2& rightDown, const color4& color /*= color4(0, 255, 0, 255)*/ )
{
	unsigned long colr = color.dwordARGB();

	mDebugVertexBuffer[mDebugLinesCount++] = vertex2d(leftTop.x, leftTop.y, 1.0f, 0.0f, 0.0f, colr);
	mDebugVertexBuffer[mDebugLinesCount++] = vertex2d(rightDown.x, leftTop.y, 1.0f, 0.0f, 0.0f, colr);

	mDebugVertexBuffer[mDebugLinesCount++] = vertex2d(rightDown.x, leftTop.y, 1.0f, 0.0f, 0.0f, colr);
	mDebugVertexBuffer[mDebugLinesCount++] = vertex2d(rightDown.x, rightDown.y, 1.0f, 0.0f, 0.0f, colr);

	mDebugVertexBuffer[mDebugLinesCount++] = vertex2d(rightDown.x, rightDown.y, 1.0f, 0.0f, 0.0f, colr);
	mDebugVertexBuffer[mDebugLinesCount++] = vertex2d(leftTop.x, rightDown.y, 1.0f, 0.0f, 0.0f, colr);

	mDebugVertexBuffer[mDebugLinesCount++] = vertex2d(leftTop.x, rightDown.y, 1.0f, 0.0f, 0.0f, colr);
	mDebugVertexBuffer[mDebugLinesCount++] = vertex2d(leftTop.x, leftTop.y, 1.0f, 0.0f, 0.0f, colr);
}
