#ifndef STRETCH_RECT_H
#define STRETCH_RECT_H

#include "public.h"

#include "render_system/texture.h"
#include "util/objects.h"
#include "util/math/color.h"
#include "util/math/rect.h"
#include "util/math/vector2.h"
#include "util/serialization/serialization.h"
#include "util/smart_ptrs.h"

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
		color4 mVertexColors[4];

		Part();
		Part(const vec2f& LTPercent, const vec2f& LTPixel, const vec2f& RBPercent, const vec2f& RBPixel, const fRect& texRect,
			 const color4& vertex0Color = color4::white(), const color4& vertex1Color = color4::white(), 
			 const color4& vertex2Color = color4::white(), const color4& vertex3Color = color4::white());
	};
	typedef vector<Part> PartsVec;

	sharedPtr(grMesh) mMesh;
	PartsVec    mParts;
	fRect       mRect;
	bool        mNeedUpdateMesh;

public:
	cStretchRect(int parts = 0, const grTexture& texture = grTexture());
	cStretchRect(const cStretchRect& stretchRect);

	cStretchRect& operator=(const cStretchRect& stretchRect);

	int addPart(const vec2f& LTPercent, const vec2f& LTPixel, const vec2f& RBPercent, const vec2f& RBPixel, const fRect& texRect,
			    const color4& vertex0Color = color4::white(), const color4& vertex1Color = color4::white(), 
			    const color4& vertex2Color = color4::white(), const color4& vertex3Color = color4::white());

	void removePart(int idx);

	void setRect(const fRect& rect);
	const fRect& getRect() const;

	void setPosition(const vec2f& pos);
	vec2f getPosition() const;

	void setSize(const vec2f& size);
	vec2f getSize() const;

	void draw();

	SERIALIZE_METHOD_DECL();

protected:
	void createMesh(int partsCount, const grTexture& texture);
	void updateMesh();
};

CLOSE_O2_NAMESPACE

#endif //STRETCH_RECT_H