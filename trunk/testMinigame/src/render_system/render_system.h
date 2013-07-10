#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <vector>

#include "dx8/include/d3d8.h"
#include "dx8/include/d3dx8.h"
#include "dx8/include/d3dx8core.h"

#include "math/vector2.h"
#include "math/vertex.h"

//simple d3d vertex format
#define D3DFVF_VERTEX_2D (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX2)

class Sprite;
class RenderSystem;

/** Texture. Contains texture object pointer, tex size, reference count and file name. */
class Texture
{	
	friend class RenderSystem;

	RenderSystem*      mRenderSystem;
	LPDIRECT3DTEXTURE8 mTexturePtr;
	vec2f              mSize;
	int                mRefCount;
	char               mFilename[128];

public:
	Texture();
	Texture(RenderSystem* renderSystem, const char* fileName);
	virtual ~Texture();

	bool  load(RenderSystem* renderSystem, const char* fileName);
		  
	void  incRefCount();
	void  decRefCount();
	int   getRefCount();

	vec2f getSize() const;

	char* const getFileName();
};
typedef std::vector<Texture*> TexturesVec;

/** Render system. Drawing 2D Sprites with batching, managing textures. */
class RenderSystem
{	
	enum { nVertexBufferSize = 6000 };

//d3d
	LPDIRECT3D8             mDirect3D;
	LPDIRECT3DDEVICE8       mDirect3DDevice;
	D3DPRESENT_PARAMETERS   mDirect3DParametr;
	vec2i                   mResolution;          /**< Window client rect size. */
	
//textures
	TexturesVec             mTextures;
		
//vertex & index buffers
	vertex2*                mVertexData;          /**< Pointer to vertex data buffer. */
	LPDIRECT3DVERTEXBUFFER8 mVertexBuffer;         
	LPDIRECT3DINDEXBUFFER8  mIndexBuffer;
	
//batching parametres
	Texture*                mLastDrawTexture;     /**< Stored texture ptr from last DIP. */
	unsigned int            mLastDrawVertex;      /**< Last vertex idx for next DIP. */
	unsigned int            mTrianglesCount;      /**< Triatgles count for next DIP. */
	unsigned int            mFrameTrianglesCount; /**< Total triangles at current frame. */
	unsigned int            mDIPCount;            /**< DrawIndexedPrimitives calls count. */

	bool                    mReady;               /**< True, if render system initialized. */

public:
	RenderSystem();
	~RenderSystem();

	/** Initializing D3D8 render. */
	bool     initialize(HWND hwnd, const vec2i& resolution);
		   
	/** Calls at beginning render. */
	bool     beginRender();
	
	/** Calls at finishing render. */
	bool     endRender();

	/** Creates texture, if not exist texture with same file name, else returning ptr to exist texture. */
	Texture* createTexture(const char* fileName);

	/** Get texture by filename. */
	Texture* getTexture(const char* fileName);

	/** Releasing texture. If texture reference count < 0 - destroy texture. */
	void     releaseTexture(Texture* texture);

	/** Remove all textures ignoring ref counters. */
	void     removeAlltextures();

	/** Sending sprite geometry to render pipeline. */
	void     drawSprite(Sprite* sprite);

	/** Returns direct3d8 device. */
	LPDIRECT3DDEVICE8 getDirect3DDevice() { return mDirect3DDevice; }

	vec2i    getResolution() const { return mResolution; }

private:
	/** Lock vertex buffer. */
	void lockBuffer();

	/** Unlock vertex buffer. */
	void unlockBuffer();

	/** Send vertex buffer data to render. */
	void drawPrimitives();
};

#endif //RENDER_SYSTEM_H