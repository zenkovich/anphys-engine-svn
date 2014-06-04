#ifndef STRETCH_RECT_H
#define STRETCH_RECT_H

#include "public.h"

#include "render_system/texture.h"
#include "util/serialization/serialization.h"
#include "util/property.h"
#include "util/graphics/rect_drawable.h"

OPEN_O2_NAMESPACE

class grMesh;
class grRenderSystem;

/** Stretching rect drawable. Contains some parts, what building using position and size of drawable. */
class cStretchRect:public IRectDrawable
{
	/** Drawable part. Contains texture source rect, each corners colors, texture wrapping and position params. */
	struct Part
	{
		vec2f  mLTPosPercent;    /** Left top corner relative position component. */
		vec2f  mLTPosPixel;      /** Left top corner pixel position component. */
		vec2f  mRBPosPercent;    /** Right down corner relative position component. */
		vec2f  mRBPosPixel;      /** Right down corner pixel position component. */
		fRect  mTextureSrcRect;  /** Texture source rect. */
		bool   mWrapTexture;     /** True, if texture will be repeat and keep source rect size. */
		color4 mVertexColors[4]; /** Corners colors. */
		bool   mClampLeft;       /** True, if need clamp left side by rect. */
		bool   mClampRight;      /** True, if need clamp right side by rect. */
		bool   mClampTop;        /** True, if need clamp top side by rect. */
		bool   mClampBottom;     /** True, if need clamp bottom side by rect. */

		/** def ctor. */
		Part();

		/** ctor. */
		Part(const vec2f& LTPercent, const vec2f& LTPixel, const vec2f& RBPercent, const vec2f& RBPixel, 
			 const fRect& texRect, bool wrapTexture = false,
			 const color4& vertex0Color = color4::white(), const color4& vertex1Color = color4::white(), 
			 const color4& vertex2Color = color4::white(), const color4& vertex3Color = color4::white());
	};
	typedef vector<Part> PartsVec;

	shared<grMesh> mMesh;             /** Mesh. */
	PartsVec       mParts;            /** Parts array. */
	vec2f          mMinSize;          /** Minimal size, when part will bot be clamped. */
	bool           mNeedUpdateMesh;   /** True, if need to update mesh verticies. */
	bool           mNeedUpdateColors; /** True, if need update just verticies colors. */

public:

	/** ctor. */
	cStretchRect(int parts = 0, const grTexture& texture = grTexture());

	/** ctor, creating standart 9-slice stretching rect. */
	cStretchRect(const grTexture& texture, int left, int top, int right, int bottom, const fRect& texRect = fRect(), 
		         const color4& color = color4::white());

	/** copy-ctor. */
	cStretchRect(const cStretchRect& stretchRect);

	/** copy operator. */
	cStretchRect& operator=(const cStretchRect& stretchRect);

	/** Returns a copy of drawable. */
	shared<IRectDrawable> clone() const;

	/** Adding a part. Automatically resizing mesh. */
	int addPart(const vec2f& LTPercent, const vec2f& LTPixel, const vec2f& RBPercent, const vec2f& RBPixel, 
		        const fRect& texRect, bool wrapTexture = false,
			    const color4& vertex0Color = color4::white(), const color4& vertex1Color = color4::white(), 
			    const color4& vertex2Color = color4::white(), const color4& vertex3Color = color4::white());

	/** Removes part. */
	void removePart(int idx);

	/** Drawing. */
	void draw();

	/** Drawing debug frames for each parts. */
	void drawDebug();

	/** Sets the kinimal size. */
	void setMinSize(const vec2f& minSize);

	/** Returns minimal size. */
	vec2f getMinSize() const;

	SERIALIZE_METHOD_DECL();

protected:	
	/** Calls when positon changed. */
	void positionChanged();

	/** Calls when size changed. */
	void sizeChanged();

	/** Calls when pivot changed. */
	void pivotChanged();

	/** Calls when color changed. */
	void colorChanged();

	/** Creates mesh for some parts count. */
	void createMesh(int partsCount, const grTexture& texture);

	/** Initialize mesh polygons for parts. */
	void initMeshPolygons(int startIdx = 0);

	/** Updating mesh. */
	void updateMesh();

	/** Updating mesh colors. */
	void updateColors();
};

CLOSE_O2_NAMESPACE

#endif //STRETCH_RECT_H