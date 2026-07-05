/******************************************************************************
 * Spine Runtimes License Agreement
 * Last updated July 28, 2023. Replaces all prior versions.
 *
 * Copyright (c) 2013-2023, Esoteric Software LLC
 *
 * Integration of the Spine Runtimes into software or otherwise creating
 * derivative works of the Spine Runtimes is permitted under the terms and
 * conditions of Section 2 of the Spine Editor License Agreement:
 * http://esotericsoftware.com/spine-editor-license
 *
 * Otherwise, it is permitted to integrate the Spine Runtimes into software or
 * otherwise create derivative works of the Spine Runtimes (collectively,
 * "Products"), provided that each user of the Products must obtain their own
 * Spine Editor license and redistribution of the Products in any form must
 * include this license and copyright notice.
 *
 * THE SPINE RUNTIMES ARE PROVIDED BY ESOTERIC SOFTWARE LLC "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ESOTERIC SOFTWARE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES,
 * BUSINESS INTERRUPTION, OR LOSS OF USE, DATA, OR PROFITS) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THE
 * SPINE RUNTIMES, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#include "SpineAtlasImportFactory.h"
#include "Windows/WindowsPlatformMisc.h"
#include <windows.h>
#include <string.h>
#include <string>

#include "AssetToolsModule.h"
#include "SpineAtlasAsset.h"
#include "Editor.h"
#include "EditorAssetLibrary.h"
#include "CoreMinimal.h"
#include "AssetRegistry/AssetRegistryModule.h"

#define LOCTEXT_NAMESPACE "Spine"

using namespace spine;

USpineAtlasAssetFactory::USpineAtlasAssetFactory(const FObjectInitializer &objectInitializer) : Super(objectInitializer) {
	bCreateNew = false;
	bEditAfterNew = true;
	bEditorImport = true;
	SupportedClass = USpineAtlasAsset::StaticClass();

	Formats.Add(TEXT("atlas;Spine Atlas file"));
}

FText USpineAtlasAssetFactory::GetToolTip() const {
	return LOCTEXT("SpineAtlasAssetFactory", "Animations exported from Spine");
}

bool USpineAtlasAssetFactory::FactoryCanImport(const FString &Filename) {
	return true;
}

static bool RenameAsset(UObject* Asset, const FString& Suffix)
{
	if (!Asset) return false;

	FString NewName = Asset->GetName() + Suffix;

	// ��ȡ�ʲ�����·��
	UPackage* AssetPackage = Asset->GetOutermost();
	FString PackageName = AssetPackage->GetName();
	FString NewAssetPath = PackageName + "/" + NewName;

	// ������
	FAssetRegistryModule::AssetRenamed(Asset, *NewAssetPath);
	Asset->Rename(*NewName, AssetPackage);
	Asset->MarkPackageDirty();

	// �����ʲ�
	FAssetRegistryModule::AssetCreated(Asset);

	return true;
}
bool RenameExternalFile(const FString& OldFilePath, const FString& NewFilePath)
{
	std::wstring OldFilePathW = std::wstring(*OldFilePath);
	std::wstring NewFilePathW = std::wstring(*NewFilePath);
	// �����������ļ�
	if (MoveFile(OldFilePathW.c_str(), NewFilePathW.c_str()))
	{
		// �ɹ�������
		return true;
	}
	else
	{
		// �������
		DWORD error = GetLastError();
		wprintf(L"Failed to rename file from %s to %s. Error code: %lu\n", OldFilePathW.c_str(), NewFilePathW.c_str(), error);
		return false;
	}
}
bool RenameAsset2(UObject* Asset, const FString& NewName)
{
	if (!Asset) return false;

	// ��ȡ�ʲ��İ���·��
	UPackage* AssetPackage = Asset->GetOutermost();
	FString OldAssetPath = Asset->GetPathName();
	FString NewAssetPath = AssetPackage->GetName() + "/" + NewName;

	// ����������Ƿ���Ч
	if (FPackageName::IsValidObjectPath(NewAssetPath))
	{
		// ִ��������
		Asset->Rename(*NewName, AssetPackage);
		Asset->MarkPackageDirty();

		// �����ʲ�
		if (UEditorAssetLibrary::SaveAsset(OldAssetPath))
		{
			// �����ʲ�ע��
			FAssetRegistryModule::AssetRenamed(Asset, *NewAssetPath);
			return true;
		}
	}

	return false;
}
UObject *USpineAtlasAssetFactory::FactoryCreateFile(UClass *InClass, UObject *InParent, FName InName, EObjectFlags Flags, const FString &Filename, const TCHAR *Parms, FFeedbackContext *Warn, bool &bOutOperationCanceled) {
	
	FString FileExtension = FPaths::GetExtension(Filename);
	GEditor->GetEditorSubsystem<UImportSubsystem>()->BroadcastAssetPreImport(this, InClass, InParent, InName, *FileExtension);

	FString rawString;
	if (!FFileHelper::LoadFileToString(rawString, *Filename)) {
		return nullptr;
	}

	FString currentSourcePath, filenameNoExtension, unusedExtension;
	const FString longPackagePath = FPackageName::GetLongPackagePath(InParent->GetOutermost()->GetPathName());
	FPaths::Split(UFactory::GetCurrentFilename(), currentSourcePath, filenameNoExtension, unusedExtension);

	USpineAtlasAsset *asset = NewObject<USpineAtlasAsset>(InParent, InClass, InName, Flags);
	asset->SetRawData(rawString);
	asset->SetAtlasFileName(FName(*Filename));
	LoadAtlas(asset, currentSourcePath, longPackagePath);
	GEditor->GetEditorSubsystem<UImportSubsystem>()->BroadcastAssetPostImport(this, asset);
	//if(RenameAsset2(asset,InName.ToString()+FString("_Atlas")))
	//	UE_LOG(LogTemp,Log,TEXT("RenameYea"));
	//FAssetRegistryModule::AssetCreated(asset);
	//UEditorAssetLibrary::SaveAsset(asset->GetOutermost()->GetName());
	return asset;
}
/*
FString FileExtension = FPaths::GetExtension(Filename);
	UE_LOG(LogTemp,Log,TEXT("%s"),*InName.ToString());
	FString BaseName = FPaths::GetBaseFilename(Filename);
	UE_LOG(LogTemp,Log,TEXT("BaseName %s"),*BaseName);
	UE_LOG(LogTemp,Log,TEXT("FileName %s"),*Filename);
	FString LocalfilenameNoExtension;
	FString LocalcurrentSourcePath;
	FString LocalunusedExtension;
	FPaths::Split(UFactory::GetCurrentFilename(), LocalcurrentSourcePath, LocalfilenameNoExtension, LocalunusedExtension);
	
	if(Filename.Find(FString("_Atlas"))==-1)
	{
		LocalcurrentSourcePath+=FString("\\")+BaseName + FString("_Atlas.atlas");
		UE_LOG(LogTemp,Log,TEXT("LocalCurrentSource %s"),*LocalcurrentSourcePath);
		//RenameExternalFile(Filename,LocalcurrentSourcePath);
		FString A = BaseName + FString("_Atlas.atlas");
		//return FactoryCreateFile(InClass,InParent,*A,Flags,LocalcurrentSourcePath,Parms,Warn,bOutOperationCanceled);
	}
	
	BaseName = FPaths::GetBaseFilename(Filename);
	//RenameExternalFile(Filename,LocalcurrentSourcePath)
	FName NewAssetName = *BaseName;
	UE_LOG(LogTemp,Log,TEXT("NewAssetName %s"),*NewAssetName.ToString());
	
	//MakeUniqueObjectName(InParent, InClass, FName(*(BaseName + TEXT("_Atlas"))));
	GEditor->GetEditorSubsystem<UImportSubsystem>()->BroadcastAssetPreImport(this, InClass, InParent, NewAssetName, *FileExtension);
	//InName = FName(InName.ToString()+FName("_Atlas").ToString());
	FString rawString;
	if (!FFileHelper::LoadFileToString(rawString, *LocalcurrentSourcePath)) {
		return nullptr;
	}

	FString currentSourcePath, filenameNoExtension, unusedExtension;
	//filenameNoExtension = FString("_Atlas");
	const FString longPackagePath = FPackageName::GetLongPackagePath(InParent->GetOutermost()->GetPathName());
	UE_LOG(LogTemp,Log,TEXT("%s"),*longPackagePath);
	FPaths::Split(UFactory::GetCurrentFilename(), currentSourcePath, filenameNoExtension, unusedExtension);
	UE_LOG(LogTemp,Log,TEXT("%s"),*currentSourcePath);
	
	
	USpineAtlasAsset *asset = NewObject<USpineAtlasAsset>(InParent, InClass,NewAssetName, Flags);
	//asset->Rename(*MakeUniqueObjectName(InParent, InClass, FName(*(BaseName + TEXT("_Atlas")))).ToString());
	asset->SetRawData(rawString);
	asset->SetAtlasFileName(NewAssetName);
	//asset->Rename(*FName(*(BaseName + TEXT("_Atlas"))).ToString());
	//RenameAsset(asset,FName(*(BaseName + TEXT("_Atlas"))).ToString());
	FAssetRegistryModule::AssetCreated(asset);
	//asset->MarkPackageDirty();

	// ���м��ز���
	LoadAtlas(asset, currentSourcePath, longPackagePath);
	//asset->SetAtlasFileName(FName(*Filename));
	//LoadAtlas(asset, currentSourcePath, longPackagePath);
	GEditor->GetEditorSubsystem<UImportSubsystem>()->BroadcastAssetPostImport(this, asset);

	;
	
	//FAssetRegistryModule::AssetCreated(asset);
	// �����ʲ�
	//UEditorAssetLibrary::SaveAsset(PackageName);
	return asset;
 */
bool USpineAtlasAssetFactory::CanReimport(UObject *Obj, TArray<FString> &OutFilenames) {
	USpineAtlasAsset *asset = Cast<USpineAtlasAsset>(Obj);
	if (!asset) return false;

	FString filename = asset->GetAtlasFileName().ToString();
	if (!filename.IsEmpty())
		OutFilenames.Add(filename);

	return true;
}

void USpineAtlasAssetFactory::SetReimportPaths(UObject *Obj, const TArray<FString> &NewReimportPaths) {
	USpineAtlasAsset *asset = Cast<USpineAtlasAsset>(Obj);

	if (asset && ensure(NewReimportPaths.Num() == 1))
		asset->SetAtlasFileName(FName(*NewReimportPaths[0]));
}

EReimportResult::Type USpineAtlasAssetFactory::Reimport(UObject *Obj) {
	USpineAtlasAsset *asset = Cast<USpineAtlasAsset>(Obj);
	FString rawString;
	if (!FFileHelper::LoadFileToString(rawString, *asset->GetAtlasFileName().ToString())) return EReimportResult::Failed;
	asset->SetRawData(rawString);

	FString currentSourcePath, filenameNoExtension, unusedExtension;
	const FString longPackagePath = FPackageName::GetLongPackagePath(asset->GetOutermost()->GetPathName());
	FString currentFileName = asset->GetAtlasFileName().ToString();
	FPaths::Split(currentFileName, currentSourcePath, filenameNoExtension, unusedExtension);

	LoadAtlas(asset, currentSourcePath, longPackagePath);

	if (Obj->GetOuter()) Obj->GetOuter()->MarkPackageDirty();
	else
		Obj->MarkPackageDirty();

	GEditor->GetEditorSubsystem<UImportSubsystem>()->BroadcastAssetReimport(asset);
	return EReimportResult::Succeeded;
}

UTexture2D *resolveTexture(USpineAtlasAsset *Asset, const FString &PageFileName, const FString &TargetSubPath) {
	FAssetToolsModule &AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");

	TArray<FString> fileNames;
	fileNames.Add(PageFileName);

	TArray<UObject *> importedAsset = AssetToolsModule.Get().ImportAssets(fileNames, TargetSubPath);
	UTexture2D *texture = (importedAsset.Num() > 0) ? Cast<UTexture2D>(importedAsset[0]) : nullptr;

	return texture;
}

void USpineAtlasAssetFactory::LoadAtlas(USpineAtlasAsset *Asset, const FString &CurrentSourcePath, const FString &LongPackagePath) {
	Atlas *atlas = Asset->GetAtlas();
	Asset->atlasPages.Empty();

	const FString targetTexturePath = LongPackagePath / TEXT("Textures");

	Vector<AtlasPage *> &pages = atlas->getPages();
	for (size_t i = 0, n = pages.size(); i < n; i++) {
		AtlasPage *page = pages[i];
		const FString sourceTextureFilename = FPaths::Combine(*CurrentSourcePath, UTF8_TO_TCHAR(page->name.buffer()));
		UTexture2D *texture = resolveTexture(Asset, sourceTextureFilename, targetTexturePath);
		Asset->atlasPages.Add(texture);
	}
}

#undef LOCTEXT_NAMESPACE
