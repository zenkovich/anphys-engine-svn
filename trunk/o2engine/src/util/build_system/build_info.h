#ifndef BUILD_INFO_H
#define BUILD_INFO_H

#include "public.h"
#include "util/serialize_util.h"
#include "util/file_system/file_system.h"

OPEN_O2_NAMESPACE

class cImageAtlasInfo;

/** Build info file. Contains location, type and some data about when it wrote. */
struct cBuildFileInfo: public cSerializable
{
	enum Type { MT_FOLDER = 0, MT_FILE, MT_IMAGE };

	cFileLocation mLocation;      /** FIle location. */
	Type          mType;          /** File type. */
	bool          mBuildIncluded; /** True, if it file included into build. */
	uint32        mSize;          /** Size of file in bytes. */
	WideTime      mWritedTime;    /** Last wrote time. */

	virtual ~cBuildFileInfo() {}

	bool operator==(const cBuildFileInfo& v) const;
	bool operator!=(const cBuildFileInfo& v) const;
	
	virtual cBuildFileInfo* clone() const;
	SERIALIZBLE_METHODS(cBuildFileInfo);
};
typedef array<cBuildFileInfo*> BuildFileInfoArr;

/** Build image file info. Inherites from cBuildFileInfo and contains atlas, where that image positioning. */
struct cBuildImageInfo: public cBuildFileInfo
{
	friend class cImageAtlasInfo;
	friend class cBuildInfo;

protected:
	string           mAtlasName; /** Atlas name. */
	cImageAtlasInfo* mAtlas;     /** Atlas pointer. */

public:
	vec2f mSize;       /** Size of asset image. */
	fRect mSourceRect; /** Source rect in PSD file. */
	fRect mAtlasRect;  /** Atlas rect of image. */
	int   mAtlasPage;  /** Atlas page. Negative - no atlas. */

public:
	cBuildImageInfo();
	cBuildImageInfo(const cBuildImageInfo& info);
	~cBuildImageInfo();

	/** Sets atlas to image. */
	void setAtlas(cImageAtlasInfo* atlas);

	/** Returns atlas pointer. */
	cImageAtlasInfo* getAtlas() const;

	SERIALIZBLE_INHERITED_METHODS(cBuildImageInfo, cBuildFileInfo);
	virtual cBuildFileInfo* clone() const;
};
typedef array<cBuildImageInfo*> BuildImageInfoArr;

/** Build path info. Contains link to attached atlas and filed inside that path. */
struct cBuildPathInfo: public cBuildFileInfo
{
	friend class cImageAtlasInfo;
	friend class cBuildInfo;

protected:
	string           mAttachedAtlasName; /** Attached atlas name. */
	cImageAtlasInfo* mAttachedAtlas;     /** Attached atlas pointer. */

public:
	BuildFileInfoArr mFiles;             /** Files inside path. */

	cBuildPathInfo();
	cBuildPathInfo(const cBuildPathInfo& info);
	~cBuildPathInfo();

	/** Attaching to atlas. NULL - no attached atlas. */
	void attachAtlas(cImageAtlasInfo* atlas);

	/** Returns attached atlas. */
	cImageAtlasInfo* getAttachedAtlas() const;

	/** Returns all inside files and paths. */
	BuildFileInfoArr getAllInsideFiles() const;

	/** Updates inside files array from files. */
	void updateInsideFiles(BuildFileInfoArr& files);

	SERIALIZBLE_INHERITED_METHODS(cBuildPathInfo, cBuildFileInfo);
	virtual cBuildFileInfo* clone() const;
};

/** Build info data. Contains all files and atlases information. */
class cBuildInfo: public cSerializable
{
	friend class cBuildSystem;
	friend class cImageAtlasInfo;
	friend class cNonBuildFilesBuildStage;
	friend class cAtlasesBuildingStage;

public:
	typedef array<cImageAtlasInfo*> AtlasesArr;

private:
	BuildFileInfoArr mFileInfos;  /** Files infos. */
	AtlasesArr       mAtlases;    /** Atlases. */
	cImageAtlasInfo* mBasicAtlas; /** Basic atlas, attached to root path. */
	cBuildPathInfo*  mRootPath;   /** Root path info. */

public:
	/** ctor. */
	cBuildInfo();

	/** dtor. */
	virtual ~cBuildInfo();
	
	/** Adding file. */
	void addFile(cBuildFileInfo* info);

	/** Removing file. */
	void removeFile(cBuildFileInfo* info);

	/** Returns file by id. */
	cBuildFileInfo* getFile(uint32 id) const;

	/** Returns file by location. */
	cBuildFileInfo* getFile(const cFileLocation& location) const;

	/** Adding atlas. */
	cImageAtlasInfo* addAtlas(const string& name, const vec2f& maxSize, cBuildPathInfo* attachingPath = NULL);

	/** Returns atlas by name. */
	cImageAtlasInfo* getAtlas(const string& name) const;

	/** Removes atlas. */
	void removeAtlas(const string& name);

	/** Refreshing images lists in atlases. */
	void refreshAtlases();

	SERIALIZBLE_METHODS(cBuildInfo);

protected:
	virtual void onDeserialized();

	/** Updates root path files. */
	void updateRootPathFiles();
};

CLOSE_O2_NAMESPACE

#endif // BUILD_INFO_H
