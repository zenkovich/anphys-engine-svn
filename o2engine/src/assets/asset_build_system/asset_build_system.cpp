#include "asset_build_system.h"

#include "assets/assets.h"
#include "asset_building_convertor.h"
#include "app/application.h"
#include "util/file_system/file_system.h"
#include "config/asset_config.h"
#include "asset_file_building_convertor.h"
#include "asset_folder_building_convertor.h"
#include "asset_image_building_convertor.h"

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

	checkRemovedFiles(mAssetsFolderInfo, mBuildedAssetsFolderInfo);
	checkNewFiles(mAssetsFolderInfo, mBuildedAssetsFolderInfo);
	convertFiles(mAssetsFolderInfo, mBuildedAssetsFolderInfo);

	saveBuildInfo();
}

void AssetBuildSystem::removeAllBuildedAssets()
{
	fileSystem()->removeDirectory(mBuildedAssetsFolderPath);
	fileSystem()->createDirectory(mBuildedAssetsFolderPath);
	fileSystem()->deleteFile(fileSystem()->getFilePathByExt(mBuildedAssetsInfoFilePath, cFileType::CONFIG));
}

void AssetBuildSystem::loadAssetFolderInfo()
{
	mAssetsFolderInfo.clear();

	//load assets path info
	cPathInfo assetsPathInfo = fileSystem()->getPathInfo(mAssetsFolderPath);
	assetsPathInfo.clampPathNames();

	//load assets configs
	asPathConfig assetsPathConfig;
	cSerializer serializer;
	if (serializer.load(mAssetsFolderConfigFilePath))
		serializer.serialize(&assetsPathConfig, "assetsConfigs");

	//process loading assets infos
	processLoadingAssetsFolderInfo(assetsPathInfo, assetsPathConfig, mAssetsFolderInfo);

	//save configs
	cSerializer outSerializer(cSerializer::ST_SERIALIZE);
	outSerializer.serialize(&assetsPathConfig, "assetsConfigs");
	outSerializer.save(mAssetsFolderConfigFilePath);
}

void AssetBuildSystem::processLoadingAssetsFolderInfo(cPathInfo& pathInfo, asPathConfig& pathConfig, abFolderInfo& asPathInfo)
{
	foreach(cPathInfo::FilesArr, pathInfo.mFiles, fileInfIt)
	{
		if (fileInfIt->mPath.rfind(".meta") != string::npos)
			continue;

		asAssetConfig* asFileConfig = pathConfig.getAssetConfig(fileInfIt->mPath);
		if (asFileConfig && !asFileConfig->mIncludeBuild)
			continue;
		
		abAssetInfo* asFileInfo = createAssetInfroFromFileInfo(*fileInfIt);
		asFileInfo->mLocation.mPath = fileInfIt->mPath;
		asFileInfo->mLocation.mId = tryGetAssetsInfoMetaId(pathInfo, fileInfIt->mPath);
		asFileInfo->mWriteTime = fileInfIt->mEditDate;		

		if (asFileConfig)
			asFileInfo->initFromConfigs(asFileConfig);
		else
			pathConfig.mInsideAssets.add(asFileInfo->getConfigsSample());


		asPathInfo.mInsideAssets.add(asFileInfo);
	}

	foreach(cPathInfo::PathsArr, pathInfo.mPaths, pathInfIt)
	{
		asAssetConfig* asFileConfig = pathConfig.getAssetConfig(pathInfIt->mPath);
		if (asFileConfig && !asFileConfig->mIncludeBuild)
			continue;

		abFolderInfo* folderInfo = mnew abFolderInfo();

		folderInfo->mLocation.mPath = pathInfIt->mPath;
		folderInfo->mLocation.mId = tryGetAssetsInfoMetaId(pathInfo, pathInfIt->mPath);		

		if (!asFileConfig) 
			asFileConfig = pathConfig.mInsideAssets.add(folderInfo->initFromConfigs());

		asPathInfo.mInsideAssets.add(folderInfo);

		processLoadingAssetsFolderInfo(*pathInfIt, *(asPathConfig*)asFileConfig, *folderInfo);
	}
}

void AssetBuildSystem::loadBuildedAssetsFolderInfo()
{
	mBuildedAssetsFolderInfo.clear();

	cSerializer serializer;
	if (serializer.load(mBuildedAssetsInfoFilePath))
	{
		serializer.serialize(&mBuildedAssetsFolderInfo, "assets");
	}
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
		if (assetInfo && !assetInfo->isEquals(*assetIt))
		{
			//copy and convert new asset
			removeConvertedAsset(assetInfo);
			copyAndConvertAsset(*assetIt, assetInfo);
		}

		if (assetInfo)
		{
			if ((*assetIt)->getType() == abFolderInfo::getStaticType())
				convertFiles(*static_cast<abFolderInfo*>(*assetIt), *static_cast<abFolderInfo*>(assetInfo));
		}
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

uint32 AssetBuildSystem::tryGetAssetsInfoMetaId(cPathInfo &pathInfo, const string& path)
{
	uint32 res = 0;

	string metaPath = path + ".meta";

	if (pathInfo.isFileExist(metaPath))
	{
		cSerializer serializer(metaPath);
		serializer.serialize(res, "id");
	}

	return res;
}

string AssetBuildSystem::getAssetsFolderPath() const
{
	return mAssetsFolderPath;
}

string AssetBuildSystem::getBuildedAssetsFolderPath() const
{
	return mBuildedAssetsFolderPath;
}

abAssetInfo* AssetBuildSystem::createAssetInfroFromFileInfo(const cFileInfo& fileInfo)
{
	if (fileInfo.mFileType == cFileType::IMAGE)
		return mnew abImageAssetInfo();
	else if (fileInfo.mFileType == cFileType::ATLAS)
		return mnew abAtlasAssetInfo();

	return mnew abAssetInfo();
}

void AssetBuildSystem::saveBuildInfo()
{
	cSerializer serializer;
	serializer.serialize(&mBuildedAssetsFolderInfo, "assets");
	serializer.save(mBuildedAssetsInfoFilePath);
}

CLOSE_O2_NAMESPACE