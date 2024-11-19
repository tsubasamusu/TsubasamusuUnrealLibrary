#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TsubasamusuLocalFileLibrary.generated.h"

UCLASS()
class TSUBASAMUSUUNREALLIBRARY_API UTsubasamusuLocalFileLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "TSUBASAMUSU|Storage")
	static TArray<uint8> LoadLocalFile(const FString& LocalFileName);

	UFUNCTION(BlueprintPure, Category = "TSUBASAMUSU|Storage")
	static bool LocalFileExists(const FString& LocalFileName);

private:
	static TArray<uint8> GetLocalFileData(const FString& LocalFileName);
};