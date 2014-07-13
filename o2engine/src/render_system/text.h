#ifndef TEXT_H
#define TEXT_H

#include "public.h"

#include "util/graphics/rect_drawable.h"
#include "util/property.h"
#include "util/string.h"
#include "font.h"

OPEN_O2_NAMESPACE

class grMesh;
class grRenderSystem;

/** Text renderer class. Using font, basis and many style parameters. Caching text into meshes. */
class grText: public IRectDrawable
{
public: 
	enum { nMeshMaxPolyCount = 4096 };

protected:
	typedef vector< grMesh* > MeshVec;

	wstring          mText;               /** Wide char string, containing rendering text. */
	grFont*          mFont;               /** Using font. */
	basis            mTransform;          /** Transformation. */
	basisDef         mTransformDef;       /** Transform definition. */
	float            mCharactersDistCoef; /** Characters distance coef, 1 is standard. */
	float            mLinesDistCoef;      /** Lines distance coef, 1 is standard. */
	grFont::VerAlign mVerAlign;           /** Vertical align. */
	grFont::HorAlign mHorAlign;           /** Horizontal align. */
	bool             mWordWrap;           /** True, when words wrapping. */
			         
	MeshVec          mMeshes;             /** Meshes vector. */
	basis            mLastTransform;      /** Last mesh update transformation. */
	bool             mNeedUpdateMesh;     /** True, when need rebuild meshes. */
	bool             mNeedTransformMesh;  /** True, when need transform meshes. */
	
	grFont::TextSymbolsSet mSymbolsSet;   /** Symbols set definition. */

public:
	//properties
	PROPERTY(grText, grFont*)          font;                /** Font pointer property. Uses set/getFont. */
	PROPERTY(grText, wstring)          text;                /** Text property, wstring. Uses set/getText. */
	PROPERTY(grText, string)           ctext;               /** Text property, string. Uses set/getCText. */
	PROPERTY(grText, grFont::VerAlign) verAlign;            /** vertical align property. Uses set/getVerAlign. */
	PROPERTY(grText, grFont::HorAlign) horAlign;            /** Horizontal align property. Uses set/getHorAlign. */
	PROPERTY(grText, bool)             wordWrap;            /** Words wrapping flag property. Uses set/getWordWrap. */
	PROPERTY(grText, float)            angle;               /** Angle of rotation property. Uses set/getAngle. */
	PROPERTY(grText, vec2f)            scale;               /** Scale property. Uses set/getScale. */
	PROPERTY(grText, float)            charactersHeight;    /** Characters height property, pixels. Uses set/getCharactersHeight. */
	PROPERTY(grText, basis)            transform;           /** Transformation property. Uses set/getTransform. */
	PROPERTY(grText, basisDef)         transformDef;        /** Transform definition property. Uses set/getTransformDef. */
	PROPERTY(grText, float)            charactersDistCoef;  /** Characters distance coef, 1 is standard. Uses set/getCharactersDistCoef. */
	PROPERTY(grText, float)            linesDistCoef;       /** Lines distance coef, 1 is standard. Uses set/getLinesDistCoef. */
	
	/** ctor. */
	grText(grFont* font);

	/** ctor. */
	grText(const grText& text);

	/** dtor. */
	~grText();

	/** Returns copy of drawable. */
	IRectDrawable* clone() const;

	/** Draw text. */
	void draw();	

	/** Sets using font. */
	void setFont(grFont* font);

	/** Returns using font. */
	grFont* getFont() const;

	/** Sets text. */
	void setText(const wstring& text);

	/** Returns text. */
	wstring getText() const;

	/** Sets text. */
	void setCText(const string& text);

	/** Returns text as string. */
	string getCText() const;

	/** Sets rotation angle. */
	void setAngle(const float& angle);

	/** Returns rotation angle. */
	float getAngle() const;

	/** Sets text scale. */
	void setScale(const vec2f& scale);

	/** Returns text scale. */
	vec2f getScale() const;

	/** Sets characters height in pixels. */
	void setCharactersHeight(const float& height);

	/** Returns characters height in pixels. */
	float getCharactersHeight() const;

	/** Sets transformation of text. */
	void setTransform(const basis& bas);

	/** Returns text transformation. */
	basis getTransform() const;

	/** Sets text transformation definition. */
	void setTransformDef(const basisDef& def);

	/** Returns text transformation definition. */
	basisDef getTransformDef() const;

	/** Sets horizontal align. */
	void setHorAlign(const grFont::HorAlign& align);

	/** Returns horizontal align. */
	grFont::HorAlign getHorAlign() const;

	/** Sets vertical align. */
	void setVerAlign(const grFont::VerAlign& align);

	/** returns vertical align. */
	grFont::VerAlign getVerAlign() const;

	/** Sets word wrapping. */
	void setWordWrap(const bool& flag);

	/** Returns word wrapping. */
	bool getWordWrap() const;

	/** Sets characters distance coefficient. */
	void setCharactersDistCoef(const float& coef = 1);

	/** Returns characters distance coef. */
	float getCharactersDistCoef() const;

	/** Sets lines distance coefficient. */
	void setLinesDistCoef(const float& coef = 1);

	/** Returns lines distance coefficient. */
	float getLinesDistCoef() const;

	/** Returns symbol set structure pointer. */
	grFont::TextSymbolsSet* getSymbolsSet();

	/** Updates mesh immediately, don't wait drawing. */
	void forceUpdateMesh();

protected:	
	/** Calls when position was changed. */
	void positionChanged();

	/** Calls when size was changed. */
	void sizeChanged();

	/** Calls when pivot was changed. */
	void pivotChanged();

	/** Calls when color was changed. */
	void colorChanged();

	/** Initializing properties. */
	void initializeProperties();

	/** Updating meshes. */
	void updateMesh();

	/** Transforming meshes by basis. */
	void transformMesh(const basis& bas);

	/** Preparing meshes for characters count. */
	void prepareMesh(int charactersCount);
};

CLOSE_O2_NAMESPACE

#endif // TEXT_H
