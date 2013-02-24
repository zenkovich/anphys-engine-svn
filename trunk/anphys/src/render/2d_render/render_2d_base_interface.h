#ifndef RENDER_2D_BASE_INTERFACE_H
#define RENDER_2D_BASE_INTERFACE_H

struct grRenderBaseInterface;
struct grRender;
struct grRender2DObjectMeshBase;
struct grCamera2D;

struct grRender2DBaseInterface
{
	grRender*   mRender;

	grCamera2D* mCamera;

	bool        mRendering;
	bool        mReady;

	grRender2DBaseInterface(grRenderBaseInterface* render);
	virtual ~grRender2DBaseInterface();

	virtual void beginRender() {}
	virtual void endRender() {}

	virtual void drawMesh(grRender2DObjectMeshBase* renderMesh) {}
};

#endif //RENDER_2D_BASE_INTERFACE_H