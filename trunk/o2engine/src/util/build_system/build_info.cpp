#include "build_info.h"
#include "atlas_info.h"

OPEN_O2_NAMESPACE
	

std::map<string, cSerializable*> gSerializeTypesContainer::mSamples;

SERIALIZE_METHOD_IMPL(cBuildFileInfo)
{
	SERIALIZE_ID(&mLocation, "location");
	SERIALIZE_ID(mBuildIncluded, "buildIncluded");
	SERIALIZE_ID(mSize, "size");
	SERIALIZE_ID(mWritedTime, "writedTime");

	int itype = (int)mType;
	SERIALIZE_ID(itype, "type");
	mType = (Type)itype;

	return true;
}

cBuildFileInfo* cBuildFileInfo::clone() const
{
	return mnew cBuildFileInfo(*this);
}

bool cBuildFileInfo::operator==(const cBuildFileInfo& v) const
{
	return mLocation == v.mLocation && mLocation.mPath == v.mLocation.mPath && 
		   mSize == v.mSize && mWritedTime == v.mWritedTime;
}

bool cBuildFileInfo::operator!=(const cBuildFileInfo& v) const
{
	return !(*this == v);
}


SERIALIZE_INHERITED_METHOD_IMPL(cBuildImageInfo)
{
	SERIALIZE_ID(mAtlasName, "atlas");

	if (serializer->getType() == cSerializer::ST_DESERIALIZE || mSourceRect != fRect())
		SERIALIZE_ID(mSourceRect, "sourceRect");
	if (serializer->getType() == cSerializer::ST_DESERIALIZE || mAtlasRect != fRect())
		SERIALIZE_ID(mAtlasRect, "atlasRect");
	if (serializer->getType() == cSerializer::ST_DESERIALIZE || mAtlasPage > -1)
		SERIALIZE_ID(mAtlasPage, "atlasPage");

	return true;
}

cBuildFileInfo* cBuildImageInfo::clone() const
{
	return mnew cBuildImageInfo(*this);
}

void cBuildImageInfo::setAtlas(cImageAtlasInfo* atlas)
{
	if (atlas)
		atlas->addImage(this);
	else
	{
		if (mAtlas)
			mAtlas->removeImage(this);

		mAtlas = NULL;
		mAtlasName = "";
	}
}

cImageAtlasInfo* cBuildImageInfo::getAtlas() const
{
	return mAtlas;
}

cBuildImageInfo::cBuildImageInfo():mAtlas(NULL), mAtlasPage(-1)
{
}

cBuildImageInfo::cBuildImageInfo(const cBuildImageInfo& info):
	cBuildFileInfo(info), mAtlas(NULL)
{
}

cBuildImageInfo::~cBuildImageInfo()
{
	setAtlas(NULL);
}


SERIALIZE_INHERITED_METHOD_IMPL(cBuildPathInfo)
{
	SERIALIZE_ID(mAttachedAtlasName, "attachedAtlas");

	return true;
}

cBuildFileInfo* cBuildPathInfo::clone() const
{
	return mnew cBuildPathInfo(*this);
}

BuildFileInfoArr cBuildPathInfo::getAllInsideFiles() const
{
	BuildFileInfoArr res;
	foreach_const(BuildFileInfoArr, mFiles, file) 
	{
		res.add(*file);

		if ((*file)->mType == cBuildFileInfo::MT_FOLDER)
		{
			cBuildPathInfo* path = static_cast<cBuildPathInfo*>(*file);
			BuildFileInfoArr pathFiles = path->getAllInsideFiles();
			FOREACH(BuildFileInfoArr, pathFiles, pathFile)
				res.add(*pathFile);
		}
	}

	return res;
}

void cBuildPathInfo::attachAtlas(cImageAtlasInfo* atlas)
{
	if (atlas)
		atlas->attachPath(this);
	else
	{
		mAttachedAtlas = NULL;
		mAttachedAtlasName = "";
	}
}

cImageAtlasInfo* cBuildPathInfo::getAttachedAtlas() const
{
	return mAttachedAtlas;
}

cBuildPathInfo::cBuildPathInfo():mAttachedAtlas(NULL)
{
}

cBuildPathInfo::cBuildPathInfo(const cBuildPathInfo& info):
	cBuildFileInfo(info), mAttachedAtlas(NULL)
{
}

void cBuildPathInfo::updateInsideFiles(BuildFileInfoArr& files)
{
	mFiles.clear();

	int locLength = mLocation.mPath.length();
	foreach(BuildFileInfoArr, files, file)
	{
		if (*file == this)
			continue;

		if (!isPathInsideOtherPath((*file)->mLocation.mPath, mLocation.mPath, true))
			continue;

		/*int pathSearchIdx = (*file)->mLocation.mPath.find(mLocation.mPath);
		if (pathSearchIdx != 0)
			continue;

		int delRightIdx = (*file)->mLocation.mPath.rfind("/");
		if (delRightIdx != string::npos && delRightIdx > locLength)
			continue;*/

		mFiles.add(*file);
	}
}

cBuildPathInfo::~cBuildPathInfo()
{
	attachAtlas(NULL);
}


cBuildInfo::cBuildInfo()
{
	mBasicAtlas = mnew cImageAtlasInfo(this);
	mBasicAtlas->setName("basic");
	mBasicAtlas->mMaxSize = vec2f(2048, 2048);

	mRootPath = mnew cBuildPathInfo();
	mRootPath->attachAtlas(mBasicAtlas);
}

cBuildInfo::~cBuildInfo()
{
	RELEASE_VECTOR(BuildFileInfoArr, mFileInfos);
	RELEASE_VECTOR(AtlasesArr, mAtlases);
	safe_release(mBasicAtlas);
	safe_release(mRootPath);
}

void cBuildInfo::addFile(cBuildFileInfo* info)
{
	mFileInfos.add(info);

	foreach(BuildFileInfoArr, mFileInfos, fileIt)
	{
		if ((*fileIt)->mType == cBuildFileInfo::MT_FOLDER)
		{
			cBuildPathInfo* path = static_cast<cBuildPathInfo*>(*fileIt);
			if (isPathInsideOtherPath(info->mLocation.mPath, path->mLocation.mPath, true))
				path->mFiles.add(info);
		}
	}

	if (isPathInsideOtherPath(info->mLocation.mPath, mRootPath->mLocation.mPath, true))
		mRootPath->mFiles.add(info);
}

void cBuildInfo::removeFile(cBuildFileInfo* info)
{
	foreach(BuildFileInfoArr, mFileInfos, metaIt)
	{
		if (info->mLocation == (*metaIt)->mLocation)
		{
			mFileInfos.remove(metaIt);
			safe_release(info);
			return;
		}
	}
}

cBuildFileInfo* cBuildInfo::getFile(uint32 id) const
{
	foreach_const(BuildFileInfoArr, mFileInfos, metaIt)
	{
		if (id == (*metaIt)->mLocation.mId)
			return *metaIt;
	}

	return NULL;
}

cBuildFileInfo* cBuildInfo::getFile(const cFileLocation& location) const
{
	if (location == cFileLocation())
		return mRootPath;

	foreach_const(BuildFileInfoArr, mFileInfos, metaIt)
	{
		if (location == (*metaIt)->mLocation)
			return *metaIt;
	}

	return NULL;
}

SERIALIZE_METHOD_IMPL(cBuildInfo)
{
	SERIALIZE_ID(mFileInfos, "filesMeta");
	SERIALIZE_ID(mAtlases, "atlases");
	SERIALIZE_ID(mBasicAtlas, "basicAtlas");

	return true;
}

void cBuildInfo::onDeserialized()
{
	//restore image<->atlas links
	foreach(BuildFileInfoArr, mFileInfos, file)
	{
		if ((*file)->mType == cBuildFileInfo::MT_IMAGE)
		{
			cBuildImageInfo* image = static_cast<cBuildImageInfo*>(*file);
			image->setAtlas(getAtlas(image->mAtlasName));
		}
	}

	//get folders files
	foreach(BuildFileInfoArr, mFileInfos, file)
	{
		if ((*file)->mType == cBuildFileInfo::MT_FOLDER)
		{
			cBuildPathInfo* path = static_cast<cBuildPathInfo*>(*file);
			path->updateInsideFiles(mFileInfos);			
		}
	}

	updateRootPathFiles();

	//restore path<->atlas links
	foreach(BuildFileInfoArr, mFileInfos, file)
	{
		if ((*file)->mType == cBuildFileInfo::MT_FOLDER)
		{
			cBuildPathInfo* path = static_cast<cBuildPathInfo*>(*file);
			path->attachAtlas(getAtlas(path->mAttachedAtlasName));
		}
	}

	//attach atlases owner to this
	foreach(AtlasesArr, mAtlases, atlasIt)
		(*atlasIt)->mOwnerBuildInfo = this;

	mBasicAtlas->mOwnerBuildInfo = this;
}

cImageAtlasInfo* cBuildInfo::addAtlas( const string& name, const vec2f& maxSize, cBuildPathInfo* attachingPath /*= NULL*/ )
{
	cImageAtlasInfo* newAtlas = mnew cImageAtlasInfo(this);
	newAtlas->setName(name);
	newAtlas->mMaxSize = maxSize;
	mAtlases.add(newAtlas);
	newAtlas->attachPath(attachingPath);

	return newAtlas;
}

cImageAtlasInfo* cBuildInfo::getAtlas( const string& name ) const
{
	foreach_const(AtlasesArr, mAtlases, atl)
		if ((*atl)->getName() == name)
			return *atl;

	if (name == mBasicAtlas->getName())
		return mBasicAtlas;

	return NULL;
}

void cBuildInfo::removeAtlas( const string& name )
{
	foreach(AtlasesArr, mAtlases, atl) 
	{
		if ((*atl)->getName() == name)
		{
			//copy images metas
			foreach(BuildImageInfoArr, (*atl)->mImages, img)
				mBasicAtlas->addImage(*img);
			(*atl)->mImages.clear();

			safe_release(*atl);
			mAtlases.remove(atl);
			return;
		}
	}		
}

void cBuildInfo::refreshAtlases()
{
	foreach(AtlasesArr, mAtlases, atl)
		(*atl)->refreshImagesList();

	mBasicAtlas->refreshImagesList();
}

void cBuildInfo::updateRootPathFiles()
{
	mRootPath->updateInsideFiles(mFileInfos);
}



CLOSE_O2_NAMESPACE