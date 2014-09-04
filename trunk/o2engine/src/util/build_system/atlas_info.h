#ifndef ATLAS_INFO_H
#define ATLAS_INFO_H

#include "public.h"
#include "util/serialize_util.h"
#include "build_system.h"

OPEN_O2_NAMESPACE

class cImageAtlasInfo: public cSerializable
{
public: 
	typedef vector<cBuildSystem::ImageFileMeta*> ImagesMetaVec;

protected:
	string        mName;
	cFileLocation mAttachedPath;

public:
	vec2f         mMaxSize;
	ImagesMetaVec mImages;

public:
	cImageAtlasInfo();
	~cImageAtlasInfo();

	void addImage(cBuildSystem::ImageFileMeta* meta);
	void removeImage(cBuildSystem::ImageFileMeta* meta);
	cBuildSystem::ImageFileMeta* getImage(const cFileLocation& location);

	void setName(const string& name);
	string getName() const;

	void attachPath(const cFileLocation& pathLocation);
	void unattachPath();

	SERIALIZBLE_METHODS(cImageAtlasInfo);
};

CLOSE_O2_NAMESPACE

#endif // ATLAS_INFO_H
