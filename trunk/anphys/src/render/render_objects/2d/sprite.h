#ifndef SPRITE_H
#define SPRITE_H

#include <string>

#include "util/math/mmath.h"
#include "util/serialization/serialization.h"

struct grRender2DObjectMesh;
struct grTexture;
struct grRender;

struct grSprite
{
protected:

	vec2   mPosition;
	vec2   mSize;
	vec2   mScale;
	float  mAngle;
	vec2   mRotationCenter;
	fRect  mTextureSrcRect;

	grRender2DObjectMesh* mMesh;

	bool  mNeedUpdateMeshVerticies;
	bool  mNeedUpdateMeshTexCoords;

public:
	grSprite(grRender* render, grTexture* texture = NULL, const fRect& textureSrcRect = fRect(-1.0f, 0.0f, 0.0f, 0.0f), 
		     const vec2& position = vec2(0.0f, 0.0f), const vec2& size = vec2(-1.0f, 0.0f), 
			 const vec2& scale = vec2(1.0f, 1.0f), float angle = 0, const vec2& rotationCenter = vec2(0.0f, 0.0f),
			 const color4& color = color4(1.0f, 1.0f, 1.0f, 1.0f));

	grSprite(grRender* render, cDataObject& dataObject);
	grSprite(grRender* render, const std::string& file, const std::string& path);

	grSprite(const grSprite& sprite);
	~grSprite();

	grSprite*  clone() const;

	void       draw();

	grSprite&  setPosition(const vec2& position);
	vec2       getPosition() const;

	grSprite&  setScale(const vec2& scale);
	vec2       getScale() const;

	grSprite&  setAngle(float angle);
	float      getAngle() const;

	grSprite&  setRotationCenter(const vec2& center);
	vec2       getRotationCenter() const;

	grSprite&  setTextureSrcRect(const fRect& rect);
	fRect      getTextureSrcRect() const;

	grSprite&  setTexture(grTexture* texture);
	grTexture* getTexture() const;

	grSprite&  setSize(const vec2& size);
	vec2       getSize() const;

	grSprite&  setColor(const color4& color, int vertexId = -1);
	color4     getColor(int vertexId = 0);

	serializeMethodDecl(grSprite);

protected:
	void updateMeshVerticies();
	void updateMeshTexCoords();
};

#endif //SPRITE_H