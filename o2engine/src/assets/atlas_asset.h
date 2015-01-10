#ifndef ATLAS_ASSET_H
#define ATLAS_ASSET_H

#include "asset.h"
#include "image_asset.h"

OPEN_O2_NAMESPACE
	
class asAtlas: public asAsset, public Serializable
{
	DEFINE_TYPE(asAtlas);

public:
	struct ImageDef: public Serializable
	{
		FileLocation mLocation;
		bool         mLinkedDirectly;

		ImageDef(const FileLocation& location = FileLocation(), bool linkedDirectly = true);

		SERIALIZBLE_METHODS(ImageDef);

		bool operator=(const ImageDef& other);
	};
	typedef array<ImageDef> ImagesDefsArr;

protected:
	string        mAtlasName;
	vec2f         mMaxSize;
	bool          mAttachedToFolder;
	FileLocation  mAttachFolder;
	ImagesDefsArr mImages;

public:
	asAtlas();
	asAtlas(const string& location);
	asAtlas(const FileLocation& location);
	~asAtlas();
	
	void addImage(const FileLocation& imageLocation);
	bool removeImage(const FileLocation& imageLocation);
	ImagesDefsArr getImagesArray() const;

	void setMaxSize(const vec2f& maxSize);
	vec2f getMaxSize() const;

	void setName(const string& name);
	string getName() const;

	void attachToFolder(const FileLocation& folderLocation);
	FileLocation getAttachedFolder() const;
	bool isAttachedToFolder() const;
	void deattachFolder();

	SERIALIZBLE_METHODS(asAtlas);

protected:
	void loadData();
	void saveData();
	
	asAtlas& operator=(const asAtlas& asset);
};

CLOSE_O2_NAMESPACE

#endif // ATLAS_ASSET_H
