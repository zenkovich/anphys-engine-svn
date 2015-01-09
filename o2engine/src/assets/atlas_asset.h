#ifndef ATLAS_ASSET_H
#define ATLAS_ASSET_H

#include "asset.h"
#include "image_asset.h"

OPEN_O2_NAMESPACE
	
class asAtlas: public asAsset, public cSerializable
{
	DEFINE_TYPE(asAtlas);

protected:
	string           mAtlasName;
	vec2f            mMaxSize;
	bool             mAttachedToFolder;
	FileLocation     mAttachFolder;
	FileLocationsArr mImages;

public:
	asAtlas();
	asAtlas(const string& location);
	asAtlas(const FileLocation& location);
	~asAtlas();

	asAtlas& operator=(const asAtlas& asset);

	void addImage(const FileLocation& imageLocation);
	void removeImage(const FileLocation& imageLocation);
	FileLocationsArr getImagesArray() const;

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
};

CLOSE_O2_NAMESPACE

#endif // ATLAS_ASSET_H
