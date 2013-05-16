#include "render.h"

#ifdef RENDER_D3D8
#include "engine/frame_system/win32_render_frame/render_frame_win32_window.h"
#endif //RENDER_D3D8

//util
#include "../util/debug/render_stuff.h"
#include "../util/other/singleton.h"

grRender::grRender():grRenderBase(), mFrame(NULL) {}

grRender::grRender(grRenderFrame* frame, fRect outputRect)
{
	mFrame = frame;
#ifdef RENDER_D3D8

	grRenderBase::initialize(static_cast<apRenderWindow*>(frame)->mHWnd, outputRect);

#endif //RENDER_D3D8


	mRenderStuff = new cRenderStuff;
	cRenderStuff::initializeSingleton();

	createStdMaterials();

	grRenderSceneBaseInterface* scene = mSceneManager->addScene(new grRenderSceneBaseInterface());
	mRenderStuff->initialize(scene);
}

grRender::~grRender() 
{	
	safe_release(mRenderStuff);
}

void grRender::createStdMaterials()
{	
	mMaterials->addMaterial(new grMaterial("redMaterial", color4(1.0f,0.0f,0.0f,1.0f), 
		color4(1.0f,0.0f,0.0f,1.0f), color4(1.0f,0.0f,0.0f,1.0f), color4(1.0f,0.0f,0.0f,1.0f), 1));
	mMaterials->addMaterial(new grMaterial("blueMaterial", color4(0.0f,0.0f,1.0f,1.0f), 
		color4(0.0f,0.0f,1.0f,1.0f), color4(0.0f,0.0f,1.0f,1.0f), color4(1.0f,0.0f,0.1f,1.0f), 1));
	mMaterials->addMaterial(new grMaterial("greenMaterial", color4(0.0f,1.0f,0.0f,1.0f), 
		color4(0.0f,1.0f,0.0f,1.0f), color4(0.0f,1.0f,0.0f,1.0f), color4(0.0f,1.0f,0.0f,1.0f), 1));
	mMaterials->addMaterial(new grMaterial("whiteMaterial", color4(1.0f,1.0f,1.0f,1.0f), 
		color4(1.0f,1.0f,1.0f,1.0f), color4(1.0f,1.0f,1.0f,1.0f), color4(1.0f,1.0f,1.0f,1.0f), 1));

	grTexture* tex = mTextures->createTexture("../data/textures/white.jpg");

	grSurfaceMaterial* rmat = mSurfaceMaterials->addSurfaceMaterial(new grSurfaceMaterial("redMaterial"));
	rmat->setMaterial(mMaterials->getMaterial("redMaterial"));
	rmat->pushTexture(tex);
	rmat->setShadeModel(NULL);

	grSurfaceMaterial* bmat = mSurfaceMaterials->addSurfaceMaterial(new grSurfaceMaterial("blueMaterial"));
	bmat->setMaterial(mMaterials->getMaterial("blueMaterial"));
	bmat->pushTexture(tex);
	bmat->setShadeModel(NULL);

	grSurfaceMaterial* gmat = mSurfaceMaterials->addSurfaceMaterial(new grSurfaceMaterial("greenMaterial"));
	gmat->setMaterial(mMaterials->getMaterial("greenMaterial"));
	gmat->pushTexture(tex);
	gmat->setShadeModel(NULL);
}