#include "asset_build_system.h"

#include "assets/assets.h"
#include "asset_building_convertor.h"
#include "app/application.h"
#include "util/file_system/file_system.h"
#include "config/asset_config.h"
#include "config/project_config.h"
#include "asset_file_building_convertor.h"
#include "asset_folder_building_convertor.h"
#include "asset_image_building_convertor.h"
#include "asset_atlas_building_convertor.h"

OPEN_O2_NAMESPACE

AssetBuildSystem::AssetBuildSystem(Assets* assets):
	mAssets(assets)
{
	mAssetsFolderPath = ASSETS_PATH;
	mBuildedAssetsFolderPath = ASSETS_BUILDED_PATH;
	mAssetsFolderConfigFilePath = ASSETS_FOLDER_CONFIG_FILE_PATH;
	mBuildedAssetsInfoFilePath = ASSETS_BUILDED_INFO_FILE_PATH;
	
	mAssetConverters.add(mnew asAssetFileBuildingConvertor(this));
	mAssetConverters.add(mnew asAssetFolderBuildingConvertor(this));
	mAssetConverters.add(mnew asAssetImageBuildingConvertor(this));
	mAssetConverters.add(mnew asAssetAtlasBuildingConvertor(this));
}

AssetBuildSystem::~AssetBuildSystem()
{
	release_array(AsConvertersArr, mAssetConverters);
}

void AssetBuildSystem::rebuildAssets(bool forcible /*= false*/)
{
	if (forcible)
		removeAllBuildedAssets();

	loadAssetFolderInfo();
	loadBuildedAssetsFolderInfo();

	checkMovedFiles(mAssetsFolderInfo, mBuildedAssetsFolderInfo);
	checkRemovedFiles(mAssetsFolderInfo, mBuildedAssetsFolderInfo);
	convertFiles(mAssetsFolderInfo, mBuildedAssetsFolderInfo);
	checkNewFiles(mAssetsFolderInfo, mBuildedAssetsFolderInfo);

	saveBuildInfo();
	saveAssetsInfo();
}

void AssetBuildSystem::removeAllBuildedAssets()
{
	fileSystem()->removeDirectory(mBuildedAssetsFolderPath);
	fileSystem()->createDirectory(mBuildedAssetsFolderPath);
	fileSystem()->deleteFile(fileSystem()->getFilePathByExt(mBuildedAssetsInfoFilePath, FileType::CONFIG));
}

void AssetBuildSystem::loadAssetFolderInfo()
{
	mAssetsFolderInfo.clear();

	//load assets path info
	PathInfo assetsPathInfo = fileSystem()->getPathInfo(mAssetsFolderPath);
	assetsPathInfo.clampPathNames();

	//process loading assets infos
	processLoadingAssetsFolderInfo(assetsPathInfo, mAssets->mAssetsConfigs, mAssetsFolderInfo);

	//add fake basic atlas asset
	abAtlasAssetInfo* basicAtlas = mnew abAtlasAssetInfo();
	basicAtlas->mName = "basic";
	basicAtlas->mAttachedToFolder = true;
	basicAtlas->mAttachFolderLocation = mAssetsFolderInfo.mLocation;
	basicAtlas->mLocation = FileLocation("basic_atlas");
	mAssetsFolderInfo.addInsideAsset(basicAtlas);

	//make links between folders, atlases and images
	mAssetsFolderInfo.linkAtlases();

	//save configs
	Serializer outSerializer(Serializer::ST_SERIALIZE);
	outSerializer.serialize(&mAssets->mAssetsConfigs, "assetsConfigs");
	outSerializer.save(mAssetsFolderConfigFilePath);
}

void AssetBuildSystem::processLoadingAssetsFolderInfo(PathInfo& pathInfo, asFolderConfig& pathConfig, abFolderInfo& asPathInfo)
{
	foreach(PathInfo::FilesArr, pathInfo.mFiles, fileInfIt)
	{
		//skip metas
		if (fileInfIt->mPath.rfind(".meta") != string::npos)
			continue;

		//create asset info by file info type
		abAssetInfo* asFileInfo = createAssetInfroFromFileInfo(*fileInfIt);
		
		//copy basic parametres for asset info
		asFileInfo->mLocation.mPath = fileInfIt->mPath;
		asFileInfo->mLocation.mId = tryGetAssetsInfoMetaId(pathInfo, fileInfIt->mPath);
		asFileInfo->mWriteTime = fileInfIt->mEditDate;		

		//seek asset config and remove from searched place
		asAssetConfig* asFileConfig = mAssets->mAssetsConfigs.getAndRemoveAssetConfig(asFileInfo->mLocation);

		//skip if asset not included 
		if (asFileConfig && !asFileConfig->mIncludeBuild)
		{
			safe_release(asFileInfo);
			continue;
		}

		//copy configuration into asset info, if config was found. Else - create new config
		if (asFileConfig)
			asFileInfo->initFromConfigs(asFileConfig);
		else
			asFileConfig = asFileInfo->getConfigsSample();
		
		//add asset config at right place in configs hierarhy
		pathConfig.mInsideAssets.add(asFileConfig);

		//and add asset info in assets infos hierarhy
		asPathInfo.addInsideAsset(asFileInfo);
	}

	foreach(PathInfo::PathsArr, pathInfo.mPaths, pathInfIt)
	{
		//create folder asset info
		abFolderInfo* folderInfo = mnew abFolderInfo();
		
		//copy location parametres for folder asset info
		folderInfo->mLocation.mPath = pathInfIt->mPath;
		folderInfo->mLocation.mId = tryGetAssetsInfoMetaId(pathInfo, pathInfIt->mPath);	
		
		//seek asset config and remove from searched place
		asAssetConfig* asFldrConfig = mAssets->mAssetsConfigs.getAndRemoveAssetConfig(pathInfIt->mPath);
		if (asFldrConfig && !asFldrConfig->mIncludeBuild)
		{
			safe_release(folderInfo);
			continue;
		}	

		//create new config, if needs
		if (!asFldrConfig) 
			asFldrConfig = folderInfo->getConfigsSample();

		//add asset config at right place in configs hierarhy
		pathConfig.mInsideAssets.add(asFldrConfig);
		
		//and add asset info in assets infos hierarhy
		asPathInfo.addInsideAsset(folderInfo);

		//process files inside that folder
		processLoadingAssetsFolderInfo(*pathInfIt, *(asFolderConfig*)asFldrConfig, *folderInfo);
	}
}

void AssetBuildSystem::loadBuildedAssetsFolderInfo()
{
	mBuildedAssetsFolderInfo.clear();

	Serializer serializer;
	if (serializer.load(mBuildedAssetsInfoFilePath))
		serializer.serialize(&mBuildedAssetsFolderInfo, "assets");

	mBuildedAssetsFolderInfo.linkAtlases();
}

void AssetBuildSystem::checkMovedFiles(abFolderInfo& assetFolderInfo, abFolderInfo& buildedAssetFolderInfo)
{

}

void AssetBuildSystem::checkRemovedFiles(abFolderInfo& assetFolderInfo, abFolderInfo& buildedAssetFolderInfo)
{
	//remove files first
	foreach_rem(abAssetsInfosArr, buildedAssetFolderInfo.mInsideAssets, assetIt) 
	{
		if ((*assetIt)->getType() == abFolderInfo::getStaticType()) 
		{
			++assetIt;
			continue;
		}

		abAssetInfo* assetInfo = assetFolderInfo.getInsideAsset((*assetIt)->mLocation);
		if (assetInfo == NULL) 
		{
			//remove builded asset
			removeConvertedAsset(*assetIt);
			assetIt = buildedAssetFolderInfo.mInsideAssets.remove(assetIt);
			continue;
		}

		++assetIt;
	}
	
	//after remove folders
	foreach_rem(abAssetsInfosArr, buildedAssetFolderInfo.mInsideAssets, assetIt) 
	{
		if ((*assetIt)->getType() != abFolderInfo::getStaticType()) 
		{
			++assetIt;
			continue;
		}		

		abAssetInfo* assetInfo = assetFolderInfo.getInsideAsset((*assetIt)->mLocation);
		if (assetInfo == NULL) 
		{
			//remove builded asset
			removeConvertedAsset(*assetIt);
			assetIt = buildedAssetFolderInfo.mInsideAssets.remove(assetIt);
			continue;
		}
		else checkRemovedFiles(*static_cast<abFolderInfo*>(assetInfo), *static_cast<abFolderInfo*>(*assetIt));

		++assetIt;
	}
}

void AssetBuildSystem::checkNewFiles(abFolderInfo& assetFolderInfo, abFolderInfo& buildedAssetFolderInfo)
{
	//create folders first
	foreach(abAssetsInfosArr, assetFolderInfo.mInsideAssets, assetIt)
	{		
		if ((*assetIt)->getType() != abFolderInfo::getStaticType()) 
			continue;

		abAssetInfo* assetInfo = buildedAssetFolderInfo.getInsideAsset((*assetIt)->mLocation);
		if (assetInfo == NULL)
		{
			assetInfo = (*assetIt)->clone();
			buildedAssetFolderInfo.addInsideAsset(assetInfo);

			//copy and convert new asset
			copyAndConvertAsset(*assetIt, assetInfo);
		}

		checkNewFiles(*static_cast<abFolderInfo*>(*assetIt), *static_cast<abFolderInfo*>(assetInfo));
	}
	
	//after create files
	foreach(abAssetsInfosArr, assetFolderInfo.mInsideAssets, assetIt)
	{		
		if ((*assetIt)->getType() == abFolderInfo::getStaticType()) 
			continue;

		abAssetInfo* assetInfo = buildedAssetFolderInfo.getInsideAsset((*assetIt)->mLocation);
		if (assetInfo == NULL)
		{
			assetInfo = (*assetIt)->clone();
			buildedAssetFolderInfo.addInsideAsset(assetInfo);

			//copy and convert new asset
			copyAndConvertAsset(*assetIt, assetInfo);
		}
	}
}

void AssetBuildSystem::convertFiles(abFolderInfo& assetFolderInfo, abFolderInfo& buildedAssetFolderInfo)
{
	foreach(abAssetsInfosArr, assetFolderInfo.mInsideAssets, assetIt)
	{
		abAssetInfo* assetInfo = buildedAssetFolderInfo.getInsideAsset((*assetIt)->mLocation);
		if (!assetInfo)
			continue;

		if (!assetInfo->isEquals(*assetIt))
		{
			//copy and convert new asset
			removeConvertedAsset(assetInfo);
			copyAndConvertAsset(*assetIt, assetInfo);
		}

		if ((*assetIt)->getType() == abFolderInfo::getStaticType())
			convertFiles(*static_cast<abFolderInfo*>(*assetIt), *static_cast<abFolderInfo*>(assetInfo));
	}
}

void AssetBuildSystem::copyAndConvertAsset(abAssetInfo* assetInfo, abAssetInfo* buildAssetInfo)
{
	buildAssetInfo->copyFrom(assetInfo);

	foreach(AsConvertersArr, mAssetConverters, convIt)
	{
		if ((*convIt)->getConvertingType() == assetInfo->getType())
			(*convIt)->convert(assetInfo);
	}
}

void AssetBuildSystem::removeConvertedAsset(abAssetInfo* buildAssetInfo)
{
	foreach(AsConvertersArr, mAssetConverters, convIt)
	{
		if ((*convIt)->getConvertingType() == buildAssetInfo->getType())
			(*convIt)->remove(buildAssetInfo);
	}
}

uint32 AssetBuildSystem::tryGetAssetsInfoMetaId(PathInfo &pathInfo, const string& path)
{
	uint32 res = 0;

	string metaPath = path + ".meta";

	if (pathInfo.isFileExist(metaPath))
	{
		Serializer serializer;
		serializer.load(mAssetsFolderPath + "/" + metaPath, false);
		serializer.serialize(res, "id");
	}
	else if (projectConfig()->mAssetsUsesMetaIds) 
	{
		uint32 metaId = generateFileId();
		Serializer serializer;
		serializer.serialize(metaId, "id");
		serializer.save(mAssetsFolderPath + "/" + metaPath, false);
		return metaId;
	}

	return res;
}

uint32 AssetBuildSystem::generateFileId() const
{
	return mAssets->generateFileId();
}

string AssetBuildSystem::getAssetsFolderPath() const
{
	return mAssetsFolderPath;
}

string AssetBuildSystem::getBuildedAssetsFolderPath() const
{
	return mBuildedAssetsFolderPath;
}

abAssetInfo* AssetBuildSystem::createAssetInfroFromFileInfo(const FileInfo& fileInfo)
{
	if (fileInfo.mFileType == FileType::IMAGE)
		return mnew abImageAssetInfo();
	else if (fileInfo.mFileType == FileType::ATLAS)
		return mnew abAtlasAssetInfo();

	return mnew abAssetInfo();
}

void AssetBuildSystem::saveBuildInfo()
{
	Serializer serializer;
	serializer.serialize(&mBuildedAssetsFolderInfo, "assets");
	serializer.save(mBuildedAssetsInfoFilePath);
}

void AssetBuildSystem::saveAssetsInfo()
{
	abAssetsInfosArr assetsBuildInfos = mBuildedAssetsFolderInfo.getAllInsideAssets();
	AssetsInfosArr assetsInfos;
	foreach(abAssetsInfosArr, assetsBuildInfos, assetIt)
	{
		FileLocation loc = (*assetIt)->mLocation;
		loc.mPath = extractExtension(loc.mPath);
		assetsInfos.add(asAssetInfo(loc, (*assetIt)->getTypeName(), (*assetIt)->mWriteTime));
	}

	Serializer serializer;
	serializer.serialize(assetsInfos, "assets");
	serializer.save(ASSETS_INFO_FILE_PATH);
}

CLOSE_O2_NAMESPACE