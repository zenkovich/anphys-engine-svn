#ifndef STRETCH_RECT_H
#define STRETCH_RECT_H

#include "public.h"

#include "render_system/texture.h"
#include "util/serialization/serialization.h"
#include "util/property.h"

OPEN_O2_NAMESPACE

class grMesh;
class grRenderSystem;

class cStretchRect:public cSerializableObj
{
	struct Part
	{
		vec2f  mLTPosPercent;
		vec2f  mLTPosPixel;
		vec2f  mRBPosPercent;
		vec2f  mRBPosPixel;
		fRect  mTextureSrcRect;
		bool   mWrapTexture;
		color4 mVertexColors[4];

		Part();
		Part(const vec2f& LTPercent, const vec2f& LTPixel, const vec2f& RBPercent, const vec2f& RBPixel, 
			 const fRect& texRect, bool wrapTexture = false,
			 const color4& vertex0Color = color4::white(), const color4& vertex1Color = color4::white(), 
			 const color4& vertex2Color = color4::white(), const color4& vertex3Color = color4::white());
	};
	typedef vector<Part> PartsVec;

	shared<grMesh> mMesh;
	PartsVec       mParts;
	fRect          mRect;
	bool           mNeedUpdateMesh;
	bool           mNeedUpdateTransparency;
	float          mTransparency;

public:
	PROPERTY(cStretchRect, fRect) rect;
	PROPERTY(cStretchRect, vec2f) position;
	PROPERTY(cStretchRect, vec2f) size;
	PROPERTY(cStretchRect, float) transparency;

	cStretchRect(int parts = 0, const grTexture& texture = grTexture());
	cStretchRect(const grTexture& texture, int left, int top, int right, int bottom, const fRect& texRect = fRect(), 
		         const color4& color = color4::white());
	cStretchRect(const cStretchRect& stretchRect);

	cStretchRect& operator=(const cStretchRect& stretchRect);

	int addPart(const vec2f& LTPercent, const vec2f& LTPixel, const vec2f& RBPercent, const vec2f& RBPixel, 
		        const fRect& texRect, bool wrapTexture = false,
			    const color4& vertex0Color = color4::white(), const color4& vertex1Color = color4::white(), 
			    const color4& vertex2Color = color4::white(), const color4& vertex3Color = color4::white());

	void removePart(int idx);

	void setRect(const fRect& rect);
	fRect getRect() const;

	void setPosition(const vec2f& pos);
	vec2f getPosition() const;

	void setSize(const vec2f& size);
	vec2f getSize() const;

	void setTransparency(float transparency);
	float getTransparency() const;

	void draw(bool debug = false);

	SERIALIZE_METHOD_DECL();

protected:
	void createMesh(int partsCount, const grTexture& texture);
	void updateMesh();
	void updateTransparency();
	void initializeProperties();
};

CLOSE_O2_NAMESPACE

#endif //STRETCH_RECT_H