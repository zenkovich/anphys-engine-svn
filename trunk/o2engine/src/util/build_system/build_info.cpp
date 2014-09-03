#include "build_info.h"
#include "atlas_info.h"

OPEN_O2_NAMESPACE
	

cBuildInfo::cBuildInfo()
{
	mBasicAtlas = mnew cImageAtlasInfo();
}

cBuildInfo::~cBuildInfo()
{
	RELEASE_VECTOR(FilesMetaVec, mFilesMeta);
	RELEASE_VECTOR(AtlasesVec, mAtlases);
	safe_release(mBasicAtlas);
}

void cBuildInfo::addFile(cBuildSystem::FileMeta* meta)
{
	mFilesMeta.push_back(meta);
}

void cBuildInfo::removeFile(cBuildSystem::FileMeta* meta)
{
	FOREACH(FilesMetaVec, mFilesMeta, metaIt)
	{
		if (meta->mMetaId == (*metaIt)->mMetaId)
		{
			mFilesMeta.erase(metaIt);
			safe_release(meta);
			return;
		}
	}
}

cBuildSystem::FileMeta* cBuildInfo::findFile(uint32 id)
{
	FOREACH(FilesMetaVec, mFilesMeta, metaIt)
	{
		if (id == (*metaIt)->mMetaId)
			return *metaIt;
	}

	return NULL;
}

SERIALIZE_METHOD_IMPL(cBuildInfo)
{
	SERIALIZE_ID(mFilesMeta, "filesMeta");
	SERIALIZE_ID(mAtlases, "atlases");
	SERIALIZE_ID(mBasicAtlas, "basicAtlas");

	return true;
}

cImageAtlasInfo* cBuildInfo::addAtlas( const string& name, const vec2f& maxSize, const string& attachingPath /*= ""*/ )
{
	cImageAtlasInfo* newAtlas = mnew cImageAtlasInfo();
	newAtlas->mName = name;
	newAtlas->mMaxSize = maxSize;
	newAtlas->mAttachedPath = attachingPath;
	mAtlases.push_back(newAtlas);
	return newAtlas;
}

cImageAtlasInfo* cBuildInfo::getAtlas( const string& name ) const
{
	FOREACH_CONST(AtlasesVec, mAtlases, atl)
		if ((*atl)->mName == name)
			return *atl;

	return NULL;
}

void cBuildInfo::removeAtlas( const string& name )
{
	FOREACH(AtlasesVec, mAtlases, atl) 
	{
		if ((*atl)->mName == name)
		{
			//copy images metas
			FOREACH(cImageAtlasInfo::ImagesMetaVec, (*atl)->mImages, img)
				mBasicAtlas->addImage( static_cast<cBuildSystem::ImageFileMeta*>((*img)->clone()) );

			safe_release(*atl);
			mAtlases.erase(atl);
			return;
		}
	}		
}

CLOSE_O2_NAMESPACE