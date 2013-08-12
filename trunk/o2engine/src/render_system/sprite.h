#ifndef SPRITE_H
#define SPRITE_H

#include "public.h"
#include "util/math/vector2.h"
#include "util/math/rect.h"
#include "util/math/color.h"
#include "util/serialization/serialization.h"

OPEN_O2_NAMESPACE

class grMesh;
class grRenderSystem;
class grTexture;
	
class grSprite
{
	vec2f   mPosition;
	vec2f   mSize;
	vec2f   mScale;
	float   mAngle;
	vec2f   mRotationCenter;
	fRect   mTextureSrcRect;

	grMesh* mMesh;

	bool    mNeedUpdateMeshVerticies;
	bool    mNeedUpdateMeshTexCoords;

public:
	grSprite(grRenderSystem* render, grTexture* texture = NULL, const fRect& textureSrcRect = fRect(-1.0f, 0.0f, 0.0f, 0.0f), 
		     const vec2f& position = vec2f(0.0f, 0.0f), const vec2f& size = vec2f(-1.0f, 0.0f), 
			 const vec2f& scale = vec2f(1.0f, 1.0f), float angle = 0, const vec2f& rotationCenter = vec2f(0.0f, 0.0f),
			 const color4& color = color4(1.0f, 1.0f, 1.0f, 1.0f));

	//grSprite(grRenderSystem* render, cDataObject& dataObject);

	grSprite(grRenderSystem* render, const std::string& file, const std::string& path);

	grSprite(const grSprite& sprite);
	~grSprite();

	grSprite* clone() const;

	void draw();

	grSprite& setPosition(const vec2f& position);
	vec2f getPosition() const;

	grSprite& setScale(const vec2f& scale);
	vec2f getScale() const;

	grSprite& setAngle(float angle);
	float getAngle() const;

	grSprite& setRotationCenter(const vec2f& center);
	vec2f getRotationCenter() const;

	grSprite& setTextureSrcRect(const fRect& rect);
	fRect getTextureSrcRect() const;

	grSprite& setTexture(grTexture* texture);
	grTexture* getTexture() const;

	grSprite& setSize(const vec2f& size);
	vec2f getSize() const;

	grSprite& setColor(const color4& color, int vertexId = -1);
	color4 getColor(int vertexId = 0);

	SERIALIZE_METHOD_DECL();

protected:
	void updateMeshVerticies();
	void updateMeshTexCoords();
};

CLOSE_O2_NAMESPACE

#endif //SPRITE_H