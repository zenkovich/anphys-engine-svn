#ifndef ATLAS_INFO_H
#define ATLAS_INFO_H

#include "public.h"
#include "util/serialize_util.h"
#include "build_info.h"

OPEN_O2_NAMESPACE

/** Images atlas info. Contains infos of images. */
class cImageAtlasInfo: public cSerializable
{
	friend class cBuildInfo;

protected:
	string          mName;            /** Name of atlas. */
	cFileLocation   mAttachedPathLoc; /** Attached path location. */
	cBuildPathInfo* mAttachedPath;    /** Attached path pointer. */
	cBuildInfo*     mOwnerBuildInfo;  /** owner build onfo pointer. */

public:
	vec2f             mMaxSize;       /** Max atlas size. */
	BuildImageInfoVec mImages;        /** Images array. */

public:
	/** ctor. */
	cImageAtlasInfo(cBuildInfo* buildInfo = NULL);

	/** dtor. */
	~cImageAtlasInfo();

	/** Clearing atlas. All images will get NULL atlas. */
	void clear();

	/** Refreshing images array in attached path. */
	void refreshImages();

	/** Adding image to atlas. */
	void addImage(cBuildImageInfo* image);

	/** Removing image from atlas. */
	void removeImage(cBuildImageInfo* image);

	/** Returns image by location. */
	cBuildImageInfo* getImage(const cFileLocation& location);

	/** Sets atlas name. */
	void setName(const string& name);

	/** Returns atlas name. */
	string getName() const;

	/** Attaching atlas to path. */
	void attachPath(cBuildPathInfo* path);

	/** Deattaching atlas from path. */
	void unattachPath();
	
	SERIALIZBLE_METHODS(cImageAtlasInfo);

private:
	/** gathering all images inside attached path. */
	void searchImagesFromAttachedPath();

	/** Searching images recursively in path. */
	void searchPathImages(cBuildPathInfo* path);

	/** Search images what attached to this atlas. */
	void searchImagesForAtlas();
};

CLOSE_O2_NAMESPACE

#endif // ATLAS_INFO_H
