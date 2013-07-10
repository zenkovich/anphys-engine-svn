#ifndef SPRITE_H
#define SPRITE_H

#include <string>

#include "math/vector2.h"
#include "math/rect.h"
#include "math/vertex.h"
#include "math/color.h"

class RenderSystem;
class Texture;

/** Sprite. Have texture, texture src rect, scale rotation with geometry optimizations. */
class Sprite
{
	friend class RenderSystem;

	RenderSystem* mRenderSystem;

	vec2f         mPosition;                /**< Left top corner position, without rotation. */
	vec2f         mSize;                    /**< Size of sprite. */
	vec2f         mScale;                   /**< Scale of sprite. scale (1, 1) is standart. Result sprite size = mSize*mScale. */
	float         mAngle;                   /**< Rotation angle in degrees. */
	vec2f         mRotationCenter;          /**< Center of rotation in local coordinates. */
	fRect         mTextureSrcRect;          /**< texture source rect, in pixels. */
			      				            
	vertex2       mVerticies[4];            /**< Vertex geometry. */
	Texture*      mTexture;                 /**< Texture. Can be NULL. */
			      
	bool          mNeedUpdateMeshVerticies; /**< true, if need update mesh geometry. */
	bool          mNeedUpdateMeshTexCoords; /**< true, if need update texture coords. */

public:
	/** Sprite ctor. 
	 *  @param render         pointer to render system? can't be NULL.
	 *  @param texture        texture pointer, can be NULL. 
	 *  @param textureSrcRect texture source rect, in pixels. if left top < 0, using full texture as source. 
	 *  @param size           size of sprite, if x < 0 - sprite will be as texture src size. */
	Sprite(RenderSystem* render, Texture* texture = NULL, const fRect& textureSrcRect = fRect(-1.0f, 0.0f, 0.0f, 0.0f), 
		     const vec2f& position = vec2f(0.0f, 0.0f), const vec2f& size = vec2f(-1.0f, 0.0f), 
			 const vec2f& scale = vec2f(1.0f, 1.0f), float angle = 0, const vec2f& rotationCenter = vec2f(0.0f, 0.0f),
			 const color4& color = color4(1.0f, 1.0f, 1.0f, 1.0f));

	/** Copy ctor. */
	Sprite(const Sprite& sprite);

	/** dtor. */
	~Sprite();

	/** Creates same sprite. */
	Sprite*  clone() const;
			
	/** Draw sprite. */
	void     draw();
			 
	/** Setting position of left top corner. */
	Sprite&  setPosition(const vec2f& position);

	/** Returns position of left top corner. */
	vec2f    getPosition() const;
			 
	/** Setting scale. */
	Sprite&  setScale(const vec2f& scale);

	/** Returns scale. */
	vec2f    getScale() const;
			 
	/** Setting angle in degrees. */
	Sprite&  setAngle(float angle);

	/* Returns angle in degrees. */
	float    getAngle() const;
			 
	/** Setting rotation center in local coordinates. */
	Sprite&  setRotationCenter(const vec2f& center);

	/** Returns rotation center in local coordinates. */
	vec2f    getRotationCenter() const;
			 
	/** Setting texture src rect, in pixels. */
	Sprite&  setTextureSrcRect(const fRect& rect);

	/** Return texture src rect, in pixels. */
	fRect    getTextureSrcRect() const;

	/** Setting texture, if texture is NULL - sprite will be as solid color quad. */
	Sprite&  setTexture(Texture* texture);

	/** Returns texture ptr. */
	Texture* getTexture() const;

	/** Setting sprite size. */
	Sprite&  setSize(const vec2f& size);

	/** return sprite size, without scaling. */
	vec2f    getSize() const;

	/** Setting color for vertex.
	 *  @param color color of vertex
	 *  @param vertexId index of vertex (0,1,2,3), if vertexId < 0, setting color for all verticies. */
	Sprite&  setColor(const color4& color, int vertexId = -1);

	/** Returns color of vertex[vertexId]. */
	color4   getColor(int vertexId = 0);
	
protected:
	/** Updating mesh verticies coords. */
	void updateMeshVerticies();

	/** Updating mesh verticies texture coords. */
	void updateMeshTexCoords();
};

#endif //SPRITE_H