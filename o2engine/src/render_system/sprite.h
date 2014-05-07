#ifndef SPRITE_H
#define SPRITE_H

#include "public.h"

#include "texture.h"
#include "util/objects.h"
#include "util/property.h"
#include "util/math/vector2.h"
#include "util/math/rect.h"
#include "util/math/color.h"
#include "util/serialization/serialization.h"

OPEN_O2_NAMESPACE

class grMesh;
class grRenderSystem;
	
/** Sprite, just a quad with texture. */
class grSprite:public IDrawable
{
	vec2f   mPosition;       /** Position of the sprite. All sprite position already depends on mPivot too. */
	vec2f   mSize;           /** Size of sprite, in pixels. Real sprite size is mSize*mScale. */
	vec2f   mScale;          /** Scale of sprite. Real sprite size is mSize*mScale. */
	float   mAngle;          /** Rotation angle. */
	vec2f   mPivot;          /** Pivot position, in pixels. */
	fRect   mTextureSrcRect; /** texture src rect. */

	shared(grMesh) mMesh;                    /** Mesh. */

	bool           mNeedUpdateMeshVerticies; /** True, when mesh vertex positions needs to update. */
	bool           mNeedUpdateMeshTexCoords; /** True, when mesh vertex tex coords needs to update. */

public:
	//properties
	PROPERTY(grSprite, vec2f)     position;       /** Position property. Uses set/getPosition. */
	PROPERTY(grSprite, vec2f)     size;           /** Size property. Uses set/getSize. */
	PROPERTY(grSprite, vec2f)     scale;          /** Scale property. Uses set/getScale. */
	PROPERTY(grSprite, float)     angle;          /** Angle property. Uses set/getAngle. */
	PROPERTY(grSprite, vec2f)     pivot;          /** Pivot property. Uses set/getPivot. */
	PROPERTY(grSprite, vec2f)     relativePivot;  /** Relative pivot property. Uses set/getRelativePivot. */
	PROPERTY(grSprite, fRect)     textureSrcRect; /** Texture src rect property. Uses set/getTextureSrcRect. */	
	PROPERTY(grSprite, grTexture) texture;        /** Texture property. uses set/getTexture. */

	/** ctor.
	 *  @param texture        - texture ptr, use NULL if no texture 
	 *  @param textureSrcRect - texture source rect, if left is < 0 - use all texture size
	 *  @param position       - position of left top corner of sprite
	 *  @param size           - size of sprite, in pixels. If x < 0 - size as texture source rect
	 *  @param scale          - scale of the sprite
	 *  @param angle          - rotation of sprite, in degrees
	 *  @param rotationCenter - center of sprite rotation
	 *  @param color          - color of the sprite. */
	grSprite(grTexture texture = grTexture(), const fRect& textureSrcRect = fRect(-1.0f, 0.0f, 0.0f, 0.0f), 
		     const vec2f& position = vec2f(0.0f, 0.0f), const vec2f& size = vec2f(-1.0f, 0.0f), 
			 const vec2f& scale = vec2f(1.0f, 1.0f), float angle = 0, const vec2f& rotationCenter = vec2f(0.0f, 0.0f),
			 const color4& color = color4(1.0f, 1.0f, 1.0f, 1.0f));

	//grSprite(grRenderSystem* render, cDataObject& dataObject);
	
	/** ctor. Loads sprite from config file. */
	grSprite(pugi::xml_node& xmlNode);

	/** copy ctor. */
	grSprite(const grSprite& sprite);

	/** dtor. */
	~grSprite();

	/** Returns cloned sprite. */
	grSprite* clone() const;

	/** Drawing that sprite. */
	void draw();

	/** Sets position of sprite. */
	void setPosition(const vec2f& position);

	/** Returns position of sprite. */
	vec2f getPosition() const;

	/** Sets scale of sprite. */
	void setScale(const vec2f& scale);

	/** Returns scale of sprite. */
	vec2f getScale() const;

	/** Sets angle of sprite. */
	void setAngle(const float& angle);

	/** Returns angle of sprite. */
	float getAngle() const;

	/** Sets rotation center of sprite. */
	void setPivot(const vec2f& center);

	/** Returns rotation center of sprite. */
	vec2f getPivot() const;

	/** Sets relative pivot. */
	void setRelativePivot(const vec2f& relCenter);

	/** Returns relative pivot position. */
	vec2f getRelativePivot() const;

	/** Sets texture source rect of sprite. */
	void setTextureSrcRect(const fRect& rect);

	/** Returns texture source rect of sprite. */
	fRect getTextureSrcRect() const;

	/** Sets texture of sprite. */
	void setTexture(const grTexture& texture);

	/** Returns texture of sprite. */
	grTexture getTexture() const;

	/** Sets size of sprite. */
	void setSize(const vec2f& size);

	/** Returns size of sprite. */
	vec2f getSize() const;

	/** Sets the color of vertex sprite. If vertex id < 0 - setting color for all sprite. */
	void setColor(const color4& color, int vertexId = -1);

	/** Returns color of the sprite vertex. */
	color4 getColor(int vertexId = 0);

	SERIALIZE_METHOD_DECL();

protected:
	/** Initializing properties. */
	void initializeProperties();

	/** Updating mesh vertices positions. */
	void updateMeshVerticies();

	/** Updating mesh vertices texture coords. */
	void updateMeshTexCoords();
};

CLOSE_O2_NAMESPACE

#endif //SPRITE_H