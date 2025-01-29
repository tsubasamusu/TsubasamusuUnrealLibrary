#include "Storage/TsubasamusuLocalFileLibrary.h"
#include "Debug/TsubasamusuLogLibrary.h"

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

bool UTsubasamusuLocalFileLibrary::SaveFileToLocal(const FString& LocalFileName, const TArray<uint8>& FileData, bool bOverwriteIfAlreadyExists)
{
    if (LocalFileExists(LocalFileName) && !bOverwriteIfAlreadyExists)
    {
        UTsubasamusuLogLibrary::LogError(TEXT("\"") + LocalFileName + TEXT("\" already exists."));

        return false;
    }

    if (FFileHelper::SaveArrayToFile(FileData, *GetLocalFilePath(LocalFileName))) return true;

    UTsubasamusuLogLibrary::LogError(TEXT("Failed to save \"") + LocalFileName + TEXT("\"."));

    return false;
}

TArray<uint8> UTsubasamusuLocalFileLibrary::GetLocalFileData(const FString& LocalFileName)
{
    TArray<uint8> FileData;

    if (FFileHelper::LoadFileToArray(FileData, *GetLocalFilePath(LocalFileName))) return FileData;

    return TArray<uint8>();
}

FString UTsubasamusuLocalFileLibrary::GetLocalFilePath(const FString& LocalFileName)
{
    return FPaths::ProjectSavedDir() / LocalFileName;
}