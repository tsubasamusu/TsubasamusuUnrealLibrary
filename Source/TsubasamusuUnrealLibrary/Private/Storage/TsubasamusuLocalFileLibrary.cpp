#include "Storage/TsubasamusuLocalFileLibrary.h"

TArray<uint8> UTsubasamusuLocalFileLibrary::LoadLocalFile(const FString& LocalFileName)
{
    TArray<uint8> LocalFileData = GetLocalFileData(LocalFileName);

    if (LocalFileData.Num() > 0) return LocalFileData;

    UTsubasamusuLogLibrary::LogError(TEXT("Failed to load \"") + LocalFileName + TEXT("\"."));

    return TArray<uint8>();
}

bool UTsubasamusuLocalFileLibrary::LocalFileExists(const FString& LocalFileName)
{
    return GetLocalFileData(LocalFileName).Num() > 0;
}

TArray<uint8> UTsubasamusuLocalFileLibrary::GetLocalFileData(const FString& LocalFileName)
{
    FString FilePath = FPaths::ProjectSavedDir() / LocalFileName;

    TArray<uint8> FileData;

    if (FFileHelper::LoadFileToArray(FileData, *FilePath)) return FileData;

    return TArray<uint8>();
}