#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TsubasamusuLogLibrary.generated.h"

UCLASS()
class TSUBASAMUSUUNREALLIBRARY_API UTsubasamusuLogLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "TSUBASAMUSU|Debug", meta = (Keywords = "log"))
	static void Log(const FString& Message);
	
	UFUNCTION(BlueprintCallable, Category = "TSUBASAMUSU|Debug", meta = (Keywords = "log error"))
	static void LogError(const FString& Message);
	
	UFUNCTION(BlueprintCallable, Category = "TSUBASAMUSU|Debug", meta = (Keywords = "log warning"))
	static void LogWarning(const FString& Message);
};