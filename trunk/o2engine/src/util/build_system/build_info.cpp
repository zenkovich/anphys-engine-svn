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
	return mLocation == v.mLocation && mSize == v.mSize && mWritedTime == v.mWritedTime;
}

bool cBuildFileInfo::operator!=(const cBuildFileInfo& v) const
{
	return !(*this == v);
}

SERIALIZE_INHERITED_METHOD_IMPL(cBuildImageInfo)
{
	SERIALIZE_ID(mAtlasName, "atlas");
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
		mAtlas = NULL;
		mAtlasName = "";
	}
}

cImageAtlasInfo* cBuildImageInfo::getAtlas() const
{
	return mAtlas;
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

BuildFileInfoVec cBuildPathInfo::getAllInsideFiles() const
{
	BuildFileInfoVec res;
	FOREACH_CONST(BuildFileInfoVec, mFiles, file) 
	{
		res.push_back(*file);

		if ((*file)->mType == cBuildFileInfo::MT_FOLDER)
		{
			cBuildPathInfo* path = static_cast<cBuildPathInfo*>(*file);
			BuildFileInfoVec pathFiles = path->getAllInsideFiles();
			FOREACH(BuildFileInfoVec, pathFiles, pathFile)
				res.push_back(*pathFile);
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


cBuildInfo::cBuildInfo()
{
	mBasicAtlas = mnew cImageAtlasInfo();
}

cBuildInfo::~cBuildInfo()
{
	RELEASE_VECTOR(BuildFileInfoVec, mFileInfos);
	RELEASE_VECTOR(AtlasesVec, mAtlases);
	safe_release(mBasicAtlas);
}

void cBuildInfo::addFile(cBuildFileInfo* info)
{
	mFileInfos.push_back(info);
}

void cBuildInfo::removeFile(cBuildFileInfo* info)
{
	FOREACH(BuildFileInfoVec, mFileInfos, metaIt)
	{
		if (info->mLocation == (*metaIt)->mLocation)
		{
			mFileInfos.erase(metaIt);
			safe_release(info);
			return;
		}
	}
}

cBuildFileInfo* cBuildInfo::getFile(uint32 id) const
{
	FOREACH_CONST(BuildFileInfoVec, mFileInfos, metaIt)
	{
		if (id == (*metaIt)->mLocation.mId)
			return *metaIt;
	}

	return NULL;
}

cBuildFileInfo* cBuildInfo::getFile(const cFileLocation& location) const
{
	FOREACH_CONST(BuildFileInfoVec, mFileInfos, metaIt)
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
	FOREACH(BuildFileInfoVec, mFileInfos, file)
	{
		if ((*file)->mType == cBuildFileInfo::MT_IMAGE)
		{
			cBuildImageInfo* image = static_cast<cBuildImageInfo*>(*file);
			image->setAtlas(getAtlas(image->mAtlasName));
		}
	}

	//get folders files
	FOREACH(BuildFileInfoVec, mFileInfos, file)
	{
		if ((*file)->mType == cBuildFileInfo::MT_FOLDER)
		{
			cBuildPathInfo* path = static_cast<cBuildPathInfo*>(*file);
			
			FOREACH(BuildFileInfoVec, mFileInfos, file2)
			{
				if (*file2 == *file)
					continue;

				int pathSearchIdx = (*file2)->mLocation.mPath.find(path->mLocation.mPath);
				if (pathSearchIdx == string::npos)
					continue;

				int delRightIdx = (*file2)->mLocation.mPath.rfind("/");
				if (delRightIdx != string::npos && delRightIdx < pathSearchIdx)
					continue;

				path->mFiles.push_back(*file2);
			}
		}
	}

	//restore path<->atlas links
	FOREACH(BuildFileInfoVec, mFileInfos, file)
	{
		if ((*file)->mType == cBuildFileInfo::MT_FOLDER)
		{
			cBuildPathInfo* path = static_cast<cBuildPathInfo*>(*file);
			path->attachAtlas(getAtlas(path->mAttachedAtlasName));
		}
	}
}

cImageAtlasInfo* cBuildInfo::addAtlas( const string& name, const vec2f& maxSize, cBuildPathInfo* attachingPath /*= NULL*/ )
{
	cImageAtlasInfo* newAtlas = mnew cImageAtlasInfo();
	newAtlas->setName(name);
	newAtlas->mMaxSize = maxSize;
	mAtlases.push_back(newAtlas);
	newAtlas->attachPath(attachingPath);

	return newAtlas;
}

cImageAtlasInfo* cBuildInfo::getAtlas( const string& name ) const
{
	FOREACH_CONST(AtlasesVec, mAtlases, atl)
		if ((*atl)->getName() == name)
			return *atl;

	return NULL;
}

void cBuildInfo::removeAtlas( const string& name )
{
	FOREACH(AtlasesVec, mAtlases, atl) 
	{
		if ((*atl)->getName() == name)
		{
			//copy images metas
			FOREACH(BuildImageInfoVec, (*atl)->mImages, img)
				mBasicAtlas->addImage(*img);
			(*atl)->mImages.clear();

			safe_release(*atl);
			mAtlases.erase(atl);
			return;
		}
	}		
}



CLOSE_O2_NAMESPACE