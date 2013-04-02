#include "engine/engine_options.h"

#ifdef RENDER_D3D8
#include "render_2d_object_mesh_d3d8.h"

//render
#include "render/render.h"
#include "render/render_objects/object_manager.h"
#include "render/render_state/2d_render_state.h"

grRender2DObjectMeshBase::grRender2DObjectMeshBase():grRender2DObjectMeshBaseInterface(), mLastUsedVertexDataIndex(0),
	mLastDrawingFrameIndex(0)
{

}

grRender2DObjectMeshBase::grRender2DObjectMeshBase( unsigned int vertexCount, unsigned int polyCount )
	:grRender2DObjectMeshBaseInterface(vertexCount, polyCount), mLastUsedVertexDataIndex(0), mLastDrawingFrameIndex(0)
{
}

grRender2DObjectMeshBase::~grRender2DObjectMeshBase()
{
}

void grRender2DObjectMeshBase::draw()
{
	if (mRenderObjectsManager->mRender->mCurrentRenderState->getType() == gr2DRenderState::getStaticType())
	{
		static_cast<gr2DRenderState*>(mRenderObjectsManager->mRender->mCurrentRenderState)->drawMesh(this);
	}
}

#endif //RENDER_D3D8
